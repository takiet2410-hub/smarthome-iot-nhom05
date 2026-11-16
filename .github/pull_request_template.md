## Mô tả
<!-- Nêu ngắn gọn nội dung thay đổi. Thuộc chức năng ID nào? Phụ trách bởi ai? -->

## Loại thay đổi
- [ ] `feat`: Thêm chức năng mới
- [ ] `fix`: Sửa lỗi
- [ ] `docs`: Cập nhật tài liệu
- [ ] `chore`: Cấu hình hệ thống, CI/CD, dependencies

## Checklist kiểm tra trước khi tạo PR
- [ ] Đã chạy thử nghiệm thành công trên Wokwi và Node-RED (đính kèm hình ảnh/log nếu có).
- [ ] **Bảo mật:** Không commit bất kỳ mật khẩu hoặc token nào (`.env`, `secrets.h`, `flows_cred.json`, API key).
- [ ] **Phân quyền:** Chỉ chỉnh sửa trong tab Node-RED hoặc thư mục module mã nguồn được phân công.
- [ ] Đã cập nhật [docs/mqtt-topics.md](docs/mqtt-topics.md) nếu có thêm hoặc chỉnh sửa MQTT topic.
- [ ] Đã cập nhật hoặc tạo file giải thích luồng tại `docs/flows/id<N>.md` (phục vụ thi vấn đáp).
- [ ] Build kiểm tra thành công với PlatformIO (`pio run -e integrated`).
