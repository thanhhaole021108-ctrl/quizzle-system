# HỆ THỐNG CÂU HỎI

## 1. Giới thiệu dự án

**C HỆ THỐNG CÂU HỎI là chương trình thi trắc nghiệm được xây dựng bằng ngôn ngữ lập trình C, chạy trực tiếp trên Windows Console.

Dự án mô phỏng một hệ thống thi trắc nghiệm với giao diện tương tác, hỗ trợ thao tác bằng chuột và bàn phím, có bộ đếm thời gian, tự động trộn câu hỏi, chấm điểm và thống kê kết quả sau khi hoàn thành bài thi.

Chương trình được xây dựng nhằm vận dụng kiến thức lập trình C, đặc biệt là:
- Cấu trúc dữ liệu `struct`.
- Mảng và mảng cấu trúc.
- Hàm và cách phân chia chương trình thành các chức năng.
- Xử lý chuỗi ký tự.
- Thuật toán trộn dữ liệu ngẫu nhiên.
- Xử lý sự kiện bàn phím và chuột.
- Quản lý thời gian và tính điểm.

## 2. Chức năng chính

### 2.1. Quản lý thông tin thí sinh
- Nhập họ và tên thí sinh.
- Nhập mã số sinh viên.
- Kiểm tra thông tin nhập vào, không cho phép để trống.
- Hiển thị thông tin thí sinh trong bảng kết quả sau khi thi.
### 2.2. Vai trò của admin
- Kiểm tra chống gian lận của học sinh
- Quản lý ngân hàng câu hỏi và đáp án
- Kiểm tra phiếu điểm của học sinh
### 2.3. Lựa chọn môn thi

Chương trình cung cấp hai môn thi:

| STT | Môn thi | Số câu trong ngân hàng |
|---|---|---:|
| 1 | Triết học Mác – Lênin | 20 |
| 2 | Lý thuyết Lập trình C | 20 |

Tổng ngân hàng câu hỏi gồm **40 câu trắc nghiệm**.

### 2.4. Tạo đề thi ngẫu nhiên

Mỗi môn thi có 20 câu hỏi được chia thành ba mức độ:

| Mức độ | Số câu trong ngân hàng | Điểm/câu |
|---|---:|---:|
| Dễ | 10 | 0.5 |
| Trung bình | 5 | 1.0 |
| Khó | 5 | 1.5 |

Chương trình tạo ngẫu nhiên một trong ba dạng đề:

| Loại đề | Dễ | Trung bình | Khó | Tổng số câu |
|---|---:|---:|---:|---:|
| Đề 1 | 2 | 3 | 4 | 9 |
| Đề 2 | 4 | 2 | 4 | 10 |
| Đề 3 | 8 | 3 | 2 | 13 |

**Lưu ý:** Mỗi dạng đề đều có tổng điểm tối đa là 10 điểm.

Sau khi lấy câu hỏi theo từng mức độ, chương trình tiếp tục trộn thứ tự tổng thể để các câu hỏi xuất hiện xen kẽ ngẫu nhiên.

### 2.5. Giao diện làm bài

- Giao diện Console sử dụng màu sắc để phân biệt các thành phần.
- Hỗ trợ chọn đáp án bằng chuột hoặc bàn phím A, B, C, D.
- Di chuột vào đáp án để làm nổi bật lựa chọn.
- Nhấn Enter để xác nhận đáp án đang được chọn.
- Hiển thị tiến độ hoàn thành bài thi.
- Hiển thị số câu trả lời đúng.
- Có đồng hồ đếm ngược thời gian làm bài.
- Phát âm thanh khi thao tác và khi trả lời đúng hoặc sai.

### 2.6. Quản lý thời gian

- Thời gian tối đa cho mỗi bài thi: **20 phút**.
- Đồng hồ được cập nhật trong quá trình làm bài.
- Khi còn 30 giây, chương trình chuyển sang trạng thái cảnh báo.
- Khi hết thời gian, chương trình thông báo và hiển thị đáp án đúng của câu hỏi hiện tại.
- Thời gian làm bài thực tế và thời gian còn lại được hiển thị trong bảng kết quả.

