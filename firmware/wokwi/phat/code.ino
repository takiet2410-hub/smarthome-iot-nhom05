#include <WiFi.h>
#include <PubSubClient.h>

// Hardware Pins (Update if your yellow wires are on different GPIOs)
const int RELAY_PIN = 26;
const int PIR_PIN   = 27;

// Wokwi Virtual Wi-Fi
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// HiveMQ Public Broker
const char* mqtt_server = "mqtt-dashboard.com";
const int   mqtt_port   = 1883;

// MQTT Topics
const char* topic_relay_cmd = "nhom05/smarthome/relay/cmd";
const char* topic_pir_cmd = "nhom05/smarthome/pir/cmd";
const char* topic_pir_state = "nhom05/smarthome/pir/status";

WiFiClient espClient;
PubSubClient client(espClient);

bool isSecurityArmed = true;
int lastPirState = LOW;

// Runs automatically when a message arrives from the Broker
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (String(topic) == topic_relay_cmd) {
    if (message == "ON") {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("-> Relay & LED turned ON");
    } else if (message == "OFF") {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("-> Relay & LED turned OFF");
    }
  }

  if (String(topic) == topic_pir_cmd) {
    if (message == "ON") {
      isSecurityArmed = true;
      lastPirState = digitalRead(PIR_PIN); 
      Serial.println("Da BAT che do an ninh");

      if (lastPirState == HIGH) {
        client.publish(topic_pir_state, "MOTION_DETECTED");
        Serial.println("-> Trang thai ban dau: Co chuyen dong!");
      } else {
        client.publish(topic_pir_state, "CLEAR");
        Serial.println("-> Trang thai ban dau: Khong con chuyen dong!");
      }
    } 
    else if (message == "OFF") {
      isSecurityArmed = false;
      Serial.println("Da TAT che do an ninh (Im lang)");
    }
  }
}

void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT Broker...");
    String clientId = "ESP32-Nhom05-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Connected!");
      client.subscribe(topic_relay_cmd);
      client.subscribe(topic_pir_cmd);
      client.publish(topic_pir_state, "ESP32_ONLINE");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 3s...");
      delay(3000);
    }
  }
}

void checkPIR() {
  if (!isSecurityArmed) return; // Đang tắt chế độ an ninh thì bỏ qua hoàn toàn

  int currentPirState = digitalRead(PIR_PIN);
  if (currentPirState != lastPirState) {
    lastPirState = currentPirState;

    if (currentPirState == HIGH) {
      client.publish("nhom05/smarthome/pir/status", "MOTION_DETECTED");
      Serial.println("Co chuyen dong!");
    } else {
      client.publish("nhom05/smarthome/pir/status", "CLEAR");
      Serial.println("Khong con chuyen dong!");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  checkPIR();

  delay(100);
}