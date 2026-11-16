# KẾ HOẠCH CHO CODING AGENT: DỰNG MÔI TRƯỜNG LÀM VIỆC NHÓM (GitHub + Wokwi + Node-RED)

> **Đối tượng đọc:** Coding Agent (Claude Code / Cursor / Copilot Agent...).
> **Mục tiêu của tài liệu này:** chỉ dựng **BASE môi trường** để 3 thành viên làm chung trên GitHub. **KHÔNG** cài đặt các chức năng của đồ án (DHT22, RFID, chatbot, AI...) ở giai đoạn này, ngoại trừ một bài "Hello World" để kiểm chứng đường truyền.

---

## 0. Bối cảnh dự án (đọc kỹ trước khi làm)

**Môn học:** Vật lý cho Công nghệ thông tin, lớp 24C10, HK1/2026-2027, ĐH Khoa học Tự nhiên – ĐHQG-HCM.
**Đề tài (Nhóm 05):** Hệ thống nhà thông minh IoT – giám sát môi trường, an ninh và điều khiển từ xa.

**Thành viên:**

| Tên | MSSV | Vai trò | GitHub username |
|---|---|---|---|
| Trần Anh Kiệt | 24127287 | Nhóm trưởng | `<điền>` |
| Trần Đức Quý | 24127526 | Thành viên | `<điền>` |
| Nguyễn Tiến Phát | 24127098 | Thành viên | `<điền>` |

### 0.1. Quyết định kỹ thuật đã chốt (KHÔNG tự ý thay đổi)

1. Nhóm làm **trên giả lập**, KHÔNG làm sản phẩm thật. Giả lập ESP32 bằng **Wokwi**.
2. Website quản lý xây bằng **Node-RED** + **Node-RED Dashboard 2.0** (`@flowfuse/node-red-dashboard`). Không tự code web riêng (quy định: yêu cầu "tự xây dựng website, không dùng Node-RED" không nằm trong phạm vi nhóm).
3. Giao thức truyền: **MQTT** qua **HiveMQ Cloud** (cluster miễn phí, TLS port 8883).
4. Cloud database + Auth: **Supabase**. Thông báo: **Telegram Bot API**, **Email (SMTP/Nodemailer)**. Chatbot: **Google Gemini API**. AI dự báo mưa: **Python Flask** (microservice riêng).
5. Quản lý mã nguồn: **GitHub**, một repo duy nhất (monorepo).

### 0.2. Kiến trúc kết nối (QUAN TRỌNG – quyết định toàn bộ thiết kế repo)

```
 Wokwi (ESP32 ảo, chạy local qua VS Code)         Node-RED (chạy trên máy mỗi người)
        │  WiFi ảo "Wokwi-GUEST"                          │  localhost:1880
        ▼                                                 ▼
              ┌───────────────────────────────┐
              │  HiveMQ Cloud (MQTT Broker)   │   ← cầu nối DUY NHẤT giữa hai bên
              │  <host>:8883  (TLS)           │
              └───────────────────────────────┘

 GitHub: chỉ chứa CODE (firmware, diagram.json, flows.json, tài liệu). KHÔNG nối trực tiếp với Wokwi/Node-RED.
```

Hệ quả cho thiết kế:
- ESP32 trong Wokwi **không truy cập được `localhost`** → broker bắt buộc là broker trên Internet.
- `flows.json` của Node-RED là **một file JSON duy nhất** → nguy cơ merge conflict cao → cần quy ước (xem mục 6).
- Thông tin bí mật (mật khẩu MQTT, token Telegram, key Gemini/Supabase) **tuyệt đối không commit**.

### 0.2b. Các ràng buộc từ file quy định cần nhớ khi thiết kế

- Nhóm 2–3 SV. Mỗi SV phụ trách 1 luồng căn bản (Luồng 1: Input → ESP → MQTT → BE → FE; hoặc Luồng 2: FE → BE → MQTT → ESP → Output).
- **Thiết bị input/output ở các yêu cầu khác nhau phải đôi một khác nhau.**
- **Nhóm giả lập chỉ được thay thế TỐI ĐA MỘT thiết bị không có trong giả lập**, và thiết bị thay thế phải khác các thiết bị đã dùng. (→ Task A8 yêu cầu kiểm tra linh kiện Wokwi.)
- **+1 điểm làm việc nhóm** nếu mã nguồn của mọi thành viên được **tích hợp chung trong cùng một project** → thiết kế firmware dạng module + môi trường `integrated`.
- Vấn đáp: từng người phải giải thích được luồng mình phụ trách → mỗi luồng cần tài liệu ngắn trong `docs/`.

