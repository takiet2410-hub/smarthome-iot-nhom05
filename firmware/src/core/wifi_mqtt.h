#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

// Định nghĩa kiểu hàm callback khi nhận được message MQTT
typedef void (*MQTTMessageCallback)(char* topic, byte* payload, unsigned int length);

/**
 * Khởi tạo kết nối WiFi và MQTT Client
 * @param callback Hàm xử lý khi nhận tin nhắn MQTT (nếu có)
 */
void wifiMqttSetup(MQTTMessageCallback callback = nullptr);

/**
 * Hàm duy trì kết nối WiFi và MQTT (gọi liên tục trong loop chính)
 */
void wifiMqttLoop();

/**
 * Gửi tin nhắn lên MQTT Topic
 * @param topic Tên topic
 * @param payload Nội dung tin nhắn
 * @param retained Có lưu lại tin nhắn (retained message) hay không
 * @return true nếu gửi thành công
 */
bool mqttPublish(const char* topic, const char* payload, bool retained = false);

/**
 * Đăng ký nhận tin nhắn từ MQTT Topic
 * @param topic Tên topic cần subscribe
 * @param qos Mức chất lượng dịch vụ (0 hoặc 1)
 * @return true nếu đăng ký thành công
 */
bool mqttSubscribe(const char* topic, uint8_t qos = 0);

/**
 * Kiểm tra trạng thái kết nối MQTT
 */
bool isMqttConnected();
