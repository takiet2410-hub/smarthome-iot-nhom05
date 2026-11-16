# SƠ ĐỒ PHÂN BỔ CHÂN GPIO ESP32 (PIN MAP)

Tài liệu này quy định việc phân bổ chân GPIO của board vi điều khiển ESP32 DevKit V1 cho các linh kiện trong dự án Nhà thông minh Nhóm 05.

---

## 1. Bảng phân bổ chân GPIO

| Linh kiện | Chức năng tín hiệu | Chân ESP32 | Loại chân | ID | Thành viên phụ trách | Ghi chú |
|---|---|:---:|:---:|:---:|:---:|---|
| **DHT22** | DATA (1-Wire) | **GPIO 4** | Digital I/O | 1 | Quý | Cần trở kéo lên (pull-up) 10kΩ nếu module rời |
| **Relay 1 kênh** | IN (Điều khiển) | **GPIO 26** | Digital Output | 2 | Phát | Mức kích thích (High/Low) theo cấu hình |
| **MQ-2 Gas** | AOUT (Analog) | **GPIO 34** | ADC1_CH6 (Input only) | 3 | Kiệt | Chân Input-only, không có pull-up/down nội |
| **LCD 2004 I2C** | SDA | **GPIO 21** | I2C Data | 4 | Quý | Bus I2C mặc định của ESP32 |
| **LCD 2004 I2C** | SCL | **GPIO 22** | I2C Clock | 4 | Quý | Bus I2C mặc định của ESP32 |
| **PIR HC-SR501** | OUT (Chuyển động) | **GPIO 27** | Digital Input | 7 | Phát | Nhận tín hiệu logic khi có người |
| **Servo SG90** | PWM (Góc quay) | **GPIO 13** | PWM Output | 11 | Kiệt | Tần số 50Hz, điều khiển góc 0° - 90° |
| **RFID MFRC522** | SPI SS (SDA) | **GPIO 5** | SPI Chip Select | 11 | Kiệt | Chuẩn bus VSPI |
| **RFID MFRC522** | SPI SCK | **GPIO 18** | SPI Clock | 11 | Kiệt | Chuẩn bus VSPI |
| **RFID MFRC522** | SPI MOSI | **GPIO 23** | SPI Master Out | 11 | Kiệt | Chuẩn bus VSPI |
| **RFID MFRC522** | SPI MISO | **GPIO 19** | SPI Master In | 11 | Kiệt | Chuẩn bus VSPI |
| **RFID MFRC522** | RST (Reset) | **GPIO 16** | Digital Output | 11 | Kiệt | **Khuyến nghị: GPIO 16 (Xem cảnh báo bên dưới)** |

---

## 2. CẢNH BÁO QUAN TRỌNG: Xung đột chân GPIO giữa LCD và RFID

> [!WARNING]
> Trong bản thuyết minh dự án (Proposal ban đầu):
> - Màn hình LCD 2004 I2C sử dụng **GPIO 21 (SDA)** và **GPIO 22 (SCL)**.
> - Module RFID MFRC522 nếu theo sơ đồ nối dây thông dụng trên mạng hay bị gán nhầm chân **RST vào GPIO 22**.
> - **Hậu quả:** Nếu gán RST của MFRC522 vào GPIO 22 sẽ gây xung đột trực tiếp với xung Clock của bus I2C (SCL), dẫn đến việc màn hình LCD bị treo hoặc module RFID không khởi tạo được.
>
> **Giải pháp đã thống nhất:**
> Chuyển chân **RST của RFID MFRC522 sang GPIO 16** (hoặc GPIO 17). Board ESP32 DevKit có chân GPIO 16 hoàn toàn trống, không xung đột bus SPI hay I2C.

---

## 3. Lưu ý về nguồn điện và mức logic

1. **Điện áp logic 3.3V:**
   - ESP32 hoạt động ở mức logic 3.3V. Chân ADC chỉ nhận tối đa 3.3V.
   - Cảm biến MQ-2 hoạt động ở mức 5V: Khi làm phần cứng thật cần mạch cầu phân áp (1kΩ - 2kΩ) để đưa điện áp ra về an toàn dưới 3.3V. Trong Wokwi giả lập, `wokwi-gas-sensor` có thể nối trực tiếp vào ADC pin 34.
2. **Nguồn 5V riêng cho tải:**
   - Động cơ Servo và Relay khi đóng cắt tiêu thụ dòng đỉnh lớn. Cần cấp nguồn 5V ngoài đủ dòng để tránh sụt áp gây reset ESP32 (Brownout detector).
