#include "module_kiet.h"
#include <Arduino.h>
#include "../../core/wifi_mqtt.h"
#include "config.h"

void kietSetup() {
    Serial.println("[MODULE KIET] Khoi tao module (ID 3: MQ-2 Gas, ID 11: RFID MFRC522 & Servo)");
    // TODO: Khoi tao cam bien MQ-2 tren PIN_MQ2_GAS (GPIO 34)
    // TODO: Khoi tao RFID tren bus SPI va Servo tren PIN_SERVO (GPIO 13)
}

void kietLoop() {
    // TODO: Thuc hien doc ADC dinh ky va quet the RFID cuc bo
}

void kietHandleMqttMessage(const char* topic, const char* payload) {
    // Xu ly message danh rieng cho module cua Kiet neu co
}
