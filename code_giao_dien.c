#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

// 1. ĐỊNH NGHĨA CẤU TRÚC MỘT CÂU HỎI
struct CauHoi {
    char noiDung[300]; // Nội dung câu hỏi
    char a[150];       // Đáp án A
    char b[150];       // Đáp án B
    char c[150];       // Đáp án C
    char d[150];       // Đáp án D
    char dapAnDung;    // Đáp án đúng ('A', 'B', 'C', hoặc 'D')
    float diem;        // Điểm số của câu: 0.5, 1.0 hoặc 1.5
};

struct NguoiThi {
    char hoTen[56];
    char maSoSinhVien[30];
};

static HANDLE hOut;
static HANDLE hIn;
void phatAmThanh(const char *loai) {
    if (strcmp(loai, "sai") == 0) {
        Beep(440, 90);
        Beep(330, 150);
    } else if (strcmp(loai, "dung") == 0) {
        Beep(784, 55);
        Beep(988, 70);
        Beep(1175, 105);
    } else {
        Beep(880, 35);
    }
}

/* Bang mau giao dien: nen den, chu trang, vien vang va the hover mau vang. */
int mauNenToi(int mau) {
    if (mau == 0xF0) return 0xE0;
    if (mau == 0x9F) return 0x0E;
    if (mau == 0x1B || mau == 0x1E || mau == 0x16) return 0x0E;
    if (mau == 0x1F || mau == 0x17 || mau == 0x0F) return 0x0F;
    if (mau == 0x1C || mau == 0x14) return 0x0C;
    if (mau == 0x18) return 0x08;
    return mau & 0x0F;
}

void caiDatMau(int mau) {
    SetConsoleTextAttribute(hOut, (WORD)mauNenToi(mau));
}

void anConTro() {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);
}

void hienConTro() {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 25;
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &ci);
}

void toiViTri(int x, int y) {
    COORD p;
    p.X = (SHORT)x;
    p.Y = (SHORT)y;
    SetConsoleCursorPosition(hOut, p);
}

void toVung(int x, int y, int w, int mau) {
    COORD p;
    DWORD n;
    p.X = (SHORT)x;
    p.Y = (SHORT)y;
    FillConsoleOutputAttribute(hOut, (WORD)mauNenToi(mau), (DWORD)w, p, &n);
}

void toNen(int mauChuNen) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD goc;
    DWORD n;
    int tong;
    goc.X = 0;
    goc.Y = 0;
    GetConsoleScreenBufferInfo(hOut, &info);
    tong = info.dwSize.X * info.dwSize.Y;
    FillConsoleOutputCharacterA(hOut, ' ', (DWORD)tong, goc, &n);
    FillConsoleOutputAttribute(hOut, (WORD)mauNenToi(mauChuNen), (DWORD)tong, goc, &n);
    SetConsoleCursorPosition(hOut, goc);
    caiDatMau(mauChuNen);
}

void xoaManHinh() {
    toNen(0x0F); /* chu trang, nen den */
}

void batCheDoChuot() {
    DWORD mode = 0;
    GetConsoleMode(hIn, &mode);
    SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | (mode & ~ENABLE_QUICK_EDIT_MODE));
    GetConsoleMode(hIn, &mode);
    mode |= ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS;
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hIn, mode);
}

void khoiTaoConsole() {
    COORD buf;
    SMALL_RECT win;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleTitle("Ky thi trac nghiem  -  Mon Lap trinh C");
    /* Keep the screen buffer large enough for the fixed-position UI. */
    buf.X = 120;
    buf.Y = 50;
    SetConsoleScreenBufferSize(hOut, buf);
    win.Left = 0;
    win.Top = 0;
    win.Right = 119;
    win.Bottom = 39;
    SetConsoleWindowInfo(hOut, TRUE, &win);
    batCheDoChuot();
    anConTro();
}

/* Giao dien hien tai can toi thieu 120 cot va 40 dong. Neu cua so bi thu
   nho, doi su kien resize tiep theo thay vi ve cac khung bi cat/vo. */
void choConsoleDuKichThuoc() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    INPUT_RECORD rec;
    DWORD coSan, docDuoc;
    int rong, cao;

    while (1) {
        if (GetConsoleScreenBufferInfo(hOut, &info)) {
            rong = info.srWindow.Right - info.srWindow.Left + 1;
            cao = info.srWindow.Bottom - info.srWindow.Top + 1;
            if (rong >= 120 && cao >= 40) return;
        } else {
            return;
        }

        xoaManHinh();
        toiViTri(0, 0);
        caiDatMau(0x1F);
        printf("Cua so dang qua nho. Hay phong to toi thieu 120 cot x 40 dong.\n");
        printf("Nhan nut Maximize o goc cua so, hoac keo rong cua so de tiep tuc.");
        while (1) {
            /* Kiem tra kich thuoc lien tuc de khong bo lo su kien resize neu
               nguoi dung phong to dung luc thong bao dang hien ra. */
            if (GetConsoleScreenBufferInfo(hOut, &info)) {
                rong = info.srWindow.Right - info.srWindow.Left + 1;
                cao = info.srWindow.Bottom - info.srWindow.Top + 1;
                if (rong >= 120 && cao >= 40) break;
            } else {
                Sleep(50);
                continue;
            }

            if (PeekConsoleInput(hIn, &rec, 1, &coSan) && coSan > 0) {
                ReadConsoleInput(hIn, &rec, 1, &docDuoc);
            } else {
                Sleep(50);
            }
        }
    }
}

void inTai(int x, int y, int mau, const char *s) {
    /* Ham nay khong tu xuong dong; can can chieu dai chu theo rong khung. */
    toiViTri(x, y);
    caiDatMau(mau);
    printf("%s", s);
}

void inCauHoiXuongDong(int x, int y, int mau, const char *s) {
    /* Cau thuong cat toi 104 ky tu/dong, de nam trong khung rong 110 cot. */
    char dong[105];
    int soDong = 0;
    int dai;

    while (*s && soDong < 3) {
        dai = (int)strlen(s);
        if (dai > 104) {
            int cat = 104;
            while (cat > 0 && s[cat] != ' ') cat--;
            if (cat == 0) cat = 104;
            dai = cat;
        }
        memcpy(dong, s, dai);
        dong[dai] = '\0';
        inTai(x, y + soDong, mau, dong);
        s += dai;
        while (*s == ' ') s++;
        soDong++;
    }
}

void inCauHoiNguon(int x, int y, int mau, const char *s, int gioiHanDong) {
    /* Chuoi code co \n xuong dong ro rang; dong dai cat tiep, gioi han 15 dong.
       Khung code cao 18 dong, nen chu khong cham vao vien ben duoi. */
    char dong[105];
    int soDong = 0;
    int dai;

    while (*s && soDong < gioiHanDong) {
        dai = (int)strcspn(s, "\n");
        if (dai == 0) {
            s++;
            soDong++;
            continue;
        }
        while (dai > 104 && soDong < gioiHanDong) {
            memcpy(dong, s, 104);
            dong[104] = '\0';
            inTai(x, y + soDong, mau, dong);
            s += 104;
            dai = (int)strcspn(s, "\n");
            soDong++;
        }
        if (soDong < gioiHanDong && dai > 0) {
            memcpy(dong, s, dai);
            dong[dai] = '\0';
            inTai(x, y + soDong, mau, dong);
            s += dai;
            soDong++;
        }
        if (*s == '\n') s++;
    }
}

