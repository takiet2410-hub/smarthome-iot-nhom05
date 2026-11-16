#include "wifi_mqtt.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <vector>
#include "config.h"

#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #warning "secrets.h khong ton tai. Su dung secrets.example.h tam thoi."
  #include "secrets.example.h"
#endif

static WiFiClientSecure espClient;
static PubSubClient mqttClient(espClient);
static MQTTMessageCallback userCallback = nullptr;
static std::vector<String> subscribedTopics;
static unsigned long lastMqttReconnectAttempt = 0;

static void internalMqttCallback(char* topic, byte* payload, unsigned int length) {
    if (userCallback) {
        userCallback(topic, payload, length);
    }
}

static bool attemptMqttConnect() {
    Serial.print("[MQTT] Dang ket noi toi HiveMQ Cloud: ");
    Serial.println(MQTT_HOST);

    // Tao Client ID duy nhat cho ESP32
    String clientId = "ESP32-SmartHome-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    // Ket noi kem Last Will and Testament (LWT)
    // Neu ESP32 mat ket noi dot ngot, HiveMQ se tu dong publish "offline" (retained)
    bool connected = mqttClient.connect(
        clientId.c_str(),
        MQTT_USER,
        MQTT_PASS,
        TOPIC_SYSTEM_STATUS,
        1,
        true,
        LWT_PAYLOAD_OFFLINE
    );

    if (connected) {
        Serial.println("[MQTT] Ket noi thanh cong!");
        // Báo trạng thái online lên topic LWT
        mqttClient.publish(TOPIC_SYSTEM_STATUS, LWT_PAYLOAD_ONLINE, true);

        // Tu dong dang ky lai cac topic da subscribe truoc do
        for (const auto& topic : subscribedTopics) {
            mqttClient.subscribe(topic.c_str(), 1);
            Serial.print("[MQTT] Re-subscribed: ");
            Serial.println(topic);
        }
        return true;
    } else {
        Serial.print("[MQTT] Ket noi that bai, rc=");
        Serial.println(mqttClient.state());
        return false;
    }
}

void wifiMqttSetup(MQTTMessageCallback callback) {
    userCallback = callback;

    Serial.println();
    Serial.println("==========================================");
    Serial.println("       KHOI DONG HE THONG WIFI & MQTT      ");
    Serial.println("==========================================");

    // 1. Ket noi WiFi (Wokwi-GUEST tren kenh 6 cho moi truong Wokwi)
    Serial.print("[WiFi] Dang ket noi toi SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 40) {
        delay(300);
        Serial.print(".");
        retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.print("[WiFi] Da ket noi! IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("[WiFi] Canh bao: Chua the ket noi WiFi (se tu thu lai trong loop).");
    }

    // 2. Cau hinh TLS cho MQTT
    // setInsecure() giup bo qua buoc nap chung chi CA tren ESP32, phu hop cho do an sinh vien
    espClient.setInsecure();
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(internalMqttCallback);
    mqttClient.setBufferSize(512);

    // Thu ket noi lan dau
    if (WiFi.status() == WL_CONNECTED) {
        attemptMqttConnect();
    }
}

void wifiMqttLoop() {
    // 1. Kiem tra va duy tri WiFi
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }

    // 2. Kiem tra va duy tri ket noi MQTT (Non-blocking reconnect moi 5 giay)
    if (!mqttClient.connected()) {
        unsigned long now = millis();
        if (now - lastMqttReconnectAttempt > 5000) {
            lastMqttReconnectAttempt = now;
            if (attemptMqttConnect()) {
                lastMqttReconnectAttempt = 0;
            }
        }
    } else {
        mqttClient.loop();
    }
}

bool mqttPublish(const char* topic, const char* payload, bool retained) {
    if (!mqttClient.connected()) {
        return false;
    }
    return mqttClient.publish(topic, payload, retained);
}

bool mqttSubscribe(const char* topic, uint8_t qos) {
    // Luu lai topic vao danh sach de tu dong dang ky lai khi reconnect
    bool alreadyExists = false;
    for (const auto& t : subscribedTopics) {
        if (t == topic) {
            alreadyExists = true;
            break;
        }
    }
    if (!alreadyExists) {
        subscribedTopics.push_back(String(topic));
    }

    if (mqttClient.connected()) {
        return mqttClient.subscribe(topic, qos);
    }
    return false;
}

bool isMqttConnected() {
    return mqttClient.connected();
}