### 0.3. Phân công chức năng (từ proposal) – dùng để đặt tên thư mục/nhánh

| ID | Chức năng | Phụ trách |
|---|---|---|
| 1 | DHT22 → web (căn bản, Luồng 1) | Quý |
| 2 | Relay + đèn (căn bản, Luồng 2) | Phát |
| 3 | MQ-2 gas → web (căn bản, Luồng 1) | Kiệt |
| 4 | Web → LCD 2004 I2C | Quý |
| 5 | Lưu lịch sử DHT22 lên Supabase | Quý |
| 6 | Biểu đồ dữ liệu cloud | Quý |
| 7 | PIR → web | Phát |
| 8 | Telegram thông báo | Phát |
| 9 | Email thông báo | Phát |
| 10 | Đăng ký/Đăng nhập (Supabase Auth) | Phát |
| 11 | RFID + Servo (xử lý cục bộ trên ESP32) | Kiệt |
| 12 | Chatbot Gemini | Kiệt |
| 13 | AI dự báo mưa (Flask) | Kiệt |

---

## 1. Việc của CON NGƯỜI (Agent không làm được – hãy nhắc người dùng và chờ xác nhận)

Agent phải **dừng và hỏi người dùng** tại các điểm sau, không được tự bịa giá trị:

- [ ] **H1.** Tạo repo GitHub (hoặc cho phép agent dùng `gh` CLI nếu đã `gh auth login`). Tên đề xuất: `smart-home-iot-nhom05`, **Private**.
- [ ] **H2.** Cung cấp GitHub username của 3 thành viên để mời collaborator.
- [ ] **H3.** Nhóm trưởng tạo cluster **HiveMQ Cloud (Serverless, free)** → tạo 1 credential (username/password) dùng chung cho cả nhóm → gửi riêng cho thành viên (KHÔNG đưa vào repo/chat công khai).
- [ ] **H4.** Mỗi người lấy **Wokwi license key miễn phí** (VS Code → Command Palette → `Wokwi: Request a new License`).
- [ ] **H5.** Xác nhận quy ước nhánh/PR ở mục 5 (có bật branch protection hay không).

---

## 2. Cấu trúc repo mục tiêu

```
smart-home-iot-nhom05/
├── README.md
├── CONTRIBUTING.md
├── .gitignore
├── .gitattributes
├── .editorconfig
├── .env.example                    # Node-RED + service: tên biến, KHÔNG có giá trị thật
├── .github/
│   ├── pull_request_template.md
│   ├── CODEOWNERS
│   └── workflows/
│       └── ci.yml                  # build firmware + validate flows.json
├── docs/
│   ├── 00-proposal.pdf             # (người dùng tự copy vào)
│   ├── 01-quy-dinh-do-an.pdf       # (người dùng tự copy vào)
│   ├── mqtt-topics.md              # "HỢP ĐỒNG" topic/payload
│   ├── pin-map.md                  # bảng GPIO ↔ linh kiện
│   ├── wokwi-parts-check.md        # kết quả kiểm tra linh kiện có trong Wokwi
│   ├── setup-guide.md              # hướng dẫn onboarding cho thành viên mới
│   └── flows/                      # mỗi người tự viết giải thích luồng mình phụ trách (phục vụ vấn đáp)
├── firmware/                       # MỘT project PlatformIO duy nhất
│   ├── platformio.ini              # nhiều env: kiet, quy, phat, integrated
│   ├── include/
│   │   ├── config.h                # hằng số chung, tên topic, GPIO
│   │   ├── secrets.example.h       # mẫu (commit)
│   │   └── secrets.h               # thật (GITIGNORE)
│   ├── src/
│   │   ├── main.cpp                # chỉ ghép module; chọn module theo build flag
│   │   ├── core/                   # wifi_mqtt.h/.cpp  (dùng chung, chỉ nhóm trưởng sửa)
│   │   └── modules/
│   │       ├── kiet/               # mq2, rfid_servo (sau này)
│   │       ├── quy/                # dht22, lcd (sau này)
│   │       └── phat/               # relay, pir (sau này)
│   └── wokwi/
│       ├── kiet/{wokwi.toml,diagram.json}
│       ├── quy/{wokwi.toml,diagram.json}
│       ├── phat/{wokwi.toml,diagram.json}
│       └── integrated/{wokwi.toml,diagram.json}
├── node-red/                       # userDir của Node-RED
│   ├── package.json
│   ├── settings.js
│   └── flows.json                  # mỗi người 1 tab riêng
├── ai-service/                     # Flask + model (Kiệt) – tạo khung rỗng
│   ├── README.md
│   └── requirements.txt
└── scripts/
    ├── start-node-red.sh / .ps1    # chạy Node-RED với userDir + .env
    └── check-flows.js              # validate flows.json (JSON hợp lệ, id không trùng)
```

