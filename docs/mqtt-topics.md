# HỢP ĐỒNG ĐẶT TÊN VÀ ĐỊNH DẠNG MQTT TOPIC

Tài liệu này là **hợp đồng kỹ thuật bắt buộc** giữa tầng Firmware (ESP32) và Backend/Frontend (Node-RED). Mọi thành viên phải tuân thủ nghiêm ngặt để đảm bảo khả năng tích hợp trơn tru.

---

## 1. Thông số Broker chung

- **Broker:** HiveMQ Cloud (Serverless Cluster)
- **Port:** `8883` (Bảo mật TLS/SSL bắt buộc)
- **Tiền tố bắt buộc cho mọi Topic:** `nhom05/` (nhằm tránh xung đột trên MQTT broker)
- **Chính sách QoS:**
  - `QoS 0`: Dữ liệu đo đạc cảm biến chu kỳ (DHT22, Gas, Uptime).
  - `QoS 1`: Lệnh điều khiển và sự kiện quan trọng (Light cmd, LWT status).

---

## 2. Bảng đặc tả Topic và Payload

| STT | Topic | Hướng | Định dạng Payload | Chu kỳ / Điều kiện | ID Chức năng | Phụ trách |
|:---:|---|:---:|---|---|:---:|:---:|
| 1 | `nhom05/home/sensor/dht22` | ESP → NR | `{"temp": 29.5, "humi": 70.2}` | Chu kỳ 5 giây | ID 1 | Quý |
| 2 | `nhom05/home/sensor/gas` | ESP → NR | `{"gas": 35}` *(tỷ lệ 0–100%)* | Chu kỳ 5 giây (Cảnh báo > 70%) | ID 3 | Kiệt |
| 3 | `nhom05/home/sensor/pir` | ESP → NR | `{"motion": true, "timestamp": "<ISO8601>"}` | Kích hoạt khi có chuyển động | ID 7 | Phát |
| 4 | `nhom05/home/light/cmd` | NR → ESP | `ON` hoặc `OFF` (chuỗi thô) | Khi người dùng nhấn nút trên web (QoS 1) | ID 2 | Phát |
| 5 | `nhom05/home/light/state` | ESP → NR | `ON` hoặc `OFF` (chuỗi thô, Retained) | ESP32 phản hồi trạng thái thực tế | ID 2 | Phát |
| 6 | `nhom05/home/lcd/msg` | NR → ESP | Chuỗi ASCII / UTF-8 không dấu (tối đa 80 ký tự) | Khi gửi thông điệp từ web lên LCD 2004 | ID 4 | Quý |
| 7 | `nhom05/home/rfid/event` | ESP → NR | `{"uid": "AB12CD34", "valid": true}` *(tùy chọn)* | Mỗi khi quẹt thẻ qua MFRC522 | ID 11 | Kiệt |
| 8 | `nhom05/home/status` | ESP → NR | `online` hoặc `offline` (LWT, Retained) | Báo trạng thái sống của ESP32 | Hệ thống | Nhóm trưởng |
| 9 | `nhom05/home/debug/hello` | ESP → NR | `{"hello": "<env>", "uptime": 12345}` | Chu kỳ 5 giây (Kiểm tra Hello World) | Debug | Chung |
| 10 | `nhom05/home/debug/ping` | NR → ESP | Chuỗi văn bản tùy ý | Khi nhấn inject nút Ping trên Node-RED | Debug | Chung |

---

## 3. Quy tắc chuẩn hóa dữ liệu

1. **Khóa JSON:** Luôn dùng chữ cái thường (`camelCase` hoặc `snake_case`, thống nhất trong bảng: `temp`, `humi`, `gas`, `motion`, `timestamp`, `uid`, `valid`).
2. **Kiểu số:** Dữ liệu đo đạc số học (`temp`, `humi`, `gas`, `uptime`) gửi dưới dạng **Number**, tuyệt đối không đóng dấu ngoặc kép thành String (ví dụ: `29.5`, không gửi `"29.5"`).
3. **Màn hình LCD 2004:** Thư viện LCD 2004 I2C thông thường không hỗ trợ ký tự Unicode tiếng Việt có dấu. Do đó, tầng Node-RED trước khi publish lên `nhom05/home/lcd/msg` phải qua một function node loại bỏ dấu tiếng Việt (chuyển sang ký tự Latin tiêu chuẩn).
4. **Quy tắc thay đổi:** Nếu muốn bổ sung hoặc chỉnh sửa topic, phải tạo PR sửa tài liệu này trước và có sự đồng thuận của nhóm trưởng.
