#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

/* Ky thi trac nghiem mon Lap trinh C
   Mo Code.c bang Dev-C++ roi bam F11 (Compile & Run).
*/

/* Luu ho ten va ma so sinh vien de hien lai o bang ket qua. */
struct NguoiThi {
    char hoTen[56];
    char maSoSinhVien[30];
};

/* Moi cau gom noi dung, 4 lua chon va dap an dung; mang rong tranh cat chuoi dai. */
struct CauHoi {
    char noiDung[1200];
    char a[1200];
    char b[1200];
    char c[1200];
    char d[1200];
    char dapAnDung;
};

static HANDLE hOut;
static HANDLE hIn;

/* Tao tieng beep bang API co san cua Windows, khong can file WAV. */
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
    DWORD docDuoc;
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
        FlushConsoleInputBuffer(hIn);
        while (ReadConsoleInput(hIn, &rec, 1, &docDuoc) && docDuoc > 0) {
            if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) break;
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

    sprintf(phuDe, "Phong thi  |  Cau %02d / %02d  |  Tong gio: %d phut",
            soThuTu, tongCau, (tongCau * 30 + 59) / 60);
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
    inTai(25, 11, 0x17, "Chon mon thi. Phan thi C gom dung 20 cau, theo thu tu tu 1 den 20.");

    yThe[0] = 15;
    yThe[1] = 20;
    yThe[2] = 25;

    veKhung(25, 15, 70, 4, 0x1F);
    inTai(29, 16, 0x1F, "01   TRIET HOC MAC - LENIN       10 cau");

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

int hoiTiepTucBangChuot(struct NguoiThi *thiSinh, int soCauThi, int soCauDung,
                        float phanTramDung, float tongDiem,
                        double thoiGianLamBai) {
    int yThe[2];
    char dong[90];
    int soCauSai = soCauThi - soCauDung;
    const char *xepLoai;
    int ketQua;

    if (tongDiem >= 8.5f) xepLoai = "Gioi";
    else if (tongDiem >= 7.0f) xepLoai = "Kha";
    else if (tongDiem >= 4.0f) xepLoai = "Trung binh";
    else xepLoai = "Chua dat";

veLaiKetQua:
    choConsoleDuKichThuoc();
    xoaManHinh();
    anConTro();
    veBannerDau("Ket qua  |  Bang tong ket diem");
    veKhung(18, 6, 80, 21, 0x1B);
    inTai(43, 7, 0x1B, "KET QUA CUOC THI");
    sprintf(dong, "DIEM CUOI CUNG:  %.2f / 10", tongDiem);
    inTai(39, 9, tongDiem >= 4.0f ? 0x0A : 0x0C, dong);
    sprintf(dong, "Xep loai: %s", xepLoai);
    inTai(47, 10, 0x0E, dong);
    sprintf(dong, "Trang thai: %s", tongDiem >= 4.0f ? "DAT" : "CHUA DAT");
    inTai(47, 11, tongDiem >= 4.0f ? 0x0A : 0x0C, dong);
    sprintf(dong, "Thi sinh: %.56s", thiSinh->hoTen);
    inTai(30, 12, 0x1F, dong);
    sprintf(dong, "Ma so sinh vien: %.29s", thiSinh->maSoSinhVien);
    inTai(30, 13, 0x1F, dong);
    sprintf(dong, "So cau dung        :  %d / %d", soCauDung, soCauThi);
    inTai(30, 15, 0x1F, dong);
    sprintf(dong, "So cau sai         :  %d / %d", soCauSai, soCauThi);
    inTai(30, 16, 0x1F, dong);
    sprintf(dong, "Ty le dung         :  %.1f%%", phanTramDung);
    inTai(30, 17, 0x1F, dong);
    sprintf(dong, "Diem moi cau       :  %.2f diem", 10.0f / soCauThi);
    inTai(30, 18, 0x1F, dong);
    sprintf(dong, "Thoi gian lam bai  :  %.0f giay", thoiGianLamBai);
    inTai(30, 19, 0x17, dong);
    inTai(30, 22, 0x17, "TIEN DO DIEM");
    veThanhTienDo(44, 22, 30, soCauDung, soCauThi);
    hieuUngPhaoHoaBenLe();

    yThe[0] = 28;
    yThe[1] = 32;
    veKhung(28, 28, 60, 3, 0x1F);
    inTai(34, 29, 0x1F, "Y.  Lam bai kiem tra moi");
    veKhung(28, 32, 60, 3, 0x17);
    inTai(34, 33, 0x17, "N.  Ket thuc");
    ketQua = choClickThe(2, yThe, 3, 28, 87);
    if (ketQua == -2) goto veLaiKetQua;
    return ketQua;
}

void tronDeThi(struct CauHoi ds[], int soLuong) {
    int i;
    /* Fisher-Yates: moi buoc doi phan tu hien tai voi mot vi tri chua xet. */
    for (i = soLuong - 1; i > 0; i--) {
        int viTriNgauNhien = rand() % (i + 1);
        struct CauHoi tam = ds[i];
        ds[i] = ds[viTriNgauNhien];
        ds[viTriNgauNhien] = tam;
    }
}

void tronDapAn(struct CauHoi *ch) {
    char *luaChon[4] = { ch->a, ch->b, ch->c, ch->d };
    char tam[1200];
    int viTriDung = ch->dapAnDung - 'A';
    int i;

    /* Tron 4 lua chon va cap nhat vi tri dap an dung cung luc. */
    for (i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        strcpy(tam, luaChon[i]);
        strcpy(luaChon[i], luaChon[j]);
        strcpy(luaChon[j], tam);

        if (viTriDung == i) viTriDung = j;
        else if (viTriDung == j) viTriDung = i;
    }
    ch->dapAnDung = (char)('A' + viTriDung);
}