void inDong(int mau, const char *s) {
    caiDatMau(mau);
    printf("%s\n", s);
}

void veKhung(int x, int y, int w, int h, int mau) {
    /* Khung dung kich thuoc va toa do; noi dung ben trong can de thua le vien. */
    int i;
    char dong[130];
    if (w < 4) w = 4;
    if (w > 126) w = 126;
    dong[0] = '+';
    for (i = 1; i < w - 1; i++) dong[i] = '-';
    dong[w - 1] = '+';
    dong[w] = '\0';
    inTai(x, y, mau, dong);
    inTai(x, y + h - 1, mau, dong);
    dong[0] = '|';
    for (i = 1; i < w - 1; i++) dong[i] = ' ';
    dong[w - 1] = '|';
    for (i = 1; i < h - 1; i++) {
        inTai(x, y + i, mau, dong);
    }
}

void veThanhTienDo(int x, int y, int rong, int hienTai, int tong) {
    int i;
    int day;
    char so[24];
    if (tong <= 0) tong = 1;
    day = (hienTai * rong) / tong;
    if (day > rong) day = rong;
    toiViTri(x, y);
    caiDatMau(0x17);
    printf("[");
    for (i = 0; i < rong; i++) {
        if (i < day) {
            caiDatMau(0x9F);
            printf("=");
        } else {
            caiDatMau(0x17);
            printf(" ");
        }
    }
    caiDatMau(0x17);
    printf("]");
    sprintf(so, " %d/%d", hienTai, tong);
    caiDatMau(0x1F);
    printf("%s", so);
}

void veBannerDau(const char *phuDe) {
    veKhung(1, 0, 116, 5, 0x1B);
    inTai(4, 1, 0x1F, "C  /  KIEM TRA KIEN THUC");
    inTai(33, 1, 0x17, "|  LAP TRINH C");
    inTai(4, 2, 0x1B, phuDe);
    inTai(4, 3, 0x17, "Chon dap an bang chuot hoac phim A-D  .  Enter de xac nhan");
}

void veRongKhung(int khung) {
    inDong(8,  "");
    inDong(4,  "          .  ....                                          ");
    inDong(4,  "        .#@@@@@@#.        /\\      /\\                      ");
    inDong(12, "       #@@@@@@@@@@#      /@@\\____/@@\\     _                ");
    inDong(12, "      #@@@@@@@@@@@@#    /@@@@@@@@@@@@\\   / \\               ");
    inDong(4,  "       \\@@@/\\/\\@@@/    |@@@@ .    .@@@@| |  |              ");
    inDong(6,  "        \\@/    \\@/     |@@@  \\____/  @@@| |  |             ");
    inDong(12, "                       |@@ \\        / @@|  \\/              ");
    inDong(12, "                        \\@@ \\  __  / @@/                   ");
    inDong(6,  "                         \\@@ \\/  \\/ @@/                    ");
    inDong(14, "                          \\@@@@@@@@@@/                     ");
    inDong(6,  "                           |@@@@@@@@|                      ");
    inDong(12, "                          /@@@@@@@@@@\\                     ");
    inDong(4,  "                         /@@@@@@@@@@@@\\                    ");
    inDong(4,  "                        /@@@@@@@@@@@@@@\\                   ");

    caiDatMau(15);
    printf("\n                         \\  ");
    caiDatMau(12);
    printf("====");
    caiDatMau(14);
    printf("@@@@");
    caiDatMau(15);
    printf("####");
    if (khung == 1) {
        caiDatMau(14);
        printf("~~~~>\n");
        inDong(6,  "                           \\________/                      ");
    } else if (khung == 2) {
        caiDatMau(14);
        printf("~~~~~~~~");
        caiDatMau(12);
        printf("****");
        caiDatMau(14);
        printf("~~>\n");
        inDong(12, "                           \\________/     *  *  *          ");
        inDong(14, "                                         * ** ** *         ");
    } else {
        caiDatMau(14);
        printf("~~~~~~~~~~~~");
        caiDatMau(12);
        printf("########");
        caiDatMau(14);
        printf("****");
        caiDatMau(6);
        printf("~~~~>\n");
        inDong(14, "                           \\________/   * ** **** ** *     ");
        inDong(12, "                                       **  ********  **    ");
        inDong(6,  "                                      *  ************  *   ");
        inDong(14, "                                        ****  **  ****     ");
        inDong(12, "                                       *  *        *  *    ");
    }
}

void hienThongBaoLapLanh(const char *noiDung, int x, int y, int soNhip) {
    const int bangMau[] = {0x0C, 0x0E, 0x0B, 0x0D, 0x0F};
    int i;
    for (i = 0; i < soNhip; i++) {
        inTai(x, y, bangMau[i % 5], noiDung);
        Sleep(110);
    }
}

void hieuUngCupXoay() {
    static const char *khungCup[3][15] = {
        {
            "                 .--------.",
            "              .-'          '-.",
            "             /                \\",
            "            /                  \\",
            "           |                    |",
            "           |                    |",
            "            \\                  /",
            "             '.__            __.'",
            "                 |          |",
            "              ___|          |___",
            "             /                  \\",
            "            /____________________\\",
            "                /____________\\",
            "                   |______|",
            "                __|______|__"
        },
        {
            "                    .----.",
            "                   /      \\",
            "                  |        |",
            "                  |        |",
            "                   \\      /",
            "                    '.___.'",
            "                      ||",
            "                   ___||___",
            "                  /        \\",
            "                 /__________\\",
            "                    /____\\",
            "                      ||",
            "                  ___ || ___",
            "                 |__________|",
            ""
        },
        {
            "                 .--------.",
            "              .-'          '-.",
            "             /                \\",
            "            /                  \\",
            "           |                    |",
            "           |                    |",
            "            \\                  /",
            "             '.__            __.'",
            "                 |          |",
            "              ___|          |___",
            "             /                  \\",
            "            /____________________\\",
            "                /____________\\",
            "                   |______|",
            "                __|______|__"
        }
    };
    const int thuTu[6] = {0, 1, 2, 1, 0, 1};
    const int notNhac[6] = {784, 988, 1175, 988, 784, 1047};
    const int doDai[6] = {120, 120, 180, 120, 120, 260};
    int nhip, dong;

    for (nhip = 0; nhip < 6; nhip++) {
        xoaManHinh();
        veBannerDau("CHINH XAC  /  CUP VINH QUANG");
        inTai(42, 7, 0x1E, "      *** CHUC MUNG ***");
        for (dong = 0; dong < 15; dong++) {
            inTai(39, 9 + dong, 0x1E, khungCup[thuTu[nhip]][dong]);
        }
        inTai(35, 27, 0x1B, "       NGUOI CHIEN THANG       ");
        inTai(32, 30, 0x17, "      Tiep tuc phat huy nhe!      ");
        if (nhip == 0) phatAmThanh("dung");
        Sleep(doDai[nhip]);
    }
    hienThongBaoLapLanh("BAN DA TRA LOI DUNG!", 43, 25, 8);
    Sleep(350);
}

