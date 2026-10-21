#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h> // Thư viện hỗ trợ hàm toupper() để chuyển chữ cái thường thành chữ hoa

// 1. ĐỊNH NGHĨA CẤU TRÚC (STRUCT) CHO MỘT CÂU HỎI
struct CauHoi {
    char noiDung[250]; // Nội dung câu hỏi
    char a[150];       // Đáp án A
    char b[150];       // Đáp án B
    char c[150];       // Đáp án C
    char d[150];       // Đáp án D
    char dapAnDung;    // Đáp án đúng (Lưu ký tự 'A', 'B', 'C' hoặc 'D')
};

// 2. HÀM TRỘN ĐỀ THI
void tronDeThi(struct CauHoi ds[], int soLuong) {
    for (int i = 0; i < soLuong; i++) {
        // Chọn ngẫu nhiên một vị trí bất kỳ trong mảng
        int viTriNgauNhien = rand() % soLuong;
        
        // Hoán đổi câu hỏi tại vị trí i và vị trí ngẫu nhiên
        struct CauHoi tam = ds[i];
        ds[i] = ds[viTriNgauNhien];
        ds[viTriNgauNhien] = tam;
    }
}

int main() {
    // Khởi tạo hạt giống ngẫu nhiên dựa trên thời gian thực
    srand(time(NULL));

    // NẠP NGÂN HÀNG CÂU HỎI MÔN TRIẾT HỌC MÁC - LÊNIN (10 CÂU)
    struct CauHoi nganHangTriet[10] = {
        {
            "Van de co ban cua triet hoc la gi?",
            "Van de moi quan he giua tu duy va ton tai (giua y thuc va vat chat).",
            "Van de moi quan he giua con nguoi va tu nhien.",
            "Van de ban chat cua cac hien tuong xa hoi.",
            "Van de nguon goc cua the gioi dong vat.",
            'A'
        },
        {
            "Theo Ph.Angghen, van de co ban cua triet hoc gom co may mat?",
            "1 mat.", "2 mat.", "3 mat.", "4 mat.",
            'B'
        },
        {
            "Khai niem \"vat chat\" theo dinh nghia kinh dien cua V.I.Lenin duoc hieu la gi?",
            "Mot pham tru triet hoc dung de chi thuc tai khach quan duoc dem lai cho con nguoi trong cam giac.",
            "Tat ca nhung vat the huu hinh ma con nguoi nhin thay duoc.",
            "Khoi luong va nang luong cua cac doi tuong tu nhien.",
            "Cac hat nguyen tu va truong vat ly.",
            'A'
        },
        {
            "Nguon goc tu nhien cua y thuc bao gom nhung yeu to nao?",
            "Bo nao con nguoi va the gioi khach quan tac dong len bo nao.",
            "Lao dong va ngon ngu.",
            "Tri thuc, tinh cam va y chi.",
            "Bo nao con nguoi va ngon ngu.",
            'A'
        },
        {
            "Theo triet hoc Mac - Lenin, phuong thuc ton tai cua vat chat la gi?",
            "Khong gian.", "Thoi gian.", "Van dong.", "Y thuc.",
            'C'
        },
        {
            "Diem khac biet co ban giua chu nghia duy vat va chu nghia duy tam trong viec giai quyet mat thu nhat cua van de co ban cua triet hoc la gi?",
            "Chu nghia duy vat cho rang vat chat co truoc y thuc quyet dinh, con chu nghia duy tam cho rang y thuc co truoc vat chat quyet dinh.",
            "Chu nghia duy vat tin vao than linh, con chu nghia duy tam phu nhan than linh.",
            "Chu nghia duy vat thua nhan kha nang nhan thuc cua con nguoi, con chu nghia duy tam phu nhan hoan toan.",
            "Chu nghia duy vat nguyen cuu xa hoi, con chu nghia duy tam nghien cuu tu nhien.",
            'A'
        },
        {
            "Phuong phap luan bien chung khac voi phuong phap sieu hinh o diem can ban nao sau day?",
            "Xem xet su vat, hien tuong trong moi lien he pho bien va su van dong, phat trien khong meo.",
            "Xem xet su vat trong trang thai co lap, tinh lang, khong bien doi.",
            "Phu nhan hoan toan su ton tai cua mau thuan ben trong su vat.",
            "Cho rang su phat trien chi do cac luc luong ben ngoai tac dong.",
            'A'
        },
        {
            "Moi quan he giua vat chat va y thuc theo quan diem cua chu nghia duy vat bien chung duoc giai thich nhu the nao?",
            "Vat chat quyet dinh y thuc, con y thuc co tinh doc lap tuong doi va tac dong tro lai vat chat thong qua hoat dong thuc tien.",
            "Y thuc quyet dinh hoan toan vat chat va sinh ra the gioi vat chat.",
            "Vat chat va y thuc ton tai biet lap, khong tac dong qua lai lan nhau.",
            "Y thuc quyet dinh vat chat trong xa hoi, con vat chat quyet dinh y thuc trong tu nhien.",
            'A'
        },
        {
            "Quy luat triet hoc nao vach ra nguon goc va dong luc cua su van dong, phat trien?",
            "Quy luat chuyen hoa tu nhung thay doi ve luong dan den nhung thay doi ve chat va nguoc lai.",
            "Quy luat thong nhat va dau tranh cua cac mat doi lap (Quy luat mau thuan).",
            "Quy luat phu dinh cua phu dinh.",
            "Quy luat ve su phu hop cua quan he san xuat voi trình do luc luong san xuat.",
            'B'
        },
        {
            "Trong cac nguon goc xa hoi cua y thuc, yeu to nao dong vai tro quyet dinh truc tiep den su ra doi va phat trien cua y thuc?",
            "Lao dong.", "Ngon ngu.", "Nghe thuat.", "Vui choi giai tri.",
            'A'
        }
    };

    // NẠP NGÂN HÀNG CÂU HỎI MÔN LẬP TRÌNH C (20 CÂU)
    struct CauHoi nganHangC[20] = {
        {
            "Trong ngon ngu C, cau truc dieu kien if-else duoc su dung nham muc dich gi?", 
            "Thuc hien lap lai mot khoi lenh voi so lan xac dinh truoc.", 
            "Lua chon thuc hien mot trong hai nhanh lenh dua tren ket qua kiem tra dieu kien (Dung hoac Sai).", 
            "Khai bao mot mang chua tap hop cac phan tu cung kieu du lieu.", 
            "Tam dung hoac ket thuc chuong trinh ngay lap tuc.", 
            'B'
        },
        {
            "Trong cau truc switch-case, tu khoa nao dung de thoat khoi cau lenh switch ngay sau khi mot truong hop case thuc thi xong?", 
            "continue", "goto", "break", "return", 
            'C'
        },
        {
            "Diem khac biet can ban nhat giua vong lap do-while va vong lap while la gi?", 
            "Vong lap do-while luon thuc hien khoi lenh ben trong it nhat 1 lan truoc khi kiem tra dieu kien.", 
            "Vong lap while khong yeu cau phai co bieu thuc dieu kien dung.", 
            "Vong lap do-while chi hoat dong khi dieu kien kiem tra ban dau nhan gia tri sai.", 
            "Vong lap while kiem tra dieu kien o cuoi than vong lap.", 
            'A'
        },
        {
            "Trong ngon ngu lap trinh C, chi so (index) de truy cap phan tu dau tien cua mot mang mot chieu luon bat dau bang gia tri nao?", 
            "1", "0", "-1", "Tuy thuoc vao tong so luong phan tu khai bao trong mang.", 
            'B'
        },
        {
            "Cau truc chuan cua vong lap for trong ngon ngu C gom 3 bieu thuc thanh phan dat trong dau ngoac don theo thu tu nao?", 
            "(Khoi tao; Cap nhat buoc lap; Dieu kien kiem tra)", 
            "(Dieu kien kiem tra; Khoi tao; Cap nhat buoc lap)", 
            "(Khoi tao; Dieu kien kiem tra; Cap nhat buoc lap)", 
            "(Cap nhat buoc lap; Khoi tao; Dieu kien kiem tra)", 
            'C'
        },
        {
            "Nhanh default trong cau lenh switch-case dong vai tro gi?", 
            "Bat buoc phai khai bao o dau khoi switch de khoi tao gia tri.", 
            "La thanh phan bat buoc phai co trong moi cau truc switch-case.", 
            "Duoc thuc thi khi gia tri cua bieu thuc switch khong trung khop voi bat ky gia tri case nao.", 
            "Tu dong chuyen huong chuong trinh sang mot vong lap khac.", 
            'C'
        },
        {
            "Khi cau lenh continue duoc thuc thi ben trong than cua mot vong lap for, chuong trinh se xu ly ra sao?", 
            "Thoat ra khoi vong lap for ngay lap tuc.", 
            "Bo qua cac cau lenh con lai trong luot lap hien tai va chuyen ngay den buoc cap nhat bien dem cho lan lap tiep theo.", 
            "Khoi dong lai toan bo vong lap tu trang thai khoi tao ban dau.", 
            "Tam dung chuong trinh cho den khi nhan tin hieu nhap tu ban phim.", 
            'B'
        },
        {
            "Cho mot mang mot chieu gom N phan tu trong C. Pham vi chi so hop le de truy cap an toan cac phan tu cua mang la gi?", 
            "Tu 1 den N", "Tu 0 den N", "Tu 0 den N - 1", "Tu 1 den N - 1", 
            'C'
        },
        {
            "Bieu thuc dieu kien trong cau lenh if (bieu_thuc) trong C duoc he thong danh gia la \"Dung\" (True) trong truong hop nao?", 
            "Chi khi bieu thuc tra ve duy nhat gia tri 1.", 
            "Khi bieu thuc danh gia ra bat ky gia tri nao khac 0.", 
            "Chi khi bieu thuc la mot so nguyen duong.", 
            "Khi bieu thuc tra ve gia tri bang 0.", 
            'B'
        },
        {
            "Khi su dung ten cua mot mang mot chieu trong bieu thuc hoac lam tham so truyen vao ham, ten mang dai dien cho thong tin gi?", 
            "Gia tri cua phan tu nam o vi tri cuoi cung trong mang.", 
            "Tong so luong phan tu da khai bao cua mang.", 
            "Dia chi o nho cua phan tu dau tien (phan tu chi so 0) trong mang.", 
            "Tong kich thuoc bo nho (tinh theo byte) ma mang chiem dung.", 
            'C'
        },
        {
            "Neu mot nhanh case trung khop trong cau lenh switch nhung khong chua lenh break o cuoi nhanh, chuong trinh se hoat dong the nao?", 
            "Bao loi bien dich ngay lap tuc do thieu tu khoa ngat.", 
            "Tu dong dung thuc thi va thoat khoi cau truc switch.", 
            "Tiep tuc thuc hien tuan tu cac lenh o cac nhan case ngay ben duoi cho den khi gap lenh break hoac het khoi switch (hien thuong troi nhanh - fall-through).", 
            "Quay tro lai kiem tra lai bieu thuc dieu kien cua switch tu dau.", 
            'C'
        },
        {
            "De duyiet toan bo mot mang mot chieu N phan tu theo chieu nguoc tu phan tu cuoi cung ve phan tu dau tien, viec khoi tao va kiem tra bien dem i trong vong lap for phai thiet lap ra sao?", 
            "Khoi tao i = N, dieu kien dung i > 0, giam i.", 
            "Khoi tao i = N - 1, dieu kien dung i >= 0, giam i.", 
            "Khoi tao i = N - 1, dieu kien dung i > 0, giam i.", 
            "Khoi tao i = N, dieu kien dung i >= 0, giam i.", 
            'B'
        },
        {
            "Trong vong lap while (dieu_kien), neu dieu_kien ngay o lan kiem tra dau tien da nhan gia tri Sai (False / 0), than vong lap se duoc thuc thi bao nhieu lan?", 
            "0 lan.", "1 lan.", "Lap vo han.", "Phu thuoc vao kieu du lieu cua bien dieu kien.", 
            'A'
        },
        {
            "Hanh vi truy cap hoac ghi du lieu vao chi so vuot qua pham vi khai bao cua mang mot chieu trong C se gay ra hau qua gi?", 
            "Trinh bien dich C tu dong mo rong kich thuoc mang de luu giu du lieu moi.", 
            "Trinh bien dich se ngan can va luon bat loi cu phap luc bien dich.", 
            "Gay ra loi truy cap o nho khong hop le (Undefined Behavior) hoac lam sai lech du lieu bien khac luc chay.", 
            "Gia tri truy xuat luon tu dong quy ve 0.", 
            'C'
        },
        {
            "Trong chuoi lenh dieu kien gom nhieu nhanh if - else if - else, quy trinh danh gia dieu kien cua ngon ngu C dien ra nhu the nao?", 
            "Kiem tra tat ca cac dieu kien bat ke cac nhanh phia truoc dung hay sai.", 
            "Kiem tra lan luot tu tren xuong; ngay khi gap nhanh dau tien co dieu kien Dung, khoi lenh do se thuc thi va tat ca cac nhanh con lai se bi bo qua.", 
            "Chi kiem tra cac nhanh else if neu nhanh else bi loi.", 
            "Danh gia cac nhanh else if theo thu tu ngau nhien.", 
            'B'
        },
        {
            "Khi mot mang mot chieu duoc khai bao cuc bo ben trong mot ham ma khong duoc khoi tao gia tri ban dau, cac phan tu cua mang se chua du lieu gi?", 
            "Tat ca cac phan tu tu dong duoc gan gia tri bang 0.", 
            "Cac phan tu chua gia tri rac (garbage values) khong xac dinh dang co san tai vung nho duoc cap phat.", 
            "Chuong trinh bao loi bien dich do meo gan gia tri ban dau.", 
            "Moi phan tu deu nhan gia tri hang so NULL.", 
            'B'
        },
        {
            "Neu mot mang mot chieu co 10 phan tu nhung nguoi lap trinh chi khoi tao danh sach gia tri cho 2 phan tu dau tien, cac phan tu con lai trong mang se co gia tri nhu the nao?", 
            "Giu cac gia tri rac khong xac dinh.", 
            "Trinh bien dich bao loi vi thieu so luong gia tri khoi tao.", 
            "Tat ca cac phan tu khong duoc khoi tao ro rang se tu dong duoc gan gia tri bang 0.", 
            "Kich thuoc mang tu dong thu hẹp lai chi con 2 phan tu.", 
            'C'
        },
        {
            "Phat bieu nao sau day ve quy dinh gia tri nhan case trong cau lenh switch la DUNG chuan ngon ngu C?", 
            "Gia tri cua nhan case co the la mot so thuc kieu float hoac double.", 
            "Gia tri cua nhan case bat buoc phai la mot hang so nguyen (integer constant) hoac hang ky tu, khong duoc su dung bien hay so thuc.", 
            "Hai nhan case khac nhau trong cung mot cau lenh switch co the trung gia tri voi nhau.", 
            "Bieu thuc kiem tra cua switch co the nhan du lieu kieu chuoi ky tu (string).", 
            'B'
        },
        {
            "Xet ve mat ly thuyet dieu khien, truong hop nao sau day co the dan den mot vong lap vo han (infinite loop) trong C?", 
            "Ca 3 bieu thuc trong cu phap for( ; ; ) deu bi de trong.", 
            "Bieu thuc dieu kien kiem tra dung cua vong lap while hoac for luon danh gia ra gia tri Dung (khac 0).", 
            "Bien dieu khien vong lap bi bien doi sai quy luat ben trong than lap khien dieu kien dung khong bao gio dat duoc.", 
            "Tat ca cac phuong an tren deu dung.", 
            'D'
        },
        {
            "Cho doan code sau:
              #include <stdio.h>
               int main(){
                int a[5]={1,2,3,4,5};
                int S = 0;
                for (int i=0;i<5;i++){
                    if (a[i]%2 == 0){
                        S +=a[i]*2;
                } else {
                   S +=a[i];
                 }
                 }
                  printf("%d,S);
                  return 0;
                 }
                  Man hinh se in ra ket qua nao sau day?", 
            "15", 
            "21", 
            "18", 
            "30", 
            'B'
        }
    };

    int luaChon;

    do {
        // HIỂN THỊ MENU PHÂN CHIA 2 MÔN THI
        printf("\n============================================\n");
        printf("       CHUONG TRINH THI TRAC NGHIEM C       \n");
        printf("============================================\n");
        printf("1. Mon Triet hoc Mac - Lenin (10 cau - 1.0 diem/cau)\n");
        printf("2. Mon Ly thuyet Lap trinh C (20 cau - 0.5 diem/cau)\n");
        printf("0. Thoat chuong trinh\n");
        printf("============================================\n");
        printf("Nhap lua chon cua ban (0-2): ");
        scanf("%d", &luaChon);

        int soCauThi = 0;
        struct CauHoi deThi[20]; // Mảng chứa đề thi tạm thời

        // SWITCH-CASE PHÂN LOẠI MÔN THI VÀ SỐ CÂU HỎI
        switch (luaChon) {
            case 1:
                soCauThi = 10;
                // Sao chép 10 câu Triết học vào đề thi
                for (int i = 0; i < soCauThi; i++) {
                    deThi[i] = nganHangTriet[i];
                }
                break;
            case 2:
                soCauThi = 20;
                // Sao chép 20 câu Lập trình C vào đề thi
                for (int i = 0; i < soCauThi; i++) {
                    deThi[i] = nganHangC[i];
                }
                break;
            case 0:
                printf("\nCam on ban da su dung chuong trinh. Tam biet!\n");
                return 0;
            default:
                printf("\n[Thong bao] Lua chon khong hop le! Vui long chon lai.\n");
                continue;
        }

        // Trộn ngẫu nhiên câu hỏi trong đề thi
        tronDeThi(deThi, soCauThi);

        // Bấm giờ bắt đầu làm bài
        time_t thoiGianBatDau = time(NULL);

        int soCauDung = 0;
        char traLoi;

        // VÒNG LẶP CHO THÍ SINH LÀM BÀI
        for (int i = 0; i < soCauThi; i++) {
            printf("\n--------------------------------------------\n");
            printf("Cau %d: %s\n", i + 1, deThi[i].noiDung);
            printf("  A. %s\n", deThi[i].a);
            printf("  B. %s\n", deThi[i].b);
            printf("  C. %s\n", deThi[i].c);
            printf("  D. %s\n", deThi[i].d);
            printf("Dap an cua ban (A/B/C/D): ");

            scanf(" %c", &traLoi);
            traLoi = toupper(traLoi); // Chuyển chữ thường thành chữ hoa

            if (traLoi == deThi[i].dapAnDung) {
                soCauDung++;
            }
        }

        // Bấm giờ kết thúc làm bài
        time_t thoiGianKetThuc = time(NULL);
        double thoiGianLamBai = difftime(thoiGianKetThuc, thoiGianBatDau);

        // TÍNH ĐIỂM VÀ TỶ LỆ DÙNG CÔNG THỨC LINH HOẠT
        float diemMoiCau = 10.0f / soCauThi;       // 10 câu -> 1.0 đ/câu | 20 câu -> 0.5 đ/câu
        float tongDiem = soCauDung * diemMoiCau;
        float phanTramDung = ((float)soCauDung / soCauThi) * 100.0f;

        // IN KẾT QUẢ KHI HOÀN THÀNH
        printf("\n============================================\n");
        printf("           KET QUA LAM BAI THI              \n");
        printf("============================================\n");
        printf("- Mon thi             : %s\n", (luaChon == 1) ? "Triet hoc Mac - Lenin" : "Ly thuyet Lap trinh C");
        printf("- Tong so cau hoi     : %d\n", soCauThi);
        printf("- So cau tra loi dung : %d\n", soCauDung);
        printf("- Ty le dung          : %.1f%%\n", phanTramDung);
        printf("- Diem cua ban        : %.2f / 10 diem\n", tongDiem);
        printf("- Thoi gian lam bai   : %.0f giay\n", thoiGianLamBai);
        printf("============================================\n");

        // Hỏi làm lại
        char tiepTuc;
        printf("\nBan co muon tiep tuc test lai khong? (Y/N): ");
        scanf(" %c", &tiepTuc);
        if (toupper(tiepTuc) != 'Y') {
            printf("\nCam on ban va nhom da su dung chuong trinh!\n");
            break;
        }

    } while (1);

    return 0;
}