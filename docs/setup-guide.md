# HƯỚNG DẪN THIẾT LẬP MÔI TRƯỜNG PHÁT TRIỂN (SETUP GUIDE)

Tài liệu này hướng dẫn chi tiết từng bước (Onboarding) cho các thành viên trong Nhóm 05 để cài đặt và chạy thử nghiệm hệ thống Smart Home IoT từ máy trắng.

---

## 1. Bản chất kết nối giữa Wokwi, Node-RED và GitHub

Nhiều bạn thường thắc mắc: *"Wokwi chạy thế nào, Node-RED nằm ở đâu, và GitHub đóng vai trò gì?"*

```
  Wokwi (VS Code)                                Node-RED (Local)
 [Mô phỏng ESP32]                             [Web Server & Backend]
        │                                                │
        │ WiFi ảo "Wokwi-GUEST"                          │ Chạy cổng 1880
        ▼                                                ▼
  ┌─────────────────────────────────────────────────────────────┐
  │         HiveMQ Cloud MQTT Broker (Internet TLS 8883)        │
  └─────────────────────────────────────────────────────────────┘
                                ▲
                                │
                    [Giao thức trao đổi dữ liệu]
```

- **GitHub:** Là kho lưu trữ mã nguồn chung duy nhất. Mọi người tải code về máy mình qua `git clone`, làm việc theo nhánh và hợp nhất qua Pull Request (PR). GitHub **không** kết nối trực tiếp với Wokwi hay Node-RED.
- **Wokwi:** Là extension chạy trên VS Code, giả lập board ESP32 và các linh kiện phần cứng (DHT22, Gas MQ-2, LCD 2004, PIR, RFID, Relay, Servo). ESP32 ảo này kết nối ra ngoài Internet thông qua mạng WiFi ảo `Wokwi-GUEST` tới **HiveMQ Cloud**.
- **Node-RED:** Là ứng dụng Node.js chạy trực tiếp trên máy của bạn (`http://localhost:1880`), vừa đóng vai trò xử lý logic Backend, vừa tạo giao diện người dùng Web Dashboard 2.0. Node-RED cũng kết nối qua Internet tới **HiveMQ Cloud**.
- **Cầu nối:** ESP32 và Node-RED **nói chuyện với nhau thông qua HiveMQ Cloud** qua các MQTT Topic đã được thống nhất tại [docs/mqtt-topics.md](docs/mqtt-topics.md).

---

## 2. Yêu cầu công cụ phần mềm (Cài đặt trên máy cá nhân)

1. **Git:** [Tải Git cho Windows/macOS](https://git-scm.com/).
2. **Node.js (LTS version 20 hoặc 22):** [Tải Node.js](https://nodejs.org/).
3. **VS Code:** [Tải VS Code](https://code.visualstudio.com/).
4. **Các Extension VS Code bắt buộc:**
   - **PlatformIO IDE** (dùng để biên dịch và nạp firmware cho ESP32).
   - **Wokwi Simulator** (dùng để chạy mô phỏng mạch điện).

---

## 3. Các bước thiết lập ban đầu

### Bước 1: Kích hoạt License Wokwi miễn phí
1. Mở VS Code.
2. Nhấn tổ hợp phím `Ctrl + Shift + P` (hoặc `Cmd + Shift + P` trên macOS) để mở Command Palette.
3. Gõ tìm kiếm: `Wokwi: Request a new License`.
4. Trình duyệt sẽ mở ra trang web Wokwi, đăng nhập bằng GitHub hoặc Google để nhận License Key. VS Code sẽ tự động lưu key này.

### Bước 2: Clone repository về máy
```bash
git clone <URL_REPO_GITHUB>
cd smart-home-iot-nhom05
```

### Bước 3: Cấu hình biến môi trường và mật khẩu cục bộ
1. **Đối với Node-RED:**
   - Copy file `.env.example` thành `.env` tại thư mục gốc của dự án:
     ```bash
     cp .env.example .env
     ```
   - Nhập thông tin đăng nhập **HiveMQ Cloud** do Nhóm trưởng cung cấp (`MQTT_HOST`, `MQTT_USER`, `MQTT_PASS`).
   - *(Lưu ý: File `.env` đã được gitignore, tuyệt đối không commit file này).*

2. **Đối với Firmware ESP32:**
   - Copy file `firmware/include/secrets.example.h` thành `firmware/include/secrets.h`:
     ```bash
     cp firmware/include/secrets.example.h firmware/include/secrets.h
     ```
   - Điền thông tin `MQTT_HOST`, `MQTT_USER`, `MQTT_PASS` tương tự như trên.
   - Giữ nguyên `WIFI_SSID` là `"Wokwi-GUEST"` và `WIFI_CHANNEL` là `6`.

---

## 4. Cách chạy và kiểm thử

### 4.1. Khởi chạy Node-RED Dashboard
Từ thư mục gốc dự án:
- Trên Windows PowerShell:
  ```powershell
  ./scripts/start-node-red.ps1
  ```
- Hoặc chạy bằng lệnh npm:
  ```bash
  cd node-red
  npm install
  npx node-red --userDir . --settings settings.js
  ```
- Mở trình duyệt truy cập:
  - Giao diện chỉnh sửa Flow: `http://localhost:1880`
  - Giao diện Dashboard hiển thị: `http://localhost:1880/dashboard`

### 4.2. Khởi chạy mô phỏng Wokwi trên VS Code
1. Mở project trong VS Code (chọn thư mục gốc của repo).
2. Biên dịch firmware bằng PlatformIO:
   - Mở thanh bên PlatformIO (biểu tượng con kiến) → Chọn env tương ứng của bạn (ví dụ: `env:quy` hoặc `env:integrated`) → Bấm **Build**.
3. Mở file cấu hình Wokwi tương ứng:
   - Mở file `firmware/wokwi/<tên_bạn>/diagram.json`.
   - Nhấn phím `F1` → Gõ `Wokwi: Start Simulator` (hoặc bấm nút Play màu xanh trên thanh trạng thái).
4. Quan sát cửa sổ Serial Monitor:
   - ESP32 sẽ kết nối tới WiFi `Wokwi-GUEST`.
   - ESP32 kết nối tới HiveMQ Cloud qua TLS.
   - Thấy dòng chữ `[MQTT] Connected successfully!` là kết nối thành công.
