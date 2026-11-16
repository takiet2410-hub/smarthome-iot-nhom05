# QUY ĐỊNH ĐÓNG GÓP VÀ QUY TRÌNH PHÁT TRIỂN (CONTRIBUTING)

Tài liệu này hướng dẫn quy trình phối hợp làm việc nhóm trên GitHub cho 3 thành viên Nhóm 05.

---

## 1. Quy ước đặt tên nhánh (Branching)

- **Nhánh tính năng:** `feature/id<N>-<mô-tả-ngắn>-<tên>`  
  *Ví dụ:* `feature/id1-dht22-quy`, `feature/id2-relay-phat`, `feature/id3-mq2-kiet`, `feature/id11-rfid-servo-kiet`
- **Nhánh sửa lỗi:** `fix/<tên-lỗi>-<tên>` (ví dụ: `fix/dht22-nan-reading-quy`)
- **Nhánh tài liệu:** `docs/<tên-tài-liệu>` (ví dụ: `docs/update-pin-map`)
- **Nhánh cấu hình chung:** `chore/<nội-dung>` (ví dụ: `chore/add-ci-workflow`)

---

## 2. Quy ước Commit (Conventional Commits)

Mỗi commit cần có định dạng rõ ràng, ngắn gọn và có ý nghĩa:
- `feat(id1): đọc dữ liệu dht22 và publish mqtt`
- `feat(id2): xử lý mqtt out bật tắt relay đèn`
- `fix(id4): loại bỏ dấu tiếng việt trước khi gửi lên lcd2004`
- `docs(pinmap): cập nhật sơ đồ chân rst rfid sang gpio 16`
- `chore: cấu hình platformio lib_deps cho integrated`

---

## 3. Quy trình làm việc hàng ngày (Daily Workflow)

Mỗi khi bắt đầu làm một tính năng mới:

1. **Đồng bộ mã nguồn mới nhất:**
   ```bash
   git checkout main
   git pull origin main
   ```
2. **Tạo nhánh mới:**
   ```bash
   git checkout -b feature/id1-dht22-quy
   ```
3. **Thực hiện code và commit nhỏ, thường xuyên:**
   ```bash
   git add .
   git commit -m "feat(id1): cau hinh doc dht22"
   git push -u origin feature/id1-dht22-quy
   ```
4. **Mở Pull Request (PR):**
   - Điền đầy đủ thông tin theo mẫu **Pull Request Template**.
   - Gắn thẻ ít nhất 1 thành viên khác trong nhóm vào review.
   - Khi đã test pass và được approve: Tiến hành **Squash and merge** vào `main`.
5. **Tiêu chuẩn hoàn thành (Definition of Done):**
   - Đã chạy kiểm thử thành công trên Wokwi và Node-RED.
   - Cập nhật [docs/mqtt-topics.md](docs/mqtt-topics.md) nếu có thay đổi hoặc bổ sung topic.
   - Viết tài liệu giải thích luồng hoạt động trong thư mục `docs/flows/id<N>.md` (bắt buộc để phục vụ thi vấn đáp).

---

## 4. QUY TẮC BẢO VỆ VÀ TRÁNH XUNG ĐỘT (MERGE CONFLICT) CHO `flows.json`

File `flows.json` của Node-RED là file JSON lưu toàn bộ luồng của dự án. Nếu nhiều người cùng chỉnh sửa sai quy cách sẽ dẫn đến xung đột git rất khó giải quyết. Vì vậy, bắt buộc tuân thủ các quy tắc sau:

1. **Phân chia Tab độc lập:**
   - Tab `Kiet`: Chỉ bạn Kiệt chỉnh sửa.
   - Tab `Quy`: Chỉ bạn Quý chỉnh sửa.
   - Tab `Phat`: Chỉ bạn Phát chỉnh sửa.
   - Tab `Chung (Hello MQTT)` và cấu hình MQTT Broker chung: Chỉ Nhóm trưởng chỉnh sửa khi có sự thống nhất của cả nhóm.
2. **Định dạng Pretty Print bắt buộc:**
   - Trong `node-red/settings.js`, thuộc tính `flowFilePretty: true` luôn luôn được bật để file JSON được ngắt dòng đẹp mắt, hỗ trợ xem diff chính xác trên Git.
3. **Quy trình kéo và đẩy khi sửa Node-RED:**
   - Luôn `git pull` trước khi mở Node-RED editor.
   - Làm xong một nhánh tính năng nhỏ, export tab của mình hoặc lưu lại ngay, commit và push lên nhánh PR sớm, không để code tồn đọng lâu ngày trên máy cá nhân.
4. **Cách giải quyết an toàn khi bị conflict `flows.json`:**
   - **TUYỆT ĐỐI KHÔNG** sửa thủ công các dấu conflict `<<<<<<<`, `=======`, `>>>>>>>` bên trong file `flows.json`.
   - **Cách xử lý đúng:**
     1. Mở Node-RED của bạn, chọn tab của bạn → Bấm Menu góc phải → `Export` → Chọn `Current flow` → Copy JSON ra file tạm.
     2. Hủy các thay đổi local trên `flows.json` và kéo code mới nhất từ `main` về (`git checkout origin/main -- node-red/flows.json`).
     3. Mở lại Node-RED → Bấm Menu → `Import` → Dán lại JSON của bạn vào đúng tab của mình → Bấm `Deploy`.
     4. Lúc này diff sẽ chỉ gồm đúng những thay đổi trong tab của bạn → Commit và push bình thường.

---

## 5. CÁC TÀI NGUYÊN TUYỆT ĐỐI KHÔNG COMMIT LÊN GITHUB

Để bảo mật và tránh làm bẩn repository, các mục sau đã nằm trong `.gitignore` và không bao giờ được commit:
- File cấu hình chứa mật khẩu thật: `.env`, `firmware/include/secrets.h`.
- Khóa bảo mật Node-RED: `node-red/flows_cred.json`, `node-red/.config.*.json`.
- Thư mục thư viện cài đặt: `node_modules/`, `firmware/.pio/`, `venv/`, `__pycache__/`.
- Token bot Telegram, API Key Gemini, Supabase Service Key, Mật khẩu HiveMQ Cloud.
