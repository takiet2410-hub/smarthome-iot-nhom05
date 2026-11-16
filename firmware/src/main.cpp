#include <Arduino.h>
#include <ArduinoJson.h>
#include "core/wifi_mqtt.h"
#include "config.h"

// Xac dinh ten moi truong build
#if defined(BUILD_KIET) && defined(BUILD_QUY) && defined(BUILD_PHAT)
  #define CURRENT_ENV "integrated"
#elif defined(BUILD_KIET)
  #define CURRENT_ENV "kiet"
#elif defined(BUILD_QUY)
  #define CURRENT_ENV "quy"
#elif defined(BUILD_PHAT)
  #define CURRENT_ENV "phat"
#else
  #define CURRENT_ENV "default"
#endif

// Khai bao header module tuy theo env
#ifdef BUILD_KIET
  #include "modules/kiet/module_kiet.h"
#endif
#ifdef BUILD_QUY
  #include "modules/quy/module_quy.h"
#endif
#ifdef BUILD_PHAT
  #include "modules/phat/module_phat.h"
#endif

static unsigned long lastHelloPublish = 0;

void onMqttMessageReceived(char* topic, byte* payload, unsigned int length) {
    char message[256];
    if (length >= sizeof(message)) {
        length = sizeof(message) - 1;
    }
    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.println();
    Serial.println("------------------------------------------");
    Serial.print("[MQTT IN] Topic: ");
    Serial.println(topic);
    Serial.print("[MQTT IN] Payload: ");
    Serial.println(message);
    Serial.println("------------------------------------------");

    // Kiem tra neu la topic debug ping tu Node-RED
    if (strcmp(topic, TOPIC_DEBUG_PING) == 0) {
        Serial.print(">>> [PING NHAN DUOC TU NODE-RED]: ");
        Serial.println(message);
    }

    // Chuyen tiep toi cac module tuong ung xu ly
#ifdef BUILD_KIET
    kietHandleMqttMessage(topic, message);
#endif
#ifdef BUILD_QUY
    quyHandleMqttMessage(topic, message);
#endif
#ifdef BUILD_PHAT
    phatHandleMqttMessage(topic, message);
#endif
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("==================================================");
    Serial.println(" SMART HOME IoT - NHOM 05 (PHYSICS FOR IT)");
    Serial.print  (" Moi truong Firmware hien tai: ");
    Serial.println(CURRENT_ENV);
    Serial.println("==================================================");

    // 1. Khoi tao ket noi mang WiFi va MQTT
    wifiMqttSetup(onMqttMessageReceived);

    // 2. Dang ky lang nghe topic ping tu Node-RED de kiem thu Hello World
    mqttSubscribe(TOPIC_DEBUG_PING, 1);

    // 3. Khoi tao cac module chuc nang theo build flag
#ifdef BUILD_KIET
    kietSetup();
#endif
#ifdef BUILD_QUY
    quySetup();
#endif
#ifdef BUILD_PHAT
    phatSetup();
#endif

    Serial.println("[SYSTEM] He thong da khoi dong xong va san sang hoat dong.");
}

void loop() {
    // 1. Duy tri ket noi mang va xu ly background task MQTT
    wifiMqttLoop();

    // 2. Chay loop cua cac module chuc nang
#ifdef BUILD_KIET
    kietLoop();
#endif
#ifdef BUILD_QUY
    quyLoop();
#endif
#ifdef BUILD_PHAT
    phatLoop();
#endif

    // 3. Hello World: Dinh ky 5 giay publish JSON bao song len TOPIC_DEBUG_HELLO
    unsigned long now = millis();
    if (now - lastHelloPublish >= 5000) {
        lastHelloPublish = now;

        if (isMqttConnected()) {
            JsonDocument doc;
            doc["hello"] = CURRENT_ENV;
            doc["uptime"] = now;

            char jsonBuffer[128];
            serializeJson(doc, jsonBuffer);

            mqttPublish(TOPIC_DEBUG_HELLO, jsonBuffer);
            Serial.print("[DEBUG OUT] Da publish toi ");
            Serial.print(TOPIC_DEBUG_HELLO);
            Serial.print(" -> ");
            Serial.println(jsonBuffer);
        }
    }
}
