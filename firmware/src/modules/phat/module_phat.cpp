#include "module_phat.h"
#include <Arduino.h>
#include "../../core/wifi_mqtt.h"
#include "config.h"

void phatSetup() {
    Serial.println("[MODULE PHAT] Khoi tao module (ID 2: Relay den, ID 7: Cam bien PIR)");
    // TODO: Khoi tao Relay tren PIN_RELAY (GPIO 26)
    // TODO: Khoi tao PIR tren PIN_PIR (GPIO 27)
}

void phatLoop() {
    // TODO: Theo doi tin hieu chuyen dong PIR
}

void phatHandleMqttMessage(const char* topic, const char* payload) {
    // TODO: Nhan lenh ON / OFF tu TOPIC_LIGHT_CMD de dieu khien Relay
}