void hieuUngRongPhunLua() {
    int i;
    int nhip;
    const int notNhac[9] = {523, 523, 659, 784, 1047, 784, 1047, 1319, 1047};
    const int doDai[9] = {150, 90, 150, 190, 300, 130, 180, 450, 260};

    /* Canh chao mung: rong phun lua, phao hoa va chuong khuc chien thang. */
    for (nhip = 0; nhip < 2; nhip++) {
        xoaManHinh();
        veBannerDau("CHINH XAC  /  CHIEN THANG");
        toiViTri(0, 6);
        veRongKhung(nhip == 0 ? 1 : 2);
        inTai(82, 9, 0x1E, "*      .      *");
        inTai(84, 11, 0x1E, ".   *      .");
        inTai(82, 14, 0x1E, "*    .    *");
        if (nhip == 0) phatAmThanh("dung");
        Sleep(doDai[nhip]);
    }

    xoaManHinh();
    veBannerDau("CHINH XAC  /  CHIEN THANG");
    toiViTri(0, 6);
    veRongKhung(3);
    inTai(84, 9, 0x1E, "*   .   *   .");
    inTai(82, 12, 0x1E, ".   *   .   *");
    inTai(84, 15, 0x1E, "*   .   *");
    veKhung(27, 29, 68, 5, 0x1B);
    inTai(47, 30, 0x1E, "***  TUYET VOI!  ***");
    inTai(36, 32, 0x0F, "Dap an chinh xac - tiep tuc chinh phuc nao!");

    for (i = 2; i < 9; i++) {
        Beep(notNhac[i], doDai[i]);
        if (i == 3 || i == 6) Sleep(45);
    }
    hienThongBaoLapLanh("BAN DA TRA LOI DUNG!", 43, 27, 8);
    Sleep(450);
}

void inNoiDungDapAnSai(int x, int y, const char *noiDung) {
    char dong[70];
    int soDong = 0;

    while (*noiDung && soDong < 3) {
        int dai = (int)strlen(noiDung);
        if (dai > 68) {
            dai = 68;
            while (dai > 0 && noiDung[dai] != ' ') dai--;
            if (dai == 0) dai = 68;
        }
        memcpy(dong, noiDung, dai);
        dong[dai] = '\0';
        inTai(x, y + soDong, 0x1F, dong);
        noiDung += dai;
        while (*noiDung == ' ') noiDung++;
        soDong++;
    }
}

void hieuUngSai(char dapAnDung, const char *noiDungDapAnDung) {
    char dong[80];
    INPUT_RECORD suKien;
    DWORD soSuKien;
    xoaManHinh();
    veKhung(18, 11, 84, 13, 0x1C);
    hienThongBaoLapLanh("BAN DA TRA LOI SAI!", 48, 13, 8);
    sprintf(dong, "Dap an dung (%c):", dapAnDung);
    inTai(25, 16, 0x1F, dong);
    inNoiDungDapAnSai(25, 17, noiDungDapAnDung);
    inTai(31, 21, 0x17, "Nhan ENTER de xem cau tiep theo.");
    phatAmThanh("sai");
    FlushConsoleInputBuffer(hIn);
    while (1) {
        if (!ReadConsoleInput(hIn, &suKien, 1, &soSuKien) || soSuKien == 0) continue;
        if (suKien.EventType == KEY_EVENT
            && suKien.Event.KeyEvent.bKeyDown
            && suKien.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
            phatAmThanh("click");
            break;
        }
    }
}

void hieuUngHetGio(char dapAnDung, const char *noiDungDapAnDung) {
    char dong[80];
    INPUT_RECORD suKien;
    DWORD soSuKien;
    xoaManHinh();
    veKhung(18, 11, 84, 13, 0x1C);
    hienThongBaoLapLanh("HET GIO!", 52, 13, 8);
    sprintf(dong, "Dap an dung (%c):", dapAnDung);
    inTai(25, 16, 0x1F, dong);
    inNoiDungDapAnSai(25, 17, noiDungDapAnDung);
    inTai(31, 21, 0x17, "Nhan ENTER de xem cau tiep theo.");
    phatAmThanh("sai");
    FlushConsoleInputBuffer(hIn);
    while (1) {
        if (!ReadConsoleInput(hIn, &suKien, 1, &soSuKien) || soSuKien == 0) continue;
        if (suKien.EventType == KEY_EVENT
            && suKien.Event.KeyEvent.bKeyDown
            && suKien.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
            phatAmThanh("click");
            break;
        }
    }
}

void veTheDapAn(int y, char kyTu, const char *nd, int dangHover) {
    char dong[100];
    int mauChu;
    if (dangHover) {
        mauChu = 0xF0; /* chu den tren nen trang: sang nhe, dung hang */
        toVung(6, y, 72, mauChu);
        toVung(6, y + 1, 72, mauChu);
    } else {
        mauChu = 0x1F;
        toVung(6, y, 72, mauChu);
        toVung(6, y + 1, 72, mauChu);
    }
    sprintf(dong, "+   [%c]   %-56.56s+", kyTu, "");
    inTai(6, y, mauChu, dong);
    sprintf(dong, "|         %-59.59s|", nd);
    inTai(6, y + 1, mauChu, dong);
}

void veTheDapAnDayDu(int y, char kyTu, const char *nd, int dangHover) {
    const int rongNoiDung = 98;
    char dong[100];
    const char *conLai = nd;
    int mau = dangHover ? 0xF0 : 0x1F;
    int dongNoiDung;

    veKhung(6, y, 110, 5, dangHover ? 0x1E : 0x1B);
    if (dangHover) {
        toVung(7, y + 1, 108, mau);
        toVung(7, y + 2, 108, mau);
        toVung(7, y + 3, 108, mau);
    }
    sprintf(dong, "[%c]", kyTu);
    inTai(9, y + 1, mau, dong);

    for (dongNoiDung = 0; dongNoiDung < 2 && *conLai; dongNoiDung++) {
        int dai = (int)strlen(conLai);
        if (dai > rongNoiDung) {
            dai = rongNoiDung;
            while (dai > 0 && conLai[dai] != ' ') dai--;
            if (dai == 0) dai = rongNoiDung;
        }
        memcpy(dong, conLai, dai);
        dong[dai] = '\0';
        inTai(15, y + 1 + dongNoiDung, mau, dong);
        conLai += dai;
        while (*conLai == ' ') conLai++;
    }
}

void veHopXemTruoc(int y, const char *nhan, const char *noiDung, int dangChon) {
    int i;
    char dong[40];
    veKhung(82, y, 34, 8, dangChon ? 0x1B : 0x17);
    inTai(85, y, dangChon ? 0x1B : 0x17, " XEM TRUOC ");
    if (dangChon) {
        sprintf(dong, "Phuong an  %s", nhan);
        inTai(85, y + 2, 0x17, dong);
        sprintf(dong, "%-27.27s", noiDung);
        inTai(85, y + 4, 0x1F, dong);
        inTai(85, y + 6, 0x17, "Click de xac nhan");
    } else {
        inTai(85, y + 3, 0x17, "Di chuot vao dap an");
        inTai(85, y + 4, 0x17, "de xem truoc noi dung.");
        inTai(85, y + 6, 0x17, "A-D  /  ENTER");
    }
}