**Nguyên tắc cấu trúc firmware:** mỗi chức năng là một module (`*.h/.cpp`) với hàm `xxx_setup()` và `xxx_loop()`. `main.cpp` chỉ gọi các hàm này, bật/tắt bằng build flag. Mỗi thành viên có env riêng để phát triển độc lập; env `integrated` bật tất cả → đáp ứng tiêu chí "tích hợp chung một project".

---

## 3. Các giai đoạn thực hiện cho Agent

> Làm tuần tự. Sau mỗi giai đoạn: chạy phần **Kiểm tra**, rồi commit với message theo Conventional Commits (`chore:`, `docs:`, `feat:`).

### GIAI ĐOẠN A – Khung repo & tài liệu

- [ ] **A1.** Hỏi người dùng H1/H2. Nếu có `gh`: `gh repo create <tên> --private --clone`. Nếu chưa: hướng dẫn người dùng tạo tay rồi `git clone`.
- [ ] **A2.** Tạo toàn bộ cây thư mục ở mục 2 (file rỗng dùng `.gitkeep`).
- [ ] **A3.** Viết `.gitignore` (nội dung ở Phụ lục 1).
- [ ] **A4.** Viết `.gitattributes` (`* text=auto eol=lf`; giữ `*.pdf binary`) và `.editorconfig` (UTF-8, LF, indent 2 cho json/js, 4 cho cpp/py). **Lý do:** nhóm có thể dùng cả Windows lẫn macOS; sai EOL sẽ gây diff khổng lồ trong `flows.json`.
- [ ] **A5.** Viết `README.md` gồm: giới thiệu 3-5 dòng, sơ đồ kiến trúc mục 0.2, bảng thành viên, bảng phân công mục 0.3, link tới `docs/setup-guide.md`.
- [ ] **A6.** Viết `docs/mqtt-topics.md` theo Phụ lục 2 (**hợp đồng** giữa ESP32 và Node-RED).
- [ ] **A7.** Viết `docs/pin-map.md` theo Phụ lục 3.
- [ ] **A8.** Viết `docs/wokwi-parts-check.md`: với **từng linh kiện** trong proposal (ESP32 DevKit, DHT22, MQ-2, PIR HC-SR501, MFRC522, Servo SG90, Relay 1 kênh, LED, LCD2004 I2C), kiểm tra Wokwi có hỗ trợ không (tra https://docs.wokwi.com/getting-started/supported-hardware và danh sách part, **tra tài liệu thật, không đoán**). Ghi bảng: *Linh kiện | Có trong Wokwi? | Tên part | Ghi chú*. Nếu thiếu linh kiện nào (nghi vấn nhất: **MFRC522**), nêu phương án thay thế **và nhắc người dùng quy tắc "tối đa MỘT thiết bị thay thế, thiết bị đó phải khác các thiết bị đã dùng"**. Chỉ đề xuất, **không tự quyết**.
- [ ] **A9.** Viết `CONTRIBUTING.md` (quy ước nhánh, commit, PR, quy tắc sửa `flows.json` – mục 5, 6) và `.github/pull_request_template.md` (Phụ lục 4).
- [ ] **A10.** Viết `.github/CODEOWNERS`: `firmware/src/core/`, `docs/mqtt-topics.md`, `platformio.ini` → nhóm trưởng; `firmware/src/modules/<tên>/` → người tương ứng.

**Kiểm tra A:** `git status` sạch sau commit; không có file nào chứa secret; `tree` khớp mục 2.
**Commit & push** lên `main` (commit đầu tiên được phép đẩy thẳng; sau đó bật branch protection).

### GIAI ĐOẠN B – Firmware (PlatformIO + Wokwi) – "Hello World"

- [ ] **B1.** Viết `firmware/platformio.ini` (Phụ lục 5): env `kiet`, `quy`, `phat`, `integrated`, mỗi env có `build_flags = -DBUILD_<TÊN>` và `build_src_filter` tương ứng.
- [ ] **B2.** Viết `include/config.h` (tên topic có tiền tố `nhom05/`, lấy đúng từ `docs/mqtt-topics.md`) và `include/secrets.example.h` (Phụ lục 6). **Không tạo `secrets.h` chứa giá trị thật.** Tạo `secrets.h` mẫu từ example chỉ khi người dùng đã cung cấp thông tin.
- [ ] **B3.** Viết `src/core/wifi_mqtt.{h,cpp}`: kết nối WiFi `Wokwi-GUEST` (kênh 6), kết nối MQTT TLS tới HiveMQ Cloud (`WiFiClientSecure` + `setInsecure()` – chấp nhận cho đồ án, ghi chú rõ trong code), tự reconnect, hàm `mqttPublish(topic, payload)`, `mqttSubscribe(topic, callback)`, LWT (Last Will) trên topic `nhom05/home/status`.
- [ ] **B4.** Viết `src/main.cpp` "Hello World": mỗi 5 giây publish `{"hello":"<env>","uptime":<ms>}` lên `nhom05/home/debug/hello`, và subscribe `nhom05/home/debug/ping` (nhận gì in ra Serial). **Chưa** viết logic cảm biến.
- [ ] **B5.** Với mỗi thư mục trong `firmware/wokwi/<tên>/`: tạo `wokwi.toml` và `diagram.json` tối thiểu (chỉ board ESP32 DevKit). Đường dẫn firmware trong `wokwi.toml` là **tương đối so với file toml** (Phụ lục 7).
- [ ] **B6.** Build thử: `pio run -e integrated` (và 3 env còn lại). Phải thành công.
- [ ] **B7.** Xác minh cách chạy Wokwi cho từng thư mục config (VS Code extension tìm `wokwi.toml` ở thư mục mở; kiểm tra tài liệu hiện hành https://docs.wokwi.com/vscode/getting-started). Nếu extension không hỗ trợ chọn file config tuỳ ý, **ghi vào `docs/setup-guide.md` cách mở từng thư mục `firmware/wokwi/<tên>` như workspace riêng** hoặc cách thay thế đã kiểm chứng. Không bịa cú pháp.
- [ ] **B8.** Mô tả trong `docs/setup-guide.md` cách chạy simulation (Build → `Wokwi: Start Simulator`).

**Kiểm tra B:** 4 env build OK; `secrets.h` nằm trong `.gitignore`; `git ls-files | grep secrets.h` chỉ ra `secrets.example.h`.

### GIAI ĐOẠN C – Node-RED

- [ ] **C1.** Tạo `node-red/package.json` (tên `smarthome-nodered`, `private: true`). Cài và ghi vào dependencies: `@flowfuse/node-red-dashboard`, `dotenv`. (Các node khác như Telegram, Email, Supabase sẽ cài sau khi từng thành viên làm chức năng của mình; ghi chú trong `docs/setup-guide.md`.)
- [ ] **C2.** Viết `node-red/settings.js`: nạp `.env` ở thư mục gốc repo bằng `dotenv` (đường dẫn tuyệt đối tính từ `__dirname`), đặt `flowFile: 'flows.json'`, `flowFilePretty: true` (**bắt buộc** – giúp `flows.json` diff được theo dòng), `credentialSecret` lấy từ biến môi trường, `uiPort` mặc định 1880. Không đặt `adminAuth` ở giai đoạn này (chạy local).
- [ ] **C3.** Viết `scripts/start-node-red.sh` và `start-node-red.ps1`: `cd node-red && npm install && npx node-red --userDir . --settings settings.js`. Phải chạy được trên Windows lẫn macOS/Linux.
- [ ] **C4.** Tạo `node-red/flows.json` gồm **4 tab**: `Chung (Hello MQTT)`, `Kiet`, `Quy`, `Phat`. Tab `Chung` chứa:
  - Cấu hình broker MQTT (host/port/username/password lấy từ env: `${MQTT_HOST}`, `${MQTT_PORT}`, `${MQTT_USER}`, `${MQTT_PASS}`, bật TLS).
  - `mqtt in` subscribe `nhom05/home/debug/hello` → `debug`.
  - Một `inject` + `mqtt out` publish `ping` lên `nhom05/home/debug/ping`.
  - 3 tab còn lại để trống, có comment node ghi tên chủ sở hữu.
  - **Lưu ý kỹ thuật:** kiểm tra trong tài liệu Node-RED xem trường credential của node cấu hình MQTT có hỗ trợ `${ENV}` không; nếu không, dùng cơ chế `credentialSecret` + `flows_cred.json` (đã gitignore) và hướng dẫn mỗi người nhập credential 1 lần trong editor. Ghi phương án đã chọn vào `docs/setup-guide.md`.
- [ ] **C5.** Viết `scripts/check-flows.js` (Node thuần): parse `flows.json`, báo lỗi nếu JSON hỏng, nếu có `id` trùng, hoặc nếu phát hiện chuỗi giống secret (regex cho token Telegram `\d{8,10}:[A-Za-z0-9_-]{35}`, `AIza[0-9A-Za-z_-]{35}`, chuỗi `eyJ` JWT dài, `password":"` có giá trị khác rỗng và khác dạng `${...}`).

**Kiểm tra C:** `node scripts/check-flows.js` pass; khởi động Node-RED lên được, mở `http://localhost:1880` thấy 4 tab.

### GIAI ĐOẠN D – Kiểm chứng đầu-cuối (Hello World xuyên suốt)

- [ ] **D1.** (Cần người dùng có HiveMQ credential – H3) Điền `secrets.h` và `.env` cục bộ.
- [ ] **D2.** Chạy Wokwi env `integrated` → Serial thấy "MQTT connected".
- [ ] **D3.** Chạy Node-RED → debug panel nhận được JSON hello mỗi 5 giây.
- [ ] **D4.** Bấm inject ping trong Node-RED → Serial của Wokwi in ra nội dung ping.
- [ ] **D5.** Chụp màn hình 2 chiều chạy được, lưu `docs/img/hello-world-ok.png`.

Nếu D2–D4 thất bại, tra mục 8 (Xử lý sự cố).

### GIAI ĐOẠN E – CI & bảo vệ nhánh

- [ ] **E1.** Viết `.github/workflows/ci.yml` (Phụ lục 8): job `firmware` (cài PlatformIO, tạo `secrets.h` giả từ `secrets.example.h`, build 4 env), job `flows` (chạy `check-flows.js`).
- [ ] **E2.** Hướng dẫn người dùng bật branch protection cho `main` (Settings → Branches): yêu cầu PR + 1 review + CI pass; cấm force-push. (Nếu repo private trên gói free không có tính năng này, ghi chú để nhóm tự giữ kỷ luật.)
- [ ] **E3.** Tạo GitHub Issues/Project (nếu có `gh`): 13 issue cho 13 chức năng, gán đúng người, label `can-ban` / `nang-cao`, tiêu đề dạng `[ID 5] Lưu lịch sử DHT22 lên Supabase (Quý)`. Dùng bảng mục 0.3 và thời lượng ở proposal mục 3.2.
- [ ] **E4.** Viết `docs/setup-guide.md` hoàn chỉnh (onboarding từ máy trắng → chạy được Hello World), kiểm lại bằng cách đọc như người mới.

**Kiểm tra E:** mở 1 PR thử (sửa README) → CI chạy xanh.

### GIAI ĐOẠN F – Bàn giao

- [ ] **F1.** In ra tóm tắt: những gì đã tạo, những gì chờ người dùng (H1–H5), kết quả `wokwi-parts-check.md`, rủi ro còn lại.
- [ ] **F2.** Gợi ý bước tiếp theo: mỗi thành viên bắt đầu chức năng căn bản (ID 1, 2, 3) trên nhánh `feature/id<N>-...`.

---

## 4. Quy ước đặt tên

- **Nhánh:** `feature/id<N>-<mô-tả-ngắn>-<tên>` (vd `feature/id5-supabase-history-quy`), `fix/...`, `docs/...`, `chore/...`.
- **Commit:** Conventional Commits: `feat(id1): doc DHT22 va publish MQTT`.
- **Topic MQTT:** luôn có tiền tố `nhom05/` (tránh đụng nhóm khác nếu sau này dùng broker công cộng để thử).
- **Tab Node-RED:** `Kiet`, `Quy`, `Phat`, `Chung`.

## 5. Quy trình làm việc hằng ngày (viết vào `CONTRIBUTING.md`)

1. `git checkout main && git pull`
2. `git checkout -b feature/id1-dht22-quy`
3. Làm việc → commit nhỏ, thường xuyên → `git push -u origin <nhánh>`
4. Mở Pull Request → điền template → nhờ 1 người review → merge (Squash).
5. Chức năng chỉ được xem là xong khi: chạy được trên Wokwi + Node-RED, đã cập nhật `mqtt-topics.md` (nếu thêm topic), đã viết `docs/flows/<id>.md` giải thích luồng (phục vụ vấn đáp).

## 6. Quy tắc chống xung đột `flows.json`

- Mỗi người **chỉ sửa tab của mình**. Tab `Chung` và config broker chỉ nhóm trưởng sửa.
- **Pull trước khi mở editor Node-RED**; deploy xong một tính năng nhỏ là commit + push luôn. Không để thay đổi flow nằm lâu chưa commit.
- Nên làm trên nhánh riêng. Nếu conflict: **không sửa tay** các dòng conflict trong JSON. Cách an toàn: lấy `flows.json` mới nhất từ `main` → mở Node-RED → Import lại tab của mình (đã Export trước) → Deploy → commit.
- `flowFilePretty: true` là bắt buộc.
- Không commit: `flows_cred.json`, `.config.*.json`, `node_modules/`, `.env`, `firmware/include/secrets.h`, `.pio/`.

## 7. Điều Agent KHÔNG ĐƯỢC làm

- Không commit bất kỳ secret nào (mật khẩu MQTT, token Telegram, key Gemini/Supabase/SMTP). Nếu lỡ commit, dừng lại, báo người dùng để **đổi (rotate) secret** (xoá khỏi lịch sử Git là chưa đủ).
- Không tự ý thay linh kiện trong proposal; chỉ đề xuất và nêu ràng buộc "tối đa 1 thiết bị thay thế".
- Không cài đặt chức năng của đồ án ngoài Hello World ở giai đoạn này.
- Không dùng `git push --force` lên `main`.
- Không tự bịa tên part Wokwi, cú pháp `wokwi.toml`, hay tên node Node-RED: tra tài liệu chính thức, không chắc thì ghi `TODO: verify` và báo người dùng.
- Không tạo tài khoản dịch vụ ngoài (HiveMQ, Supabase, Telegram) thay người dùng.

## 8. Xử lý sự cố thường gặp

| Triệu chứng | Nguyên nhân thường gặp | Cách xử lý |
|---|---|---|
| Wokwi không kết nối WiFi | Sai SSID/kênh | Dùng `WiFi.begin("Wokwi-GUEST", "", 6)` |
| MQTT connect fail (rc=-2 / 5) | Sai host/port/user/pass, hoặc quên TLS | Port 8883 + `WiFiClientSecure` + `setInsecure()`; kiểm tra credential HiveMQ |
| Node-RED không nhận tin | Khác topic/tiền tố, hoặc chưa bật TLS ở config broker | So lại với `docs/mqtt-topics.md`; bật TLS, port 8883 |
| `flows.json` diff lộn xộn | Thiếu `flowFilePretty`, khác EOL | Kiểm tra `settings.js`, `.gitattributes` |
| Merge conflict `flows.json` | Nhiều người sửa cùng lúc | Quy trình mục 6 |
| `pio run` thiếu thư viện | Chưa khai báo `lib_deps` | Thêm vào env tương ứng |
| Wokwi báo thiếu license | Chưa request key | Task H4 |

## 9. Định nghĩa "HOÀN THÀNH" của giai đoạn base

- [ ] Repo private, 3 người đều clone/push/PR được.
- [ ] Cấu trúc thư mục đúng mục 2; có đủ tài liệu ở `docs/`.
- [ ] `pio run` pass cho 4 env; CI xanh.
- [ ] Hello World 2 chiều (Wokwi ↔ HiveMQ ↔ Node-RED) chạy trên máy của **cả 3 thành viên**.
- [ ] Không secret nào trong repo (`git log -p | grep -iE "password|token|apikey"` rà soát thủ công).
- [ ] `wokwi-parts-check.md` đã có kết luận về từng linh kiện.
- [ ] 13 issue đã tạo và gán người.

---

# PHỤ LỤC (nội dung mẫu để Agent dùng)

## Phụ lục 1 – `.gitignore`

```gitignore
# Secrets
.env
.env.*
!.env.example
firmware/include/secrets.h

# Node-RED / Node
node-red/node_modules/
node-red/flows_cred.json
node-red/.config.*.json
node-red/.sessions.json
node-red/*.backup
node-red/.flows.json.backup
node_modules/
npm-debug.log*

# PlatformIO
firmware/.pio/
firmware/.vscode/.browse.c_cpp.db*
firmware/.vscode/c_cpp_properties.json
firmware/.vscode/launch.json
firmware/.vscode/ipch

# Python
__pycache__/
*.pyc
.venv/
venv/
ai-service/*.joblib
ai-service/data/raw/

# OS / IDE
.DS_Store
Thumbs.db
.idea/
```

## Phụ lục 2 – `docs/mqtt-topics.md` (hợp đồng topic)

Tiền tố chung: `nhom05/`. Broker: HiveMQ Cloud, port 8883 (TLS). QoS 0 cho dữ liệu cảm biến, QoS 1 cho lệnh điều khiển.

| Topic | Hướng | Payload | Chu kỳ / Ghi chú | ID |
|---|---|---|---|---|
| `nhom05/home/sensor/dht22` | ESP → NR | `{"temp":29.5,"humi":70.2}` | 5 giây | 1 |
| `nhom05/home/sensor/gas` | ESP → NR | `{"gas":35}` (0–100, %) | 5 giây; cảnh báo khi > 70 | 3 |
| `nhom05/home/sensor/pir` | ESP → NR | `{"motion":true,"timestamp":"<ISO8601>"}` | khi có chuyển động | 7 |
| `nhom05/home/light/cmd` | NR → ESP | `ON` hoặc `OFF` (text) | QoS 1 | 2 |
| `nhom05/home/light/state` | ESP → NR | `ON` hoặc `OFF` (text, retained) | sau mỗi lần đổi | 2 |
| `nhom05/home/lcd/msg` | NR → ESP | chuỗi UTF-8 không dấu, tối đa 80 ký tự (4×20) | khi người dùng gửi | 4 |
| `nhom05/home/rfid/event` | ESP → NR *(tuỳ chọn)* | `{"uid":"AB12CD34","valid":true}` | mỗi lần quẹt | 11 |
| `nhom05/home/status` | ESP → NR | `online` / `offline` (LWT, retained) | kết nối/mất kết nối | chung |
| `nhom05/home/debug/hello` | ESP → NR | `{"hello":"<env>","uptime":12345}` | 5 giây (chỉ Hello World) | chung |
| `nhom05/home/debug/ping` | NR → ESP | text bất kỳ | thủ công | chung |

**Quy tắc:** thêm/sửa topic phải sửa file này trong cùng PR và báo nhóm trưởng. JSON dùng khóa chữ thường, số ở dạng số (không phải chuỗi). Ghi chú: LCD 2004 không hiển thị tiếng Việt có dấu, nên Node-RED cần bỏ dấu trước khi publish.

## Phụ lục 3 – `docs/pin-map.md` (theo proposal)

| Linh kiện | Tín hiệu | GPIO ESP32 | ID | Người |
|---|---|---|---|---|
| DHT22 | Data (1-Wire) | 4 | 1 | Quý |
| Relay 1 kênh | IN | 26 | 2 | Phát |
| MQ-2 | AO (qua chia áp) | 34 (ADC) | 3 | Kiệt |
| LCD 2004 I2C | SDA / SCL | 21 / 22 | 4 | Quý |
| PIR HC-SR501 | OUT | 27 | 7 | Phát |
| Servo SG90 | PWM | 13 | 11 | Kiệt |
| MFRC522 | SPI: SS=5, SCK=18, MOSI=23, MISO=19, RST=22 *(TODO: kiểm tra xung đột RST=22 với I2C SCL=22; có thể đổi RST sang chân khác như 16)* | 5/18/23/19/RST | 11 | Kiệt |

> **Phát hiện cần lưu ý:** trong sơ đồ proposal, RFID dùng GPIO 5, 19, 23 cho SPI và LCD dùng GPIO 21, 22 cho I2C. Nếu RST của MFRC522 được gán chân 22 sẽ trùng SCL của LCD. Agent hãy ghi cảnh báo này vào `pin-map.md` và đề xuất chân RST khác để nhóm quyết định.

## Phụ lục 4 – `.github/pull_request_template.md`

```markdown
## Mô tả
<!-- Làm gì? Thuộc chức năng ID nào? -->

## Loại thay đổi
- [ ] feat  - [ ] fix  - [ ] docs  - [ ] chore

## Checklist
- [ ] Đã chạy được trên Wokwi / Node-RED (kèm ảnh chụp nếu có)
- [ ] Không commit secret (`.env`, `secrets.h`, token, password)
- [ ] Chỉ sửa tab Node-RED / thư mục module của mình
- [ ] Đã cập nhật `docs/mqtt-topics.md` nếu thêm/sửa topic
- [ ] Đã cập nhật `docs/flows/<id>.md` (giải thích luồng xử lý)
- [ ] `pio run -e integrated` build thành công
```

## Phụ lục 5 – `firmware/platformio.ini`

```ini
[platformio]
default_envs = integrated

[env]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
    knolleary/PubSubClient@^2.8
    bblanchon/ArduinoJson@^7
build_flags =
    -DMQTT_MAX_PACKET_SIZE=512

[env:kiet]
build_flags = ${env.build_flags} -DBUILD_KIET
build_src_filter = +<*> -<modules/quy/> -<modules/phat/>

[env:quy]
build_flags = ${env.build_flags} -DBUILD_QUY
build_src_filter = +<*> -<modules/kiet/> -<modules/phat/>

[env:phat]
build_flags = ${env.build_flags} -DBUILD_PHAT
build_src_filter = +<*> -<modules/kiet/> -<modules/quy/>

[env:integrated]
build_flags = ${env.build_flags} -DBUILD_KIET -DBUILD_QUY -DBUILD_PHAT
```

> Thư viện riêng của từng chức năng (DHT, LiquidCrystal_I2C, MFRC522, ESP32Servo) sẽ được **thêm vào `lib_deps` của env tương ứng và env `integrated`** khi từng người làm chức năng đó. Agent chỉ cần để sẵn chú thích.

## Phụ lục 6 – `firmware/include/secrets.example.h`

```cpp
#pragma once
// Copy file này thành secrets.h và điền giá trị thật. KHÔNG commit secrets.h.
#define WIFI_SSID      "Wokwi-GUEST"
#define WIFI_PASSWORD  ""
#define WIFI_CHANNEL   6

#define MQTT_HOST      "xxxxxxxxxxxxxxxx.s1.eu.hivemq.cloud"
#define MQTT_PORT      8883
#define MQTT_USER      "your-username"
#define MQTT_PASS      "your-password"
```

## Phụ lục 7 – `firmware/wokwi/<tên>/wokwi.toml` và `diagram.json` tối thiểu

`wokwi.toml` (ví dụ cho `kiet`; đổi `kiet` thành tên env tương ứng):

```toml
[wokwi]
version = 1
firmware = '../../.pio/build/kiet/firmware.bin'
elf = '../../.pio/build/kiet/firmware.elf'
```

`diagram.json`:

```json
{
  "version": 1,
  "author": "Nhom 05",
  "editor": "wokwi",
  "parts": [
    { "type": "board-esp32-devkit-c-v4", "id": "esp", "top": 0, "left": 0, "attrs": {} }
  ],
  "connections": [
    ["esp:TX", "$serialMonitor:RX", "", []],
    ["esp:RX", "$serialMonitor:TX", "", []]
  ],
  "dependencies": {}
}
```

> Agent: kiểm chứng tên part `board-esp32-devkit-c-v4` và đường dẫn tương đối trong tài liệu Wokwi hiện hành trước khi dùng; nếu khác, sửa và ghi chú.

## Phụ lục 8 – `.github/workflows/ci.yml`

```yaml
name: CI
on:
  pull_request:
  push:
    branches: [main]

jobs:
  firmware:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        env: [kiet, quy, phat, integrated]
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with: { python-version: '3.11' }
      - run: pip install platformio
      - run: cp firmware/include/secrets.example.h firmware/include/secrets.h
      - run: pio run -d firmware -e ${{ matrix.env }}

  flows:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with: { node-version: '20' }
      - run: node scripts/check-flows.js
```

## Phụ lục 9 – `.env.example`

```dotenv
# ---- MQTT (HiveMQ Cloud) ----
MQTT_HOST=xxxxxxxxxxxxxxxx.s1.eu.hivemq.cloud
MQTT_PORT=8883
MQTT_USER=
MQTT_PASS=

# ---- Node-RED ----
NODERED_CREDENTIAL_SECRET=doi-thanh-chuoi-ngau-nhien

# ---- Điền dần khi làm các chức năng tương ứng (chưa cần ở giai đoạn base) ----
SUPABASE_URL=
SUPABASE_ANON_KEY=
SUPABASE_SERVICE_KEY=
TELEGRAM_BOT_TOKEN=
TELEGRAM_CHAT_ID=
SMTP_HOST=smtp.gmail.com
SMTP_USER=
SMTP_PASS=
GEMINI_API_KEY=
AI_SERVICE_URL=http://localhost:5000
```

---

*Hết kế hoạch. Khi hoàn thành giai đoạn F, Agent báo cáo cho người dùng và chờ chỉ thị cho giai đoạn tiếp theo (xây dựng chức năng căn bản ID 1, 2, 3).*
