# Hệ Thống Nhà Thông Minh IoT (Nhóm 05)

Dự án môn học **Vật lý cho Công nghệ thông tin** (Lớp 24C10 – Học kỳ 1 / 2026–2027, ĐH Khoa học Tự nhiên – ĐHQG-HCM).  
Hệ thống tích hợp mô phỏng phần cứng vi điều khiển ESP32 qua Wokwi, kết nối giao thức bảo mật MQTT (HiveMQ Cloud TLS) với Dashboard quản lý trên Node-RED 2.0, kết hợp cơ sở dữ liệu Supabase, thông báo Telegram/Email, trợ lý ảo Gemini và mô hình học máy (Flask) dự báo mưa.

---

## 1. Kiến trúc kết nối hệ thống

```
  Wokwi (ESP32 ảo, chạy qua VS Code)           Node-RED (chạy local máy cá nhân)
         │  WiFi ảo "Wokwi-GUEST"                          │  localhost:1880
         ▼                                                 ▼
               ┌───────────────────────────────┐
               │  HiveMQ Cloud (MQTT Broker)   │   ← Cầu nối DUY NHẤT giữa hai bên
               │  <host>:8883  (TLS)           │
               └───────────────────────────────┘

  GitHub: Monorepo chứa toàn bộ mã nguồn (Firmware PlatformIO, flows Node-RED, AI Service, Docs).
```

- **ESP32 (Wokwi):** Chạy firmware PlatformIO, kết nối internet qua WiFi ảo `Wokwi-GUEST`, trao đổi dữ liệu với HiveMQ Cloud.
- **Node-RED:** Đóng vai trò backend điều phối và frontend giao diện Dashboard 2.0, kết nối tới HiveMQ Cloud qua TLS port 8883.
- **Dịch vụ bổ trợ:** Supabase (Database & Auth), Telegram Bot API, SMTP Gmail, Google Gemini API, Flask Microservice.

---

## 2. Thành viên nhóm

| STT | Họ và tên | MSSV | Vai trò | GitHub Username |
|:---:|---|:---:|---|---|
| 1 | **Trần Anh Kiệt** | 24127287 | Nhóm trưởng | `@kiet-tran` <!-- Cập nhật username thật --> |
| 2 | **Trần Đức Quý** | 24127526 | Thành viên | `@quy-tran` <!-- Cập nhật username thật --> |
| 3 | **Nguyễn Tiến Phát** | 24127098 | Thành viên | `@phat-nguyen` <!-- Cập nhật username thật --> |

---

## 3. Phân công chức năng đồ án

| ID | Chức năng chi tiết | Phân loại | Thành viên phụ trách |
|:---:|---|:---:|:---:|
| **1** | Cảm biến DHT22 (nhiệt độ, độ ẩm) → MQTT → Web Dashboard | Căn bản (Luồng 1) | Quý |
| **2** | Web Dashboard → MQTT → Điều khiển Relay + Đèn LED | Căn bản (Luồng 2) | Phát |
| **3** | Cảm biến khí gas MQ-2 → MQTT → Web Dashboard cảnh báo | Căn bản (Luồng 1) | Kiệt |
| **4** | Web Dashboard → Gửi lời nhắn hiển thị lên LCD 2004 I2C | Nâng cao #3 | Quý |
| **5** | Lưu trữ lịch sử dữ liệu cảm biến DHT22 lên Supabase (Cloud) | Nâng cao #4 | Quý |
| **6** | Lấy dữ liệu Supabase vẽ biểu đồ đường (Line Chart) trên Web | Nâng cao #5 | Quý |
| **7** | Cảm biến chuyển động PIR → Web Dashboard ghi nhận nhật ký | Nâng cao #2 | Phát |
| **8** | Cảnh báo nồng độ khí gas / an ninh qua Telegram Bot API | Nâng cao #6 | Phát |
| **9** | Gửi email báo cáo định kỳ buổi sáng (SMTP / Nodemailer) | Nâng cao #7 | Phát |
| **10** | Hệ thống Đăng ký / Đăng nhập tài khoản bảo mật (Supabase Auth) | Nâng cao #9 | Phát |
| **11** | Quẹt thẻ RFID RC522 mở khóa cửa Động cơ Servo (xử lý cục bộ ESP32) | Nâng cao #1 | Kiệt |
| **12** | Chatbot hỏi đáp thông minh dựa trên ngữ cảnh hệ thống (Gemini API) | Nâng cao #8 | Kiệt |
| **13** | Mô hình học máy tự xây dựng (Flask) dự báo mưa 4 khung giờ | Nâng cao #11 | Kiệt |

---

## 4. Tài liệu quan trọng

- [Hướng dẫn cài đặt môi trường (Setup Guide)](docs/setup-guide.md)
- [Quy chuẩn MQTT Topics & Payload (Hợp đồng dữ liệu)](docs/mqtt-topics.md)
- [Sơ đồ chân GPIO ESP32 (Pin Map)](docs/pin-map.md)
- [Kết quả kiểm tra linh kiện trên Wokwi](docs/wokwi-parts-check.md)
- [Quy định đóng góp & quy trình Git](CONTRIBUTING.md)
- [Thuyết minh dự án (Proposal PDF)](docs/00-proposal.pdf)
- [Quy định đồ án cuối kỳ môn học](docs/01-quy-dinh-do-an.pdf)

---

## 5. Cấu trúc thư mục

```
smart-home-iot-nhom05/
├── .github/              # CI/CD Workflows, PR Template, CODEOWNERS
├── ai-service/           # Dịch vụ AI Flask dự báo thời tiết mưa
├── docs/                 # Tài liệu hệ thống, sơ đồ chân, topics MQTT, đề tài
├── firmware/             # Mã nguồn ESP32 (PlatformIO) & Cấu hình Wokwi
│   ├── include/          # File cấu hình pin, topic, secrets
│   ├── src/              # Core wifi_mqtt & các modules chức năng
│   └── wokwi/            # Cấu hình Wokwi cho từng thành viên & bản tích hợp
├── node-red/             # Toàn bộ cấu hình flows & Dashboard Node-RED
└── scripts/              # Scripts khởi chạy và kiểm tra chất lượng
```
