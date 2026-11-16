# AI Service - Mô hình học máy dự báo thời tiết mưa

- **Thành viên phụ trách:** Trần Anh Kiệt (MSSV: 24127287)
- **Chức năng:** ID 13 (Yêu cầu nâng cao #11)
- **Công nghệ:** Python 3.11+, Flask, Scikit-learn, Pandas, Open-Meteo API.

---

## Mục tiêu
Dự báo khả năng xuất hiện mưa theo 4 khung giờ trong ngày:
1. Sáng (06:00 – 12:00)
2. Chiều (12:00 – 18:00)
3. Tối (18:00 – 24:00)
4. Đêm (00:00 – 06:00)

Dữ liệu đầu vào: Lấy dữ liệu nhiệt độ, độ ẩm lịch sử từ Supabase (được đo từ cảm biến DHT22).
API trả về cho Node-RED kết quả: `rain` (True/False) cùng xác suất `%` cho từng khung giờ để hiển thị trực quan lên giao diện Web.
