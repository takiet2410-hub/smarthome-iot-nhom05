# BÁO CÁO KIỂM TRA LINH KIỆN TRÊN TRÌNH GIẢ LẬP WOKWI

> **Tài liệu tham khảo chính thức:** [Wokwi Parts Documentation](https://docs.wokwi.com/parts)  
> **Ngày rà soát:** 16/11/2026

---

## 1. Kết quả kiểm tra chi tiết từng linh kiện

Theo thuyết minh dự án (Proposal), nhóm dự kiến sử dụng 9 linh kiện chính. Dưới đây là kết quả kiểm tra tính khả dụng thực tế trong Wokwi:

| STT | Linh kiện (Proposal) | Có trong Wokwi? | Tên Part chính thức | Tính năng mô phỏng & Ghi chú |
|:---:|---|:---:|---|---|
| 1 | **ESP32 DevKit V1** | **CÓ** | `board-esp32-devkit-c-v4` | Hỗ trợ đầy đủ WiFi ảo (`Wokwi-GUEST`), GPIO, ADC, I2C, SPI, UART Serial. |
| 2 | **Cảm biến DHT22** | **CÓ** | `wokwi-dht22` | Giao tiếp 1-Wire. Có thanh trượt (slider) tương tác trực tiếp nhiệt độ (-40°C đến 80°C) và độ ẩm (0-100%). |
| 3 | **Cảm biến khí gas MQ-2** | **CÓ** | `wokwi-gas-sensor` | Hỗ trợ chân ngõ ra Analog `AOUT` và Digital `DOUT`. Có thanh trượt điều chỉnh nồng độ gas `ppm` (0 - 10,000 ppm). |
| 4 | **Cảm biến chuyển động PIR HC-SR501** | **CÓ** | `wokwi-pir-motion-sensor` | Giao tiếp Digital Input. Trong mô phỏng có nút **"Simulate Motion"** để kích hoạt phát hiện chuyển động. |
| 5 | **Module RFID MFRC522** | **CÓ** | `board-mfrc522` | Giao tiếp bus SPI. Wokwi cung cấp bảng điều khiển chọn thẻ mẫu (Blue Card, Green Card, Key Fob...), sửa UID và nút quẹt thẻ (Tap / Hold). |
| 6 | **Động cơ Servo SG90** | **CÓ** | `wokwi-servo` | Điều khiển xung PWM từ 0° đến 180°. Hiển thị trực quan góc quay trục servo trong trình giả lập. |
| 7 | **Module Relay 1 kênh 5V** | **CÓ** | `wokwi-relay-module` | Hỗ trợ chân `IN`, `NO`, `NC`, `COM`. Có đèn LED báo trạng thái đóng/ngắt tiếp điểm. |
| 8 | **Đèn LED 5V / Module LED** | **CÓ** | `wokwi-led` | Mô phỏng đèn thắp sáng, chọn màu sắc linh hoạt (Red, Green, Blue...). |
| 9 | **Màn hình LCD 2004 I2C** | **CÓ** | `wokwi-lcd2004` | 4 dòng × 20 ký tự. Hỗ trợ chế độ module chuyển đổi I2C backpack qua thuộc tính `{"pins": "i2c"}` (chân GND, VCC, SDA, SCL). |

---

## 2. Kết luận và Đánh giá tuân thủ quy định đồ án

### 2.1. Đánh giá tính đầy đủ:
- **TẤT CẢ 9/9 LINH KIỆN** đề xuất trong bản Proposal đều có linh kiện mô phỏng chính thức trong thư viện của Wokwi.
- Đặc biệt, hai linh kiện phức tạp thường bị lo ngại thiếu trên các trình giả lập là **Module RFID MFRC522** (`board-mfrc522`) và **Cảm biến khí Gas MQ-2** (`wokwi-gas-sensor`) đều **hoạt động hoàn hảo và có giao diện tương tác trực tiếp** trên Wokwi.

### 2.2. Tuân thủ quy định đồ án:
- Theo mục C.2 của file **QUY ĐỊNH ĐỒ ÁN CUỐI KỲ**:  
  > *"Đối với nhóm thực hiện giả lập, chỉ được dùng tối đa MỘT thiết bị khác thay thế cho thiết bị không có trong giả lập. Thiết bị này cũng phải khác so với các thiết bị đã dùng."*
- **Kết luận:** Nhóm 05 **KHÔNG CẦN THAY THẾ BẤT KỲ THIẾT BỊ NÀO** (Số lượng thiết bị thay thế = 0 / tối đa 1). Toàn bộ phần cứng được giữ nguyên 100% đúng như bản thuyết minh dự án đã nộp.

### 2.3. Hướng dẫn thiết lập các linh kiện đặc thù trong `diagram.json`:

1. **LCD 2004 I2C:**
   Khai báo trong `diagram.json` với thuộc tính `"pins": "i2c"`:
   ```json
   { "type": "wokwi-lcd2004", "id": "lcd", "top": 0, "left": 0, "attrs": { "pins": "i2c" } }
   ```
2. **RFID MFRC522:**
   Khai báo trong `diagram.json`:
   ```json
   { "type": "board-mfrc522", "id": "rfid", "top": 0, "left": 0, "attrs": {} }
   ```
   Kết nối các chân: `SS` -> GPIO 5, `SCK` -> GPIO 18, `MOSI` -> GPIO 23, `MISO` -> GPIO 19, `RST` -> GPIO 16 (tránh GPIO 22 của I2C SCL).
3. **Cảm biến Gas MQ-2:**
   Khai báo trong `diagram.json`:
   ```json
   { "type": "wokwi-gas-sensor", "id": "gas", "top": 0, "left": 0, "attrs": {} }
   ```
   Nối chân `AOUT` vào GPIO 34 của ESP32.