char chonDapAnBangChuot(struct CauHoi *ch, int soThuTu, int tongCau,
                        int soCauDung, DWORD batDauBai, DWORD tongThoiGianMs) {
    const char *dapAn[4];
    const char *nhan[4];
    int yThe[4];
    int hover = -1;
    INPUT_RECORD rec;
    DWORD docDuoc;
    int i;
    int cauHoiMaNguon;
    int giayDaHien = -1;
    DWORD msDaQua, msConLai;
    unsigned long giayConLai, phutConLai, giayLe;
    INPUT_RECORD suKienCho;
    char dongDongHo[40];
    char phuDe[80];

    dapAn[0] = ch->a;
    dapAn[1] = ch->b;
    dapAn[2] = ch->c;
    dapAn[3] = ch->d;
    nhan[0] = "A";
    nhan[1] = "B";
    nhan[2] = "C";
    nhan[3] = "D";
    cauHoiMaNguon = (strchr(ch->noiDung, '\n') != NULL);
    if (cauHoiMaNguon) {
        yThe[0] = 25;
        yThe[1] = 28;
        yThe[2] = 31;
        yThe[3] = 34;
    } else {
        yThe[0] = 11;
        yThe[1] = 17;
        yThe[2] = 23;
        yThe[3] = 29;
    }

veLaiCauHoi:
    choConsoleDuKichThuoc();
    xoaManHinh();
    batCheDoChuot();
    anConTro();

    sprintf(phuDe, "Phong thi  |  Cau %02d / %02d  |  Tong gio: %lu phut",
            soThuTu, tongCau, (unsigned long)((tongThoiGianMs + 59999UL) / 60000UL));
    veBannerDau(phuDe);
    inTai(80, 1, 0x17, "TIEN DO");
    veThanhTienDo(88, 1, 14, soThuTu - 1, tongCau);
    inTai(80, 2, 0x17, "TRA LOI DUNG");
    veThanhTienDo(88, 2, 14, soCauDung, tongCau);

    veKhung(6, 6, 110, cauHoiMaNguon ? 18 : 5, 0x1B);
    inTai(9, 6, 0x1B, " NOI DUNG CAU HOI ");
    if (cauHoiMaNguon) inCauHoiNguon(9, 7, 0x1F, ch->noiDung, 15);
    else inCauHoiXuongDong(9, 7, 0x1F, ch->noiDung);

    for (i = 0; i < 4; i++) {
        if (cauHoiMaNguon) veTheDapAn(yThe[i], (char)('A' + i), dapAn[i], 0);
        else veTheDapAnDayDu(yThe[i], (char)('A' + i), dapAn[i], 0);
    }
    if (cauHoiMaNguon) veHopXemTruoc(24, "", "", 0);
    inTai(6, cauHoiMaNguon ? 38 : 35, 0x17,
          "CHU THICH   Di chuot de xem truoc  |  Click hoac Enter de chot dap an");

    FlushConsoleInputBuffer(hIn);

    while (1) {
        msDaQua = GetTickCount() - batDauBai;
        if (msDaQua >= tongThoiGianMs) {
            inTai(80, 3, 0x0C, "HET GIO!          ");
            Beep(440, 180);
            Sleep(500);
            return 0;
        }

        msConLai = tongThoiGianMs - msDaQua;
        giayConLai = (unsigned long)((msConLai + 999) / 1000);
        phutConLai = giayConLai / 60;
        giayLe = giayConLai % 60;
        if ((int)giayConLai != giayDaHien) {
            if (giayConLai <= 30) {
                sprintf(dongDongHo, "SAP HET GIO: %02lu:%02lu", phutConLai, giayLe);
                inTai(80, 3, (giayConLai % 2) ? 0x0C : 0x0E, dongDongHo);
                if (giayConLai % 5 == 0) Beep(880, 45);
            } else {
                sprintf(dongDongHo, "THOI GIAN BAI: %02lu:%02lu", phutConLai, giayLe);
                inTai(80, 3, 0x0B, dongDongHo);
            }
            giayDaHien = (int)giayConLai;
        }

        if (!PeekConsoleInput(hIn, &suKienCho, 1, &docDuoc) || docDuoc == 0) {
            Sleep(25);
            continue;
        }
        if (!ReadConsoleInput(hIn, &rec, 1, &docDuoc) || docDuoc == 0) continue;

        if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            hover = -1;
            goto veLaiCauHoi;
        }

        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
            int vk = rec.Event.KeyEvent.wVirtualKeyCode;
            if (vk == VK_RETURN && hover >= 0) {
                phatAmThanh("click");
                return (char)('A' + hover);
            }
            if (vk >= 'A' && vk <= 'D') {
                phatAmThanh("click");
                return (char)vk;
            }
            if (vk >= 'a' && vk <= 'd') {
                phatAmThanh("click");
                return (char)(vk - 'a' + 'A');
            }
        }

        if (rec.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD m = rec.Event.MouseEvent;
            int mx = m.dwMousePosition.X;
            int my = m.dwMousePosition.Y;
            int moi = -1;

            if (mx >= 6 && mx <= (cauHoiMaNguon ? 77 : 115)) {
                for (i = 0; i < 4; i++) {
                    if (my >= yThe[i]
                        && my <= yThe[i] + (cauHoiMaNguon ? 1 : 4)) {
                        moi = i;
                        break;
                    }
                }
            }

            if (moi != hover) {
                hover = moi;
                for (i = 0; i < 4; i++) {
                    if (cauHoiMaNguon)
                        veTheDapAn(yThe[i], (char)('A' + i), dapAn[i], i == hover);
                    else
                        veTheDapAnDayDu(yThe[i], (char)('A' + i), dapAn[i], i == hover);
                }
                if (cauHoiMaNguon) {
                    if (hover >= 0) veHopXemTruoc(24, nhan[hover], dapAn[hover], 1);
                    else veHopXemTruoc(24, "", "", 0);
                }
            }

            if (moi >= 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                && m.dwEventFlags != MOUSE_MOVED) {
                phatAmThanh("click");
                return (char)('A' + moi);
            }
        }
    }
}

int choClickThe(int soThe, int yThe[], int hThe, int x1, int x2) {
    INPUT_RECORD rec;
    DWORD docDuoc;
    int hover = -1;
    int i;
    FlushConsoleInputBuffer(hIn);
    anConTro();
    batCheDoChuot();
    while (1) {
        if (!ReadConsoleInput(hIn, &rec, 1, &docDuoc) || docDuoc == 0) continue;

        if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) return -2;

        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
            int vk = rec.Event.KeyEvent.wVirtualKeyCode;
            if (vk == VK_RETURN && hover >= 0) {
                phatAmThanh("click");
                return hover;
            }
            if (vk >= '1' && vk <= '9') {
                phatAmThanh("click");
                return vk - '1';
            }
            if (vk == '0' || vk == VK_ESCAPE) return soThe - 1;
            if (vk == 'Y' || vk == 'y') {
                phatAmThanh("click");
                return 0;
            }
            if (vk == 'N' || vk == 'n') {
                phatAmThanh("click");
                return (soThe > 1) ? 1 : 0;
            }
        }

        if (rec.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD m = rec.Event.MouseEvent;
            int mx = m.dwMousePosition.X;
            int my = m.dwMousePosition.Y;
            int moi = -1;
            if (mx >= x1 && mx <= x2) {
                for (i = 0; i < soThe; i++) {
                    if (my >= yThe[i] && my < yThe[i] + hThe) {
                        moi = i;
                        break;
                    }
                }
            }
            if (moi != hover) {
                hover = moi;
                for (i = 0; i < soThe; i++) {
                    toVung(x1, yThe[i], x2 - x1 + 1, (i == hover) ? 0xF0 : 0x1F);
                    toVung(x1, yThe[i] + 1, x2 - x1 + 1, (i == hover) ? 0xF0 : 0x1F);
                }
            }
            if (moi >= 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                && m.dwEventFlags != MOUSE_MOVED) {
                phatAmThanh("click");
                return moi;
            }
        }
    }
}