### 2.7. Chấm điểm và thống kê kết quả

Sau khi hoàn thành bài thi, chương trình tự động:
- Kiểm tra từng đáp án.
- Đếm số câu đúng.
- Đếm số câu sai.
- Đếm số câu chưa làm.
- Tính tổng điểm theo mức điểm của từng câu.
- Tính tỷ lệ trả lời đúng.
- Tính thời gian làm bài và thời gian còn lại.

**Xếp loại kết quả:**

| Điểm số | Xếp loại |
|---|---|
| 8.5 – 10 | Giỏi |
| 7.0 – dưới 8.5 | Khá |
| 4.0 – dưới 7.0 | Trung bình |
| Dưới 4.0 | Chưa đạt |

Kết quả được hiển thị cùng thông tin thí sinh và môn thi.

### 2.8. Xem lại bài làm

Sau khi thi, thí sinh có thể lựa chọn xem lại bài làm.

Chức năng này hỗ trợ:
- Hiển thị nội dung từng câu hỏi.
- Hiển thị các phương án A, B, C, D.
- Đánh dấu đáp án đúng.
- Hiển thị đáp án thí sinh đã chọn.
- So sánh đáp án đã chọn với đáp án chính xác.
- Hiển thị nội dung câu hỏi có chứa mã nguồn C.

### 2.9. Hiệu ứng giao diện

Chương trình có các hiệu ứng nhằm tăng tính tương tác:
- Hiệu ứng rồng phun lửa khi trả lời đúng.
- Hiệu ứng cúp chiến thắng.
- Hiệu ứng pháo hoa.
- Hiệu ứng đổi màu thông báo.
- Âm thanh phản hồi khi chọn đáp án và hoàn thành câu hỏi.

## 3. Công nghệ và thư viện sử dụng

**Ngôn ngữ lập trình:** C.

**Môi trường chạy:** Windows Console.

**Các thư viện được sử dụng:**

| Thư viện | Chức năng |
|---|---|
| `stdio.h` | Nhập xuất dữ liệu |
| `stdlib.h` | Cấp phát tiện ích, số ngẫu nhiên |
| `time.h` | Quản lý thời gian và khởi tạo số ngẫu nhiên |
| `ctype.h` | Xử lý ký tự |
| `string.h` | Xử lý chuỗi |
| `conio.h` | Nhận phím trực tiếp thông qua `getch()` |
| `windows.h` | Điều khiển giao diện Console, chuột, màu sắc, âm thanh và thời gian |

**Các API Windows nổi bật:**
- `SetConsoleTextAttribute()`: Thiết lập màu chữ và màu nền.
- `SetConsoleCursorPosition()`: Di chuyển vị trí con trỏ.
- `ReadConsoleInput()`: Đọc sự kiện đầu vào.
- `SetConsoleMode()`: Thiết lập chế độ hoạt động của Console.
- `Beep()`: Phát âm thanh.
- `Sleep()`: Tạm dừng chương trình.
- `GetTickCount()`: Theo dõi thời gian làm bài.

## 4. Thiết kế cấu trúc dữ liệu (STRUCT)

Chương trình sử dụng hai cấu trúc dữ liệu chính.

### 4.1. Struct CauHoi

Dùng để lưu trữ thông tin của một câu hỏi trắc nghiệm.

```c
struct CauHoi {
    char noiDung[300];
    char a[150];
    char b[150];
    char c[150];
    char d[150];
    char dapAnDung;
    float diem;
};
```

**Ý nghĩa các trường dữ liệu:**

| Trường | Kiểu dữ liệu | Ý nghĩa |
|---|---|---|
| `noiDung` | `char[300]` | Nội dung câu hỏi |
| `a` | `char[150]` | Phương án A |
| `b` | `char[150]` | Phương án B |
| `c` | `char[150]` | Phương án C |
| `d` | `char[150]` | Phương án D |
| `dapAnDung` | `char` | Ký tự đáp án chính xác |
| `diem` | `float` | Điểm số của câu hỏi |

Mỗi câu hỏi được lưu dưới dạng một biến thuộc kiểu `struct CauHoi`.

