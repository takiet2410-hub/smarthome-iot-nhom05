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

}

void loop() {

}