int chonMenuBangChuot(void) {
    int yThe[3];
    int ketQua;
veLaiMenu:
    choConsoleDuKichThuoc();
    xoaManHinh();
    anConTro();
    veBannerDau("Trang chu  |  Chon hinh thuc thi");
    veKhung(16, 7, 88, 27, 0x1B);

    inTai(25, 9, 0x1F, "BAT DAU BAI KIEM TRA");
    inTai(25, 11, 0x17, "Chon mon thi. Moi mon gom 20 cau; de thi duoc tron theo muc do.");

    yThe[0] = 15;
    yThe[1] = 20;
    yThe[2] = 25;

    veKhung(25, 15, 70, 4, 0x1F);
    inTai(29, 16, 0x1F, "01   TRIET HOC MAC - LENIN       20 cau");

    veKhung(25, 20, 70, 4, 0x1F);
    inTai(29, 21, 0x1F, "02   LY THUYET LAP TRINH C       20 cau");

    veKhung(25, 25, 70, 4, 0x17);
    inTai(29, 26, 0x17, "ESC  THOAT         Ket thuc bai kiem tra");

    inTai(25, 31, 0x17, "DUNG PHIM 1 / 2 / 0 HOAC CLICK CHON MON THI");
    ketQua = choClickThe(3, yThe, 4, 25, 94);
    if (ketQua == -2) goto veLaiMenu;
    return ketQua;
}

void hieuUngPhaoHoaBenLe(void) {
    float x[2][48], y[2][48], vx[2][48], vy[2][48];
    const int tamX[2] = { 9, 110 };
    const int mauPhao[4] = { 0x0C, 0x0E, 0x0B, 0x0D };
    int ben, i, frame;
    char hat[2];

    Beep(1175, 55);
    for (ben = 0; ben < 2; ben++) {
        for (i = 0; i < 48; i++) {
            x[ben][i] = (float)tamX[ben];
            y[ben][i] = 16.0f;
            vx[ben][i] = (float)(rand() % 9 - 4) / 3.0f;
            vy[ben][i] = (float)(rand() % 7 - 5) / 3.0f;
        }
    }

    for (frame = 0; frame < 20; frame++) {
        for (ben = 0; ben < 2; ben++) {
            for (i = 0; i < 48; i++) {
                int px, py;
                x[ben][i] += vx[ben][i];
                y[ben][i] += vy[ben][i];
                vy[ben][i] += 0.12f;
                px = (int)(x[ben][i] + 0.5f);
                py = (int)(y[ben][i] + 0.5f);
                if (py < 8 || py > 24) continue;
                if (ben == 0 && (px < 3 || px > 15)) continue;
                if (ben == 1 && (px < 103 || px > 116)) continue;
                hat[0] = (frame < 7) ? '*' : ((i % 2) ? '+' : '.');
                hat[1] = '\0';
                inTai(px, py, mauPhao[(i + frame) % 4], hat);
            }
        }
        Sleep(40);
    }
    Beep(1568, 70);
}


void docThongTin(char *chuoi, int kichThuoc, int x, int y, int rong) {
    int dai, phim, gioiHan;
    char khoangTrang[64];
    DWORD daGhi;

    gioiHan = kichThuoc - 1;
    if (gioiHan > rong) gioiHan = rong;
    if (rong > (int)sizeof(khoangTrang)) rong = (int)sizeof(khoangTrang);
    memset(khoangTrang, ' ', rong);
    while (1) {
        dai = 0;
        chuoi[0] = '\0';
        while (1) {
            phim = getch();
            if (phim == 0 || phim == 224) {
                getch();
                continue;
            }
            if (phim == '\r') break;
            if (phim == '\b') {
                if (dai > 0) chuoi[--dai] = '\0';
            } else if (phim >= 32 && phim != 127 && dai < gioiHan) {
                chuoi[dai++] = (char)phim;
                chuoi[dai] = '\0';
            }

            toiViTri(x, y);
            caiDatMau(0x1F);
            if (dai > 0) WriteConsoleA(hOut, chuoi, (DWORD)dai, &daGhi, NULL);
            if (dai < rong)
                WriteConsoleA(hOut, khoangTrang, (DWORD)(rong - dai), &daGhi, NULL);
            toiViTri(x + dai, y);
        }

        while (dai > 0 && isspace((unsigned char)chuoi[dai - 1]))
            chuoi[--dai] = '\0';
        if (dai > 0) {
            int dau = 0;
            while (chuoi[dau] && isspace((unsigned char)chuoi[dau])) dau++;
            if (dau > 0) memmove(chuoi, chuoi + dau, strlen(chuoi + dau) + 1);
            inTai(25, 25, 0x0F, "                                                        ");
            return;
        }
        inTai(25, 25, 0x0C, "Thong tin khong duoc de trong. Hay nhap lai.");
        toiViTri(x, y);
    }
}

void nhapThongTinNguoiThi(struct NguoiThi *thiSinh) {
    DWORD inputMode;

    choConsoleDuKichThuoc();
    if (GetConsoleMode(hIn, &inputMode)) {
        inputMode |= ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT;
        inputMode &= ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
        SetConsoleMode(hIn, inputMode);
    }
    xoaManHinh();
    veBannerDau("Thong tin thi sinh");
    veKhung(18, 9, 84, 19, 0x1B);
    inTai(38, 11, 0x1E, "NHAP THONG TIN TRUOC KHI THI");
    inTai(25, 14, 0x17, "Thong tin nay se hien trong bang tong ket sau khi thi.");
    veKhung(25, 15, 70, 4, 0x17);
    inTai(28, 16, 0x1F, "Ho va ten:");
    toiViTri(42, 16);
    hienConTro();
    docThongTin(thiSinh->hoTen, (int)sizeof(thiSinh->hoTen), 42, 16, 50);
    veKhung(25, 20, 70, 4, 0x17);
    inTai(28, 21, 0x1F, "Ma so sinh vien:");
    toiViTri(47, 21);
    docThongTin(thiSinh->maSoSinhVien, (int)sizeof(thiSinh->maSoSinhVien), 47, 21, 45);
    anConTro();
    FlushConsoleInputBuffer(hIn);
    batCheDoChuot();
}