Các câu hỏi được tổ chức trong mảng cấu trúc:

```c
struct CauHoi nganHang[40];
```

Mảng này lưu trữ toàn bộ ngân hàng câu hỏi của hai môn thi.

### 4.2. Struct NguoiThi

Dùng để lưu trữ thông tin cá nhân của thí sinh.

```c
struct NguoiThi {
    char hoTen[56];
    char maSoSinhVien[30];
};
```

| Trường | Kiểu dữ liệu | Ý nghĩa |
|---|---|---|
| `hoTen` | `char[56]` | Họ và tên thí sinh |
| `maSoSinhVien` | `char[30]` | Mã số sinh viên |

Thông tin được truyền vào các hàm thông qua con trỏ cấu trúc:

```c
void nhapThongTinNguoiThi(struct NguoiThi *thiSinh);
```

Việc sử dụng con trỏ giúp hàm có thể thao tác trực tiếp trên dữ liệu của thí sinh.

## 5. Luồng hoạt động của chương trình

Quy trình tổng thể:

```text
             BẮT ĐẦU
                 |
                 v
       Khởi tạo môi trường Console
                 |
                 v
        Nhập thông tin thí sinh
                 |
                 v
          Hiển thị menu chính
                 |
                 v
          Chọn môn thi
                 |
                 v
       Lấy 20 câu từ ngân hàng
                 |
                 v
       Phân loại câu hỏi theo
          3 mức độ khó
                 |
                 v
       Trộn câu hỏi từng nhóm
                 |
                 v
       Chọn ngẫu nhiên dạng đề
                 |
                 v
        Tạo đề thi hoàn chỉnh
                 |
                 v
          Trộn thứ tự câu hỏi
                 |
                 v
          Bắt đầu đếm giờ
                 |
                 v
        Thí sinh làm bài thi
                 |
                 v
        Kiểm tra và chấm điểm
                 |
                 v
        Hiển thị bảng kết quả
                 |
                 v
           Xem lại bài làm
                 |
                 v
          Chọn thi lại hoặc
             kết thúc
                 |
                 v
                KẾT THÚC
```

## 6. Các hàm chức năng chính

Chương trình được chia thành nhiều hàm nhằm tách biệt từng nhiệm vụ và thuận tiện cho việc quản lý, chỉnh sửa.

| Tên hàm | Chức năng |
|---|---|
| `khoiTaoConsole()` | Khởi tạo giao diện Console |
| `batCheDoChuot()` | Bật chế độ nhận sự kiện chuột |
| `choConsoleDuKichThuoc()` | Kiểm tra kích thước cửa sổ Console |
| `xoaManHinh()` | Xóa nội dung màn hình |
| `caiDatMau()` | Thiết lập màu giao diện |
| `veKhung()` | Vẽ khung giao diện |
| `veBannerDau()` | Hiển thị tiêu đề |
| `veThanhTienDo()` | Hiển thị thanh tiến độ |
| `tronDeThi()` | Trộn ngẫu nhiên câu hỏi |
| `chonMenuBangChuot()` | Hiển thị và xử lý menu chọn môn |
| `chonDapAnBangChuot()` | Xử lý lựa chọn đáp án bằng chuột và bàn phím |
| `nhapThongTinNguoiThi()` | Nhập thông tin thí sinh |
| `hienThiXemLai()` | Hiển thị chi tiết bài làm |
| `luaChonTiepTucGiaoDien()` | Xử lý lựa chọn thi lại hoặc kết thúc |
| `hieuUngSai()` | Hiển thị thông báo trả lời sai |
| `hieuUngHetGio()` | Hiển thị thông báo hết thời gian |
| `hieuUngRongPhunLua()` | Tạo hiệu ứng rồng |
| `hieuUngCupXoay()` | Tạo hiệu ứng cúp chiến thắng |
| `hieuUngPhaoHoaBenLe()` | Tạo hiệu ứng pháo hoa |

## 7. Cấu trúc thư mục dự án

```text
C-Kiem-Tra-Kien-Thuc/
│
├── code_giao_dien.c
│
├── README.md
│
└── .gitignore
```

