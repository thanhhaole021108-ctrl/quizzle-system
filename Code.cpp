#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>  // HO TRO HAM TOUPPER() VA ISDIGIT()
#include <string.h> // HO TRO HAM STRLEN() DE KIEM TRA DO DAI MSSV

// 1. DINH NGHIA CAU TRUC MOT CAU HOI
struct CauHoi {
    char noiDung[300]; // NOI DUNG CAU HOI
    char a[150];       // DAP AN A
    char b[150];       // DAP AN B
    char c[150];       // DAP AN C
    char d[150];       // DAP AN D
    char dapAnDung;    // DAP AN DUNG A,B,C,D
    float diem;        // DIEM SO CUA CAU 0,5Đ 1Đ 1,5Đ
};

// 2. HAM TRON DE THI NGAU NHIEN (THUAT TOAN FISHER-YATES)
void tronDeThi(struct CauHoi ds[], int soLuong) {
    for (int i = 0; i < soLuong; i++) {
        int viTriNgauNhien = rand() % soLuong;
        struct CauHoi tam = ds[i];
        ds[i] = ds[viTriNgauNhien];
        ds[viTriNgauNhien] = tam;
    }
}

int main() {
    // KHOI TAO BO SINH SO NGAU NHIEN THEO THOI GIAN
    srand(time(NULL));
    
    // 3. NGAN HANG 40 CAU HOI
    struct CauHoi nganHang[40] = {
        // 20 CAU HOI MON TRIET HOC
        // 10 CAU DE (0.5đ)
        { "Van de co ban cua triet hoc la gi?", "Moi quan he giua tu duy va ton tai.", "Moi quan he giua con nguoi va tu nhien.", "Ban chat hien tuong xa hoi.", "Nguon goc the gioi dong vat.", 'A', 0.5f },
        { "Theo Ph.Angghen, van de co ban cua triet hoc gom may mat?", "1 mat.", "2 mat.", "3 mat.", "4 mat.", 'B', 0.5f },
        { "Khai niem \"vat chat\" theo dinh nghia cua V.I.Lenin la gi?", "Pham tru chi thuc tai khach quan.", "Tat ca vat the huu hinh.", "Khoi luong va nang luong.", "Cac hat nguyen tu.", 'A', 0.5f },
        { "Nguon goc tu nhien cua y thuc gom nhung yeu to nao?", "Bo nao nguoi va the gioi khach quan.", "Lao dong va ngon ngu.", "Tri thuc va tinh cam.", "Chi co bo nao nguoi.", 'A', 0.5f },
        { "Phuong thuc ton tai cua vat chat theo triet hoc Mac la gi?", "Khong gian.", "Thoi gian.", "Van dong.", "Y thuc.", 'C', 0.5f },
        { "Diem khac biet co ban giua duy vat va duy tam o mat thu nhat?", "Vat chat quyet dinh y thuc va nguoc lai.", "Tin than linh hay khong.", "Kha nang nhan thuc.", "Nghien cuu xa hoi hay tu nhien.", 'A', 0.5f },
        { "Phuong phap luan bien chung khac sieu hinh o diem nao?", "Xem xet trong lien he va phat trien.", "Xem xet co lap, tinh lang.", "Phu nhan mau thuan.", "Phat trien do ngoai luc.", 'A', 0.5f },
        { "Moi quan he giua vat chat va y thuc theo CNDV bien chung?", "Vat chat quyet dinh y thuc, y thuc tac dong tro lai.", "Y thuc quyet dinh hoan toan.", "Ton tai biet lap.", "Khong tac dong lan nhau.", 'A', 0.5f },
        { "Quy luat nao vach ra nguon goc, dong luc cua su phat trien?", "Quy luat luong - chat.", "Quy luat mau thuan.", "Quy luat phu dinh cua phu dinh.", "Quy luat quan he san xuat.", 'B', 0.5f },
        { "Nguon goc xa hoi nao quyet định truc tiep su ra doi cua y thuc?", "Lao dong.", "Ngon ngu.", "Nghe thuat.", "Giai tri.", 'A', 0.5f },
        
        // 5 CAU TB (1.0đ)
        { "Khang dinh con nguoi nhan thuc duoc the gioi thuoc mat nao?", "Mat ban the luan.", "Mat nhan thuc luan.", "Ca hai mat.", "Khong thuoc van de co ban.", 'B', 1.0f },
        { "Quan diem \"Su vat la tap hop cac cam giac\" thuoc truong phai nao?", "Duy vat sieu hinh.", "Duy tam khach quan.", "Duy tam chu quan.", "Nhi nguyen luan.", 'C', 1.0f },
        { "Quan niem xem xet su vat chuyen hoa lan nhau the hien phuong phap gi?", "Phuong phap sieu hinh.", "Phuong phap bien chung.", "Phuong phap nguy bien.", "Phuong phap chiet trung.", 'B', 1.0f },
        { "Quy luat nao giai thich viec sinh vien tich luy kien thuc de vuot ky thi?", "Quy luat mau thuan.", "Quy luat luong - chat.", "Quy luat phu dinh cua phu dinh.", "Tinh doc lap cua y thuc.", 'B', 1.0f },
        { "Tu nguyen ly lien he pho bien rut ra quan diem phuong phap luan nao?", "Toan dien va lich su - cu thể.", "Phat trien thuan tuy.", "Phien dien, chiet trung.", "Chu quan, duy y chi.", 'A', 1.0f },
        
        // 5 CAU KHO (1.5đ)
        { "Han che lon nhat cua chu nghia duy vat truoc Mac la gi?", "Sieu hinh va duy tam ve xa hoi.", "Phu nhan vat chat.", "Coi y thuc la ban nguyen.", "Tuyet doi hoa ton giao.", 'A', 1.5f },
        { "Vai tro cua Triet hoc Mac - Lenin doi voi con nguoi la gi?", "Cung cap the gioi quan va phuong phap luan khoa hoc.", "Thay the moi khoa hoc cu the.", "Dua ra loi giai san co.", "Chi co gia tri ly luan thuan tuy.", 'A', 1.5f },
        { "Khai niem \"phat trien\" khac \"van dong\" o diem co ban nao?", "Van dong di len, tu thap den cao.", "Bao gom ca su suy thoai.", "Chi tang giam ve luong.", "Lap lai hoan toan cai cu.", 'A', 1.5f },
        { "Nguyen tac phuong phap luan quan trong nhat tu moi quan he vat chat - y thuc?", "Ton trong khach quan, phat huy chu quan.", "Xuat phat tu y muon chu quan.", "Thu dong cho doi.", "Tuyet doi hoa ly thuyet.", 'A', 1.5f },
        { "Khuynh huong phat trien cua quy luat \"Phu dinh cua phu dinh\"?", "Duong thang tap.", "Duong tron khep kin.", "Duong xoay tron oc ke thua va phat trien.", "Duong dut doan ngau nhien.", 'C', 1.5f },
         
        // 20 CAU HOI MON LAP TRIH C
        // 10 CAU DE (0.5đ)
        { "Cau truc if-else trong C dung de lam gi?", "Lap lai khoi lenh.", "Lua chon nhanh theo dieu kien Dung/Sai.", "Khai bao mang.", "Ket thuc chuong trinh.", 'B', 0.5f },
        { "Tu khoa nao dung de thoat khoi switch?", "continue", "goto", "break", "return", 'C', 0.5f },
        { "Diem khac biet chinh giua do-while va while?", "do-while chay it nhat 1 lan truoc khi kiem tra.", "while khong can dieu kien.", "do-while chi chay khi sai.", "while kiem tra o cuoi.", 'A', 0.5f },
        { "Chi so (index) mang trong C bat dau tu may?", "1", "0", "-1", "Tuy chon.", 'B', 0.5f },
        { "Cau truc chuan cua vong lap for gom cac bieu thuc nao?", "(Khoi tao; Cap nhat; Dieu kien)", "(Dieu kien; Khoi tao; Cap nhat)", "(Khoi tao; Dieu kien; Cap nhat)", "(Cap nhat; Khoi tao; Dieu kien)", 'C', 0.5f },
        { "Nhanh default trong switch-case dong vai tro gi?", "Bat buoc o dau.", "Bat buoc phai co.", "Thuc thi khi khong trung khop case nao.", "Chuyen huong vong lap.", 'C', 0.5f },
        { "Lenh continue trong vong lap for co tac dung gi?", "Thoat vong lap.", "Bo qua luot hien tai, chuyen buoc lap tiep theo.", "Khoi dong lai tu dau.", "Tam dung chuong trinh.", 'B', 0.5f },
        { "Pham vi chi so hop le cua mang N phan tu la gi?", "Tu 1 den N", "Tu 0 den N", "Tu 0 den N - 1", "Tu 1 den N - 1", 'C', 0.5f },
        { "Bieu thuc trong if(bieu_thuc) duoc coi la Dung khi nao?", "Chi khi bang 1.", "Khi khac 0.", "La so nguyen duong.", "Khi bang 0.", 'B', 0.5f },
        { "Ten cua mot mang mot chieu dai dien cho dieu gi?", "Gia tri phan tu cuoi.", "Tong so phan tu.", "Dia chi o nho phan tu dau tien (index 0).", "Kich thuoc byte.", 'C', 0.5f },

        // 5 CAU TB (1.0đ)
        { "Neu switch thieu lenh break o case, hien tuong gi xay ra?", "Bao loi bien dich.", "Tu dong thoat.", "Chay xuyen sang cac case ben duoi (fall-through).", "Quay lai tu dau.", 'C', 1.0f },
        { "Duyet mang N phan tu theo chieu nguoc, vong lap for thiet lap ra sao?", "i = N, i > 0, giam i", "i = N - 1, i >= 0, giam i", "i = N - 1, i > 0, giam i", "i = N, i >= 0, giam i", 'B', 1.0f },
        { "Vong lap while neu dieu kien sai ngay tu dau chay may lan?", "0 lan.", "1 lan.", "Lap vo han.", "Phu thuoc du lieu.", 'A', 1.0f },
        { "Truy cap vuot qua chi so mang trong C gay ra hau qua gi?", "Tu dong mo rong mang.", "Bao loi cu phap luc bien dich.", "Loi truy cap o nho (Undefined Behavior).", "Gia tri tu dong ve 0.", 'C', 1.0f },
        { "Trong chuoi if - else if - else, C xu ly the nao?", "Kiem tra tat ca dieu kien.", "Kiem tra tuan tu, dung nhanh dau tien thi thuc thi roi dung.", "Chi kiem tra nhanh else.", "Danh gia ngau nhien.", 'B', 1.0f },

        // 5 CAU KHO (1.5đ)
        { "Mang cuc bo khong khoi tao gia tri ban dau se chua gi?", "Gia tri 0.", "Gia tri rac (garbage values).", "Bao loi bien dich.", "Gia tri NULL.", 'B', 1.5f },
        { "Mang khai bao 10 phan tu nhung chi khoi tao 2 phan tu dau, phan tu con lai ra sao?", "Giu gia tri rac.", "Bao loi thieu so luong.", "Tu dong gan gia tri 0.", "Thu hep mang con 2.", 'C', 1.5f },
        { "Quy dinh gia tri nhan case trong switch la gi?", "Co the la so thuc float/double.", "Bat buoc la hang so nguyen hoac ky tu.", "Co the trung gia tri nhau.", "Nhan du lieu chuoi ky tu.", 'B', 1.5f },
        { "Truong hop nao dan den vong lap vo han trong C?", "For bo trong 3 bieu thuc.", "Dieu kien luon dung.", "Bien lap bien doi sai quy luat.", "Tat ca cac phuong an tren.", 'D', 1.5f },
        { "Doan code:\n#include<stdio.h> \nint main()  \nint a[5]={1,2,3,4,5}, S=0;\nfor(int i=0;i<5;i++) \nif(a[i]%2==0) \nS+=a[i]%2==0 \nelse \nS+=a[i];\nprintf(\"%d\",S);\nreturn0;", "15", "21", "18", "30", 'B', 1.5f }
    };

    int luaChon;

    do {
        printf("\n==================================================\n");
        printf("            CHUONG TRINH THI TRAC NGHIEM          \n");
        printf("==================================================\n");
        printf(" 1. Mon Triet hoc Mac - Lenin \n");
        printf(" 2. Mon Ly thuyet Lap trinh C \n");
        printf(" 0. Thoat chuong trinh\n");
        printf("==================================================\n");
        printf(" Nhap lua chon cua ban (0-2): ");
        if (scanf("%d", &luaChon) != 1) {
            printf("Vui long nhap so hop le!\n");
            while(getchar() != '\n');
            continue;
        }

        if (luaChon == 0) {
            printf("\nCam on ban da su dung chuong trinh. Tam biet!\n");
            break;
        }

        if (luaChon != 1 && luaChon != 2) {
            printf("\n[Thong bao] Lua chon khong hop le! Vui long chon lai.\n");
            continue;
        }
        
        // NHAP HO TEN VA MSSV
    
        char hoTen[50];
        char mssv[20];
        
        // XOA BO NHO DEM TRUOC KHI DUNG GETS/SCANF CHUOI
        while(getchar() != '\n'); 

        printf("\n--------------------------------------------------\n");
        printf("            NHAP THONG TIN THI SINH               \n");
        printf("--------------------------------------------------\n");
        printf(" Nhap ho va ten cua ban (KHONG DAU): ");
        scanf("%[^\n]s", hoTen); // DOC CA HO TEN CO DAU CACH
        
        // KIEM TRA MSSV XEM CO PHAI DUNG 10 SO KHONG
        do {
            while(getchar() != '\n'); 
            
            printf(" Nhap ma so sinh vien (dung 10 chu so): ");
            scanf("%s", mssv);

            int doDai = strlen(mssv);
            int hopLe = 1; // GIA SU BAN DAU DUNG

            // KIEM TRA XEM DO DAI CÓ BANG 10 KHONG VA CAC KY TU CO PHAI LA SO KHONG
            if (doDai != 10) {
                hopLe = 0;
            } else {
                for (int i = 0; i < doDai; i++) {
                    if (!isdigit(mssv[i])) { // NEU KY TU KHONG PHAI SO
                        hopLe = 0;
                        break;
                    }
                }
            }

            if (hopLe) {
                break; 
            } else {
                printf("MSSV phai gom dung 10 chu so va khong chua ky tu khac! Vui long nhap lai.\n");
            }
        } while (1);

        // XAC DINH VI TRI BAT DAU TRONG MANG 
        int viTriBatDau = (luaChon == 1) ? 0 : 20;
        char tenMon[50];
        sprintf(tenMon, "%s", (luaChon == 1) ? "Triet hoc Mac - Lenin" : "Ly thuyet Lap trinh C");

        // TACH 20 CAU CUA MON THÀNH 3 MUC : 10 DE,5 TB,5 KHO
        struct CauHoi nhomDe[10], nhomTB[5], nhomKho[5];
        int countDe = 0, countTB = 0, countKho = 0;

        for (int i = 0; i < 20; i++) {
            struct CauHoi q = nganHang[viTriBatDau + i];
            if (i < 10) {
                nhomDe[countDe++] = q;   // 10 CAU DAU LA DE
            } else if (i < 15) {
                nhomTB[countTB++] = q;   // 5 CAU TIEP LA TB
            } else {
                nhomKho[countKho++] = q; // 5 CAU CUOI LA KHO
            }
        }

        // TRON NGAU NHIEN TUNG DO KHO
        tronDeThi(nhomDe, 10);
        tronDeThi(nhomTB, 5);
        tronDeThi(nhomKho, 5);

        // THUAT TOAN TAO DE KHONG CO DINH SO CAU NHUNG = 10Đ
        int loaiDe = rand() % 3; // LAY NGAU NHIEN TU 0 TOI 2
        int numDe = 0, numTB = 0, numKho = 0;

        if (loaiDe == 0) {
            numDe = 2; numTB = 3; numKho = 4; // DE 8 CAU
        } else if (loaiDe == 1) {
            numDe = 4; numTB = 2; numKho = 4; // DE 10 CAU
        } else {
            numDe = 8; numTB = 3; numKho = 2; // DE 12 CAU
        }

        int soCauThi = numDe + numTB + numKho; // TONG SO CAU THUC TE TRONG DE
        struct CauHoi deThi[15];
        int idx = 0;

        for (int i = 0; i < numDe; i++) deThi[idx++] = nhomDe[i];
        for (int i = 0; i < numTB; i++) deThi[idx++] = nhomTB[i];
        for (int i = 0; i < numKho; i++) deThi[idx++] = nhomKho[i];

        // TRON TONG THE LAI DE CAI CAU DE/TB/KHO XEN KE NGAU NHIEN TRONG DE THI
        tronDeThi(deThi, soCauThi);

        // MANG LUU DAP AN CUA THI SINH
        char cauTraLoi[15];
        for (int i = 0; i < soCauThi; i++) {
            cauTraLoi[i] = ' '; 
        }

        // 4. THOI GIAN
        time_t thoiGianBatDau = time(NULL);
        int thoiGianGioiHan = 1200; 

        printf("\n=> Bat dau bai thi cho thi sinh: %s (MSSV: %s)\n", hoTen, mssv);
        printf("=> Mon thi: %s | De thi gom %d cau (Tong diem: 10.0 diem). Thoi gian: 20 phut.\n", tenMon, soCauThi);

        for (int i = 0; i < soCauThi; i++) {
            time_t thoiGianHienTai = time(NULL);
            int thoiGianDaDung = (int)difftime(thoiGianHienTai, thoiGianBatDau);
            int thoiGianConLai = thoiGianGioiHan - thoiGianDaDung;

            if (thoiGianConLai <= 0) {
                printf("\n\n[ THONG BAO ] DA HET 20 PHUT! He thong tu dong nop bai.\n");
                break;
            }

            int phut = thoiGianConLai / 60;
            int giay = thoiGianConLai % 60;

            printf("\n--------------------------------------------------\n");
            printf("[Thoi gian con lai: %02d:%02d] | Cau %d/%d (Muc diem: %.1f diem):\n", phut, giay, i + 1, soCauThi, deThi[i].diem);
            printf("%s\n", deThi[i].noiDung);
            printf("  A. %s\n", deThi[i].a);
            printf("  B. %s\n", deThi[i].b);
            printf("  C. %s\n", deThi[i].c);
            printf("  D. %s\n", deThi[i].d);
            printf("Dap an cua ban (A/B/C/D): ");

            char ans;
            scanf(" %c", &ans);
            ans = toupper(ans);

            if (ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D') {
                cauTraLoi[i] = ans;
            } else {
                cauTraLoi[i] = ' '; 
            }
        }

        // TINH THOI GIAN LAM BAI THUC TE
        time_t thoiGianKetThuc = time(NULL);
        double thoiGianLamBai = difftime(thoiGianKetThuc, thoiGianBatDau);
        if (thoiGianLamBai > thoiGianGioiHan) thoiGianLamBai = thoiGianGioiHan;
        int thoiGianConLaiCuoi = thoiGianGioiHan - (int)thoiGianLamBai;
        if (thoiGianConLaiCuoi < 0) thoiGianConLaiCuoi = 0;

        // 5. CHAM DIEM VA THONG KE KET QUA
        int soCauDung = 0;
        int soCauSai = 0;
        int soCauChuaLam = 0;
        float tongDiem = 0.0f;

        for (int i = 0; i < soCauThi; i++) {
            if (cauTraLoi[i] == ' ') {
                soCauChuaLam++;
            } else if (cauTraLoi[i] == deThi[i].dapAnDung) {
                soCauDung++;
                tongDiem += deThi[i].diem; 
            } else {
                soCauSai++;
            }
        }

        float tyLeDung = ((float)soCauDung / (float)soCauThi) * 100.0f;

        printf("\n==================================================\n");
        printf("                    KET QUA LAM BAI               \n");
        printf("==================================================\n");
        printf("- Ho va ten             : %s\n", hoTen);
        printf("- Ma so sinh vien       : %s\n", mssv);
        printf("- Mon thi               : %s\n", tenMon);
        printf("- Thoi gian con lai     : %02d phut %02d giay\n", thoiGianConLaiCuoi / 60, thoiGianConLaiCuoi % 60);
        printf("- Tong so cau trong de  : %d cau\n", soCauThi);
        printf("- So cau tra loi dung   : %d\n", soCauDung);
        printf("- So cau tra loi sai    : %d\n", soCauSai);
        printf("- So cau chua lam       : %d\n", soCauChuaLam);
        printf("- Ty le dung            : %.1f%%\n", tyLeDung);
        printf("- Tong diem cua ban     : %.1f / 10.0 diem\n", tongDiem);
        printf("==================================================\n");

        // 6. TINH NANG XEM LAI BAI TUONH TAC
        char xemLai;
        printf("\nBan co muon xem lai chi tiet bai lam khong? (Y/N): ");
        scanf(" %c", &xemLai);
        if (toupper(xemLai) == 'Y') {
            printf("\n==================================================\n");
            printf("            CHE DO XEM LAI CHI TIET               \n");
            printf("==================================================\n");
            for (int i = 0; i < soCauThi; i++) {
                printf("\nCau %d (Muc diem: %.1f diem): %s\n", i + 1, deThi[i].diem, deThi[i].noiDung);
                printf("  A. %s\n", deThi[i].a);
                printf("  B. %s\n", deThi[i].b);
                printf("  C. %s\n", deThi[i].c);
                printf("  D. %s\n", deThi[i].d);

                if (cauTraLoi[i] == ' ') {
                    printf("-> Ban da chon : [ Chua tra loi ]\n");
                } else {
                    printf("-> Ban da chon : %c\n", cauTraLoi[i]);
                }
                printf("-> Dap an dung : %c\n", deThi[i].dapAnDung);

                if (cauTraLoi[i] == deThi[i].dapAnDung) {
                    printf("Ket qua        : [ DUNG ] (+%.1f diem)\n", deThi[i].diem);
                } else {
                    printf("Ket qua        : [ SAI HOAC CHUA LAM ]\n");
                }
                printf("--------------------------------------------------\n");
                
                if (i < soCauThi - 1) {
                    printf("Nhan phim bat ky va Enter de xem cau tiep theo...");
                    char enterTam;
                    scanf(" %c", &enterTam);
                }
            }
            printf("\nDa xem xong toan bo de thi.\n");
        }

        // HOI CO MUON THI TIEP KHONG
        char tiepTuc;
        printf("\nBan co muon tiep tuc su dung chuong trinh khong? (Y/N): ");
        scanf(" %c", &tiepTuc);
        if (toupper(tiepTuc) != 'Y') {
            printf("\nCam on ban va nhom da su dung chuong trinh. Tam biet!\n");
            break;
        }

    } while (1);

    return 0;
}