void hienThiXemLai(struct CauHoi deThi[], char cauTraLoi[], int soCauThi) {
    int i, j, key;
    char dong[120];
    for (i = 0; i < soCauThi; i++) {
        if (strchr(deThi[i].noiDung, '\n') != NULL) {
            xoaManHinh();
            veBannerDau("XEM LAI  |  NOI DUNG MA NGUON");
            veKhung(6, 6, 110, 29, 0x1B);
            sprintf(dong, "CAU %d / %d  |  MUC DIEM %.1f", i + 1, soCauThi, deThi[i].diem);
            inTai(10, 7, 0x1E, dong);
            inCauHoiNguon(10, 9, 0x1F, deThi[i].noiDung, 20);
            inTai(23, 34, 0x17, "Nhan phim bat ky de xem cac lua chon");
            FlushConsoleInputBuffer(hIn);
            key = getch();
            if (key == 0 || key == 224) getch();
        }
        xoaManHinh();
        veBannerDau("XEM LAI  |  DAP AN VA KET QUA");
        veKhung(6, 6, 110, 29, 0x1B);
        sprintf(dong, "CAU %d / %d  |  MUC DIEM %.1f", i + 1, soCauThi, deThi[i].diem);
        inTai(10, 7, 0x1E, dong);
        if (strchr(deThi[i].noiDung, '\n') == NULL)
            inCauHoiXuongDong(10, 9, 0x1F, deThi[i].noiDung);
        for (j = 0; j < 4; j++) {
            const char *luaChon;
            int y = 13 + j * 5;
            if (j == 0) luaChon = deThi[i].a;
            else if (j == 1) luaChon = deThi[i].b;
            else if (j == 2) luaChon = deThi[i].c;
            else luaChon = deThi[i].d;
            sprintf(dong, "%c.", 'A' + j);
            inTai(11, y, ('A' + j == deThi[i].dapAnDung) ? 0x0B : 0x17, dong);
            inCauHoiXuongDong(15, y, ('A' + j == deThi[i].dapAnDung) ? 0x0B : 0x1F, luaChon);
        }
        sprintf(dong, "BAN CHON: %c", cauTraLoi[i] == ' ' ? '-' : cauTraLoi[i]);
        inTai(13, 34, 0x1F, dong);
        sprintf(dong, "DAP AN DUNG: %c", deThi[i].dapAnDung);
        inTai(59, 34, cauTraLoi[i] == deThi[i].dapAnDung ? 0x0B : 0x0C, dong);
        inTai(27, 37, 0x17, i < soCauThi - 1 ? "Nhan phim bat ky de xem cau tiep theo" : "Nhan phim bat ky de quay ve");
        FlushConsoleInputBuffer(hIn);
        key = getch();
        if (key == 0 || key == 224) getch();
    }
}

int luaChonTiepTucGiaoDien(void) {
    int yThe[2] = {18, 25};
    int chon;
veLai:
    xoaManHinh();
    veBannerDau("HOAN TAT  |  BAN MUON LAM GI TIEP?");
    veKhung(20, 9, 80, 24, 0x1B);
    veKhung(31, 17, 58, 4, 0x1F);
    inTai(36, 18, 0x1F, "THI LAI");
    veKhung(31, 24, 58, 4, 0x17);
    inTai(36, 25, 0x17, "KET THUC");
    inTai(31, 30, 0x17, "Nhan 1/2 hoac click vao lua chon");
    chon = choClickThe(2, yThe, 4, 31, 88);
    if (chon == -2) goto veLai;
    return chon;
}

// 2. HÀM TRỘN ĐỀ THI NGẪU NHIÊN (Thuật toán Fisher-Yates cơ bản)
void tronDeThi(struct CauHoi ds[], int soLuong) {
    for (int i = 0; i < soLuong; i++) {
        int viTriNgauNhien = rand() % soLuong;
        struct CauHoi tam = ds[i];
        ds[i] = ds[viTriNgauNhien];
        ds[viTriNgauNhien] = tam;
    }
}