Trong đó:

- `code_giao_dien.c`: Chứa toàn bộ mã nguồn chương trình.
- `README.md`: Tài liệu giới thiệu và hướng dẫn sử dụng dự án.
- `.gitignore`: Quy định các tệp không cần đưa lên GitHub.

## 8. Hướng dẫn cài đặt và chạy chương trình

### 8.1. Yêu cầu hệ thống

- Hệ điều hành Windows.
- Trình biên dịch C hỗ trợ Windows API.
- Khuyến nghị sử dụng GCC thông qua MinGW-w64.
- Cửa sổ Console có kích thước tối thiểu 120 cột × 40 dòng.

**Lưu ý:** Chương trình sử dụng `windows.h` và `conio.h`, vì vậy không thể chạy trực tiếp trên môi trường Linux hoặc macOS nếu không điều chỉnh mã nguồn.

### 8.2. Tải mã nguồn

Clone repository về máy:

```bash
git clone <repository-url>
```

Di chuyển vào thư mục dự án:

```bash
cd C-Kiem-Tra-Kien-Thuc
```

### 8.3. Biên dịch chương trình

Sử dụng GCC:

```bash
gcc -std=c99 -Wall -Wextra code_giao_dien.c -o quiz.exe
```

### 8.4. Chạy chương trình

```bash
quiz.exe
```

Hoặc chạy trực tiếp tệp thực thi trong Windows Terminal / Command Prompt.

## 9. Hướng dẫn sử dụng

**Bước 1:** Khởi chạy chương trình.

**Bước 2:** Nhập họ tên và mã số sinh viên.

**Bước 3:** Chọn môn thi:
- Phím `1`: Triết học Mác – Lênin.
- Phím `2`: Lý thuyết Lập trình C.
- Phím `0` hoặc `ESC`: Thoát khỏi chương trình.

**Bước 4:** Bắt đầu làm bài thi.

**Bước 5:** Chọn đáp án bằng cách:
- Click chuột vào phương án.
- Nhấn phím A, B, C hoặc D.
- Di chuột vào đáp án và nhấn Enter để xác nhận.

**Bước 6:** Theo dõi thời gian và tiến độ làm bài.

**Bước 7:** Xem bảng tổng kết điểm sau khi hoàn thành.

**Bước 8:** Lựa chọn xem lại bài làm hoặc bỏ qua.

**Bước 9:** Chọn thi lại hoặc kết thúc chương trình.

## 10. Hạn chế hiện tại

- Chương trình chỉ hỗ trợ hệ điều hành Windows.
- Ngân hàng câu hỏi được khai báo trực tiếp trong mã nguồn.
- Chưa có chức năng lưu kết quả thi vào tệp hoặc cơ sở dữ liệu.
- Chưa có hệ thống đăng nhập và quản lý tài khoản.
- Giao diện phụ thuộc vào kích thước cửa sổ Console.
- Nội dung câu hỏi hiện được lưu dưới dạng chuỗi không dấu để hạn chế vấn đề hiển thị ký tự trong Console.

## 11. Hướng phát triển

Trong tương lai, dự án có thể được mở rộng với các chức năng:

- Đọc ngân hàng câu hỏi từ tệp bên ngoài.
- Cho phép thêm, sửa và xóa câu hỏi.
- Lưu lịch sử kết quả thi.
- Xây dựng chức năng quản lý thí sinh.
- Bổ sung nhiều môn thi và nhiều bộ đề.
- Nâng cấp giao diện và khả năng tùy chỉnh.
- Tách mã nguồn thành nhiều tệp `.c` và `.h` để dễ quản lý.
- Xây dựng hệ thống thống kê kết quả qua nhiều lần thi.

--- 
## Project by
- [LÊ ĐOÀN THANH HẢO] MSSV: 6751071018
- [TRẦN DIỆU LINH] MSSV: 675071044
- [HOÀNG THIÊN ÂN] MSSV: 6751071005
- [ĐOÀN ĐỨC HIẾU MSSV] MSSV: 6751071022
- [BÙI VIỆT HOÀNG MSSV] MSSV: 6751071024

