#pragma once

// ==========================================
// HẰNG SỐ CHÂN PHẦN CỨNG (GPIO PIN MAPPING)
// Tham chiếu: docs/pin-map.md
// ==========================================
#define PIN_DHT22         4
#define PIN_RELAY         26
#define PIN_MQ2_GAS       34   // ADC1_CH6 (Input-only)
#define PIN_LCD_SDA       21
#define PIN_LCD_SCL       22
#define PIN_PIR           27
#define PIN_SERVO         13
#define PIN_RFID_SS       5
#define PIN_RFID_SCK      18
#define PIN_RFID_MOSI     23
#define PIN_RFID_MISO     19
#define PIN_RFID_RST      16   // GPIO 16 (tranh xung dot GPIO 22 voi I2C SCL)

// ==========================================
// ĐỊNH NGHĨA MQTT TOPICS (HỢP ĐỒNG DỮ LIỆU)
// Tham chiếu: docs/mqtt-topics.md
// ==========================================
#define TOPIC_SENSOR_DHT22     "nhom05/home/sensor/dht22"
#define TOPIC_SENSOR_GAS       "nhom05/home/sensor/gas"
#define TOPIC_SENSOR_PIR       "nhom05/home/sensor/pir"

#define TOPIC_LIGHT_CMD        "nhom05/home/light/cmd"
#define TOPIC_LIGHT_STATE      "nhom05/home/light/state"

#define TOPIC_LCD_MSG          "nhom05/home/lcd/msg"
#define TOPIC_RFID_EVENT       "nhom05/home/rfid/event"

#define TOPIC_SYSTEM_STATUS    "nhom05/home/status"
#define TOPIC_DEBUG_HELLO      "nhom05/home/debug/hello"
#define TOPIC_DEBUG_PING       "nhom05/home/debug/ping"

// Thông điệp trạng thái hệ thống LWT
#define LWT_PAYLOAD_ONLINE     "online"
#define LWT_PAYLOAD_OFFLINE    "offline"
