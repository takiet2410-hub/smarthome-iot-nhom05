#include "module_quy.h"
#include <Arduino.h>
#include "../../core/wifi_mqtt.h"
#include "config.h"

void quySetup() {
    Serial.println("[MODULE QUY] Khoi tao module (ID 1: DHT22, ID 4: LCD 2004 I2C)");
    // TODO: Khoi tao cam bien DHT22 tren PIN_DHT22 (GPIO 4)
    // TODO: Khoi tao man hinh LCD 2004 qua I2C PIN_LCD_SDA (GPIO 21), PIN_LCD_SCL (GPIO 22)
}

void quyLoop() {
    // TODO: Thuc hien doc nhiet do / do am DHT22 dinh ky 5 giay va publish MQTT
}

void quyHandleMqttMessage(const char* topic, const char* payload) {
    // TODO: Nhan thong diep tu TOPIC_LCD_MSG de hien thi len LCD 2004
}