int main() {
    // Giao dien console Windows theo phong cach Dev-C++.
    struct NguoiThi thiSinh;
    srand((unsigned)time(NULL));
    khoiTaoConsole();
    nhapThongTinNguoiThi(&thiSinh);
    
    // 3. NGÂN HÀNG 40 CÂU HỎI TỔNG HỢP (KHÔNG DẤU TIẾNG VIỆT)
    // - 20 câu đầu (1- 20): Môn Triết học Mác - Lênin
    // - 20 câu sau (21 - 40): Môn Lập trình C (10 câu Dễ, 5 câu TB, 5 câu Khó)

    struct CauHoi nganHang[40] = {
        // 20 CÂU HỎI MÔN TRIẾT HỌC MÁC - LÊNIN
        // 10 câu Dễ (0.5đ)
        { "Van de co ban cua triet hoc la gi?", "Moi quan he giua tu duy va ton tai.", "Moi quan he giua con nguoi va tu nhien.", "Ban chat hien tuong xa hoi.", "Nguon goc the gioi dong vat.", 'A', 0.5f },
        { "Theo Ph.Angghen, van de co ban cua triet hoc gom may mat?", "1 mat.", "2 mat.", "3 mat.", "4 mat.", 'B', 0.5f },
        { "Khai niem \"vat chat\" theo dinh nghia cua V.I.Lenin la gi?", "Pham tru chi thuc tai khach quan.", "Tat ca vat the huu hinh.", "Khoi luong va nang luong.", "Cac hat nguyen tu.", 'A', 0.5f },
        { "Nguon goc tu nhien cua y thuc gom nhung yeu to nao?", "Bo nao nguoi va the gioi khach quan.", "Lao dong va ngon ngu.", "Tri thuc va tinh cam.", "Chi co bo nao nguoi.", 'A', 0.5f },
        { "Phuong thuc ton tai cua vat chat theo triet hoc Mac la gi?", "Khong gian.", "Thoi gian.", "Van dong.", "Y thuc.", 'C', 0.5f },
        { "Diem khac biet co ban giua duy vat va duy tam o mat thu nhat?", "Vat chat quyet dinh y thuc va nguoc lai.", "Tin than linh hay khong.", "Kha nang nhan thuc.", "Nghien cuu xa hoi hay tu nhien.", 'A', 0.5f },
        { "Phuong phap luan bien chung khac sieu hinh o diem nao?", "Xem xet trong lien he va phat trien.", "Xem xet co lap, tinh lang.", "Phu nhan mau thuan.", "Phat trien do ngoai luc.", 'A', 0.5f },
        { "Moi quan he giua vat chat va y thuc theo CNDV bien chung?", "Vat chat quyet dinh y thuc, y thuc tac dong tro lai.", "Y thuc quyet dinh hoan toan.", "Ton tai biet lap.", "Khong tac dong lan nhau.", 'A', 0.5f },
        { "Quy luat nao vach ra nguon goc, dong luc cua su phat trien?", "Quy luat luong - chat.", "Quy luat mau thuan.", "Quy luat phu dinh cua phu dinh.", "Quy luat quan he san xuat.", 'B', 0.5f },
        { "Nguon goc xa hoi nao quyet dinh truc tiep su ra doi cua y thuc?", "Lao dong.", "Ngon ngu.", "Nghe thuat.", "Giai tri.", 'A', 0.5f },
        
        // 5 câu Trung bình (1.0đ)
        { "Khang dinh con nguoi nhan thuc duoc the gioi thuoc mat nao?", "Mat ban the luan.", "Mat nhan thuc luan.", "Ca hai mat.", "Khong thuoc van de co ban.", 'B', 1.0f },
        { "Quan diem \"Su vat la tap hop cac cam giac\" thuoc truong phai nao?", "Duy vat sieu hinh.", "Duy tam khach quan.", "Duy tam chu quan.", "Nhi nguyen luan.", 'C', 1.0f },
        { "Quan niem xem xet su vat chuyen hoa lan nhau the hien phuong phap gi?", "Phuong phap sieu hinh.", "Phuong phap bien chung.", "Phuong phap nguy bien.", "Phuong phap chiet trung.", 'B', 1.0f },
        { "Quy luat nao giai thich viec sinh vien tich luy kien thuc de vuot ky thi?", "Quy luat mau thuan.", "Quy luat luong - chat.", "Quy luat phu dinh cua phu dinh.", "Tinh doc lap cua y thuc.", 'B', 1.0f },
        { "Tu nguyen ly lien he pho bien rut ra quan diem phuong phap luan nao?", "Toan dien va lich su - cu the.", "Phat trien thuan tuy.", "Phien dien, chiet trung.", "Chu quan, duy y chi.", 'A', 1.0f },
        
        // 5 câu Khó (1.5đ)
        { "Han che lon nhat cua chu nghia duy vat truoc Mac la gi?", "Sieu hinh va duy tam ve xa hoi.", "Phu nhan vat chat.", "Coi y thuc la ban nguyen.", "Tuyet doi hoa ton giao.", 'A', 1.5f },
        { "Vai tro cua Triet hoc Mac - Lenin doi voi con nguoi la gi?", "Cung cap the gioi quan va phuong phap luan khoa hoc.", "Thay the moi khoa hoc cu the.", "Dua ra loi giai san co.", "Chi co gia tri ly luan thuan tuy.", 'A', 1.5f },
        { "Khai niem \"phat trien\" khac \"van dong\" o diem co ban nao?", "Van dong di len, tu thap den cao.", "Bao gom ca su suy thoai.", "Chi tang giam ve luong.", "Lap lai hoan toan cai cu.", 'A', 1.5f },
        { "Nguyen tac phuong phap luan quan trong nhat tu moi quan he vat chat - y thuc?", "Ton trong khach quan, phat huy chu quan.", "Xuat phat tu y muon chu quan.", "Thu dong cho doi.", "Tuyet doi hoa ly thuyet.", 'A', 1.5f },
        { "Khuynh huong phat trien cua quy luat \"Phu dinh cua phu dinh\"?", "Duong thang tap.", "Duong tron khep kin.", "Duong xoay tron oc ke thua va phat trien.", "Duong dut doan ngau nhien.", 'C', 1.5f },
         
        //  20 CÂU HỎI MÔN LẬP TRÌNH C 
        // 10 câu Dễ (0.5đ)
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

        // 5 câu Trung bình (1.0đ)
        { "Neu switch thieu lenh break o case, hien tuong gi xay ra?", "Bao loi bien dich.", "Tu dong thoat.", "Chay xuyen sang cac case ben duoi (fall-through).", "Quay lai tu dau.", 'C', 1.0f },
        { "Duyet mang N phan tu theo chieu nguoc, vong lap for thiet lap ra sao?", "i = N, i > 0, giam i", "i = N - 1, i >= 0, giam i", "i = N - 1, i > 0, giam i", "i = N, i >= 0, giam i", 'B', 1.0f },
        { "Vong lap while neu dieu kien sai ngay tu dau chay may lan?", "0 lan.", "1 lan.", "Lap vo han.", "Phu thuoc du lieu.", 'A', 1.0f },
        { "Truy cap vuot qua chi so mang trong C gay ra hau qua gi?", "Tu dong mo rong mang.", "Bao loi cu phap luc bien dich.", "Loi truy cap o nho (Undefined Behavior).", "Gia tri tu dong ve 0.", 'C', 1.0f },
        { "Trong chuoi if - else if - else, C xu ly the nao?", "Kiem tra tat ca dieu kien.", "Kiem tra tuan tu, dung nhanh dau tien thi thuc thi roi dung.", "Chi kiem tra nhanh else.", "Danh gia ngau nhien.", 'B', 1.0f },

        // 5 câu Khó (1.5đ)
        { "Mang cuc bo khong khoi tao gia tri ban dau se chua gi?", "Gia tri 0.", "Gia tri rac (garbage values).", "Bao loi bien dich.", "Gia tri NULL.", 'B', 1.5f },
        { "Mang khai bao 10 phan tu nhung chi khoi tao 2 phan tu dau, phan tu con lai ra sao?", "Giu gia tri rac.", "Bao loi thieu so luong.", "Tu dong gan gia tri 0.", "Thu hep mang con 2.", 'C', 1.5f },
        { "Quy dinh gia tri nhan case trong switch la gi?", "Co the la so thuc float/double.", "Bat buoc la hang so nguyen hoac ky tu.", "Co the trung gia tri nhau.", "Nhan du lieu chuoi ky tu.", 'B', 1.5f },
        { "Truong hop nao dan den vong lap vo han trong C?", "For bo trong 3 bieu thuc.", "Dieu kien luon dung.", "Bien lap bien doi sai quy luat.", "Tat ca cac phuong an tren.", 'D', 1.5f },
        { "Doan code sau:\n"
          "#include <stdio.h>\n"
          "int main(void) {\n"
          "    int a[5] = {1, 2, 3, 4, 5}, S = 0;\n"
          "    for (int i = 0; i < 5; i++) {\n"
          "        if (a[i] % 2 == 0) S += a[i] * 2;\n"
          "        else S += a[i];\n"
          "    }\n"
          "    printf(\"%d\", S);\n"
          "    return 0;\n"
          "}\n"
          "Man hinh in ra ket qua nao?", "15", "21", "18", "30", 'B', 1.5f }
    };

    int luaChon;

    do {
        int chonMenu = chonMenuBangChuot();
        if (chonMenu == 2) {
            xoaManHinh();
            inTai(37, 17, 0x1F, "CAM ON BAN DA THAM GIA!");
            hienConTro();
            Sleep(700);
            break;
        }
        luaChon = chonMenu + 1;

        // Xác định vị trí bắt đầu trong mảng tổng (Triết từ 0, C từ 20)
        int viTriBatDau = (luaChon == 1) ? 0 : 20;
        char tenMon[50];
        sprintf(tenMon, "%s", (luaChon == 1) ? "Triet hoc Mac - Lenin" : "Ly thuyet Lap trinh C");

        // Tách 20 câu của môn thành 3 nhóm độ khó chuẩn: 10 câu Dễ, 5 câu TB, 5 câu Khó
        struct CauHoi nhomDe[10], nhomTB[5], nhomKho[5];
        int countDe = 0, countTB = 0, countKho = 0;

        for (int i = 0; i < 20; i++) {
            struct CauHoi q = nganHang[viTriBatDau + i];
            if (i < 10) {
                nhomDe[countDe++] = q;   // 10 câu đầu: Dễ (0.5 điểm)
            } else if (i < 15) {
                nhomTB[countTB++] = q;   // 5 câu tiếp: Trung bình (1.0 điểm)
            } else {
                nhomKho[countKho++] = q; // 5 câu cuối: Khó (1.5 điểm)
            }
        }

        // Trộn ngẫu nhiên từng nhóm độ khó
        tronDeThi(nhomDe, 10);
        tronDeThi(nhomTB, 5);
        tronDeThi(nhomKho, 5);

        // THUẬT TOÁN TẠO ĐỀ CÓ SỐ CÂU KHÔNG CỐ ĐỊNH NHƯNG TỔNG = 10Đ
        
        int loaiDe = rand() % 3; // Lấy ngẫu nhiên từ 0 đến 2
        int numDe = 0, numTB = 0, numKho = 0;

        if (loaiDe == 0) {
            numDe = 2; numTB = 3; numKho = 4; // Đề 8 câu
        } else if (loaiDe == 1) {
            numDe = 4; numTB = 2; numKho = 4; // Đề 10 câu
        } else {
            numDe = 8; numTB = 3; numKho = 2; // Đề 12 câu
        }

        int soCauThi = numDe + numTB + numKho; // Tổng số câu thực tế của đề
        struct CauHoi deThi[15];
        int idx = 0;

        for (int i = 0; i < numDe; i++) deThi[idx++] = nhomDe[i];
        for (int i = 0; i < numTB; i++) deThi[idx++] = nhomTB[i];
        for (int i = 0; i < numKho; i++) deThi[idx++] = nhomKho[i];

        // Trộn tổng thể lại để các câu Dễ/TB/Khó xen kẽ ngẫu nhiên trong đề thi
        tronDeThi(deThi, soCauThi);

        // Mảng lưu đáp án thí sinh đã chọn
        char cauTraLoi[15];
        for (int i = 0; i < soCauThi; i++) {
            cauTraLoi[i] = ' '; 
        }

        // 4. BẮT ĐẦU ĐẾM THỜI GIAN (20 phút = 1200 giây)
        time_t thoiGianBatDau = time(NULL);
        int thoiGianGioiHan = 1200;
        DWORD batDauBai = GetTickCount();
        DWORD gioiHanMs = 1200UL * 1000UL;
        int soCauDungTam = 0;

        for (int i = 0; i < soCauThi; i++) {
            const char *noiDungDapAnDung;
            if (deThi[i].dapAnDung == 'A') noiDungDapAnDung = deThi[i].a;
            else if (deThi[i].dapAnDung == 'B') noiDungDapAnDung = deThi[i].b;
            else if (deThi[i].dapAnDung == 'C') noiDungDapAnDung = deThi[i].c;
            else noiDungDapAnDung = deThi[i].d;

            char ans = chonDapAnBangChuot(&deThi[i], i + 1, soCauThi,
                                          soCauDungTam, batDauBai, gioiHanMs);
            if (ans == 0) {
                hieuUngHetGio(deThi[i].dapAnDung, noiDungDapAnDung);
                break;
            }
            cauTraLoi[i] = ans;
            if (ans == deThi[i].dapAnDung) {
                soCauDungTam++;
                if (rand() % 2 == 0) hieuUngRongPhunLua();
                else hieuUngCupXoay();
                hieuUngPhaoHoaBenLe();
            } else {
                hieuUngSai(deThi[i].dapAnDung, noiDungDapAnDung);
            }
        }
        // Tính thời gian làm bài thực tế
        time_t thoiGianKetThuc = time(NULL);
        double thoiGianLamBai = difftime(thoiGianKetThuc, thoiGianBatDau);
        if (thoiGianLamBai > thoiGianGioiHan) thoiGianLamBai = thoiGianGioiHan;
        int thoiGianConLaiCuoi = thoiGianGioiHan - (int)thoiGianLamBai;
        if (thoiGianConLaiCuoi < 0) thoiGianConLaiCuoi = 0;

        // 5. CHẤM ĐIỂM VÀ THỐNG KÊ KẾT QUẢ
        int soCauDung = 0;
        int soCauSai = 0;
        int soCauChuaLam = 0;
        float tongDiem = 0.0f;

        for (int i = 0; i < soCauThi; i++) {
            if (cauTraLoi[i] == ' ') {
                soCauChuaLam++;
            } else if (cauTraLoi[i] == deThi[i].dapAnDung) {
                soCauDung++;
                tongDiem += deThi[i].diem; // Cộng đúng số điểm tương ứng của câu đó
            } else {
                soCauSai++;
            }
        }

        float tyLeDung = ((float)soCauDung / (float)soCauThi) * 100.0f;

        {
            char dong[100];
            const char *xepLoai;
            if (tongDiem >= 8.5f) xepLoai = "Gioi";
            else if (tongDiem >= 7.0f) xepLoai = "Kha";
            else if (tongDiem >= 4.0f) xepLoai = "Trung binh";
            else xepLoai = "Chua dat";
            xoaManHinh();
            veBannerDau("Ket qua  |  Bang tong ket diem");
            veKhung(18, 6, 84, 27, 0x1B);
            inTai(44, 7, 0x1B, "KET QUA CUOC THI");
            sprintf(dong, "DIEM CUOI CUNG: %.2f / 10", tongDiem);
            inTai(40, 9, tongDiem >= 4.0f ? 0x0A : 0x0C, dong);
            sprintf(dong, "Xep loai: %s", xepLoai); inTai(47, 10, 0x0E, dong);
            sprintf(dong, "Trang thai: %s", tongDiem >= 4.0f ? "DAT" : "CHUA DAT");
            inTai(47, 11, tongDiem >= 4.0f ? 0x0A : 0x0C, dong);
            sprintf(dong, "Thi sinh: %.56s", thiSinh.hoTen); inTai(28, 12, 0x1F, dong);
            sprintf(dong, "Ma so sinh vien: %.29s", thiSinh.maSoSinhVien); inTai(28, 13, 0x1F, dong);
            sprintf(dong, "So cau dung       : %d / %d", soCauDung, soCauThi); inTai(28, 15, 0x1F, dong);
            sprintf(dong, "So cau sai        : %d / %d", soCauSai, soCauThi); inTai(28, 16, 0x1F, dong);
            sprintf(dong, "Cau chua lam      : %d", soCauChuaLam); inTai(28, 17, 0x1F, dong);
            sprintf(dong, "Ty le dung        : %.1f%%", tyLeDung); inTai(28, 18, 0x1F, dong);
            sprintf(dong, "Diem moi cau      : %.2f diem", 10.0f / soCauThi); inTai(28, 19, 0x1F, dong);
            sprintf(dong, "Thoi gian lam bai : %02d:%02d:%02d",
                    (int)thoiGianLamBai / 3600,
                    ((int)thoiGianLamBai % 3600) / 60,
                    (int)thoiGianLamBai % 60);
            inTai(28, 20, 0x17, dong);
            sprintf(dong, "Thoi gian con lai : %02d:%02d:%02d",
                    thoiGianConLaiCuoi / 3600,
                    (thoiGianConLaiCuoi % 3600) / 60,
                    thoiGianConLaiCuoi % 60);
            inTai(28, 21, 0x17, dong);
            sprintf(dong, "Mon thi: %s", tenMon); inTai(28, 22, 0x17, dong);
            inTai(28, 23, 0x17, "TIEN DO DIEM");
            veThanhTienDo(43, 23, 30, soCauDung, soCauThi);
            hieuUngPhaoHoaBenLe();
            inTai(30, 34, 0x17, "Nhan phim bat ky de xem tiep");
            FlushConsoleInputBuffer(hIn);
            {
                int phim = getch();
                if (phim == 0 || phim == 224) getch();
            }
        }
        {
            int yThe[2] = {18, 25};
            int xemLai;
            xoaManHinh();
            veBannerDau("XEM LAI  |  BAN CO MUON KIEM TRA BAI LAM?");
            veKhung(20, 9, 80, 24, 0x1B);
            veKhung(31, 17, 58, 4, 0x1F);
            inTai(36, 18, 0x1F, "XEM CHI TIET");
            veKhung(31, 24, 58, 4, 0x17);
            inTai(36, 25, 0x17, "BO QUA");
            xemLai = choClickThe(2, yThe, 4, 31, 88);
            if (xemLai == 0) hienThiXemLai(deThi, cauTraLoi, soCauThi);
        }
        if (luaChonTiepTucGiaoDien() != 0) {
            xoaManHinh();
            inTai(37, 17, 0x1F, "CAM ON BAN VA NHOM!");
            hienConTro();
            Sleep(700);
            break;
        }
    } while (1);

    return 0;
}