int main() {
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
            "Nhan default trong cau lenh switch-case dong vai tro gi?", 
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
            "De duyet toan bo mot mang mot chieu N phan tu theo chieu nguoc tu phan tu cuoi cung ve phan tu dau tien, viec khoi tao va kiem tra bien dem i trong vong lap for phai thiet lap ra sao?", 
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
        /* Ghep nhieu string literal; ky tu \n tao dong moi cho doan code. */
        {
            "Cho doan code sau:\n"
            "#include <stdio.h>\n"
            "int main() {\n"
            "    int a[5] = {1, 2, 3, 4, 5};\n"
            "    int S = 0;\n"
            "    for (int i = 0; i < 5; i++) {\n"
            "        if (a[i] % 2 == 0) S += a[i] * 2;\n"
            "        else S += a[i];\n"
            "    }\n"
            "    printf(\"%d\", S);\n"
            "    return 0;\n"
            "}\n"
            "Man hinh se in ra ket qua nao sau day?",
            "15",
            "21",
            "18",
            "30",
            'B'
        }
    };

    struct CauHoi deThi[20];
    int luaChon;
    int soCauThi;
    int soCauDung;
    int i;
    char traLoi;
    struct NguoiThi thiSinh;
    DWORD batDauBai;
    DWORD tongThoiGianMs;
    time_t thoiGianBatDau;
    time_t thoiGianKetThuc;
    float diemMoiCau;
    float tongDiem;
    float phanTramDung;
    double thoiGianLamBai;

    /* Seed theo dong ho de thu tu tron thay doi giua cac lan thi. */
    srand((unsigned)time(NULL));
    khoiTaoConsole();
    nhapThongTinNguoiThi(&thiSinh);

    do {
        luaChon = chonMenuBangChuot();
        if (luaChon == 0) {
            soCauThi = 10;
            for (i = 0; i < soCauThi; i++) deThi[i] = nganHangTriet[i];
        } else if (luaChon == 1) {
            soCauThi = 20;
            for (i = 0; i < soCauThi; i++) deThi[i] = nganHangC[i];
        } else {
            xoaManHinh();
            veKhung(28, 14, 60, 7, 0x1B);
            inTai(38, 16, 0x1F, "Ky thi da ket thuc.");
            inTai(32, 18, 0x17, "Cam on thi sinh da tham du.");
            hienConTro();
            Sleep(800);
            return 0;
        }

        tronDeThi(deThi, soCauThi);
        for (i = 0; i < soCauThi; i++) tronDapAn(&deThi[i]);

        soCauDung = 0;
        tongThoiGianMs = (DWORD)soCauThi * 30000UL;
        batDauBai = GetTickCount();
        thoiGianBatDau = time(NULL);

        for (i = 0; i < soCauThi; i++) {
            traLoi = chonDapAnBangChuot(&deThi[i], i + 1, soCauThi,
                                        soCauDung, batDauBai, tongThoiGianMs);
            if (traLoi == 0) {
                const char *noiDungDapAnDung;
                if (deThi[i].dapAnDung == 'A') noiDungDapAnDung = deThi[i].a;
                else if (deThi[i].dapAnDung == 'B') noiDungDapAnDung = deThi[i].b;
                else if (deThi[i].dapAnDung == 'C') noiDungDapAnDung = deThi[i].c;
                else noiDungDapAnDung = deThi[i].d;
                hieuUngHetGio(deThi[i].dapAnDung, noiDungDapAnDung);
                break;
            }
            if (traLoi == deThi[i].dapAnDung) {
                soCauDung++;
                if (rand() % 2 == 0) hieuUngRongPhunLua();
                else hieuUngCupXoay();
                hieuUngPhaoHoaBenLe();
            } else {
                const char *noiDungDapAnDung;
                if (deThi[i].dapAnDung == 'A') noiDungDapAnDung = deThi[i].a;
                else if (deThi[i].dapAnDung == 'B') noiDungDapAnDung = deThi[i].b;
                else if (deThi[i].dapAnDung == 'C') noiDungDapAnDung = deThi[i].c;
                else noiDungDapAnDung = deThi[i].d;
                hieuUngSai(deThi[i].dapAnDung, noiDungDapAnDung);
            }
        }

        thoiGianKetThuc = time(NULL);
        thoiGianLamBai = difftime(thoiGianKetThuc, thoiGianBatDau);
        diemMoiCau = 10.0f / soCauThi;
        /* Tong diem tren thang 10; moi cau dung dong gop 10 / tong so cau. */
        /* Phan tram = so cau dung chia tong so cau, roi nhan 100. */
        phanTramDung = ((float)soCauDung / soCauThi) * 100.0f;
        tongDiem = (phanTramDung / 100.0f) * 10.0f;

        if (hoiTiepTucBangChuot(&thiSinh, soCauThi, soCauDung, phanTramDung,
                                tongDiem, thoiGianLamBai) != 0) {
            xoaManHinh();
            veKhung(28, 14, 60, 7, 0x1B);
            inTai(34, 16, 0x1F, "Cam on thi sinh va nhom.");
            inTai(36, 18, 0x17, "Ky thi trac nghiem C.");
            hienConTro();
            break;
        }
    } while (1);

    return 0;
}

