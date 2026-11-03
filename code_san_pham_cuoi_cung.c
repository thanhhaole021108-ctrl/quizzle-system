#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_CAU_THI 30

/* SHA-256 noi bo de khong can thu vien ngoai. */
typedef struct { unsigned int h[8]; unsigned long long bits; unsigned char block[64]; size_t used; } SHA256_CTX;
static const unsigned int shaK[64] = {
0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
static unsigned int rotr32(unsigned int x, unsigned int n) { return (x >> n) | (x << (32 - n)); }
static void sha256Block(SHA256_CTX *c, const unsigned char *p) {
    unsigned int w[64],a,b,d,e,f,g,h,t1,t2,cc; int i;
    for(i=0;i<16;i++) w[i]=((unsigned int)p[i*4]<<24)|((unsigned int)p[i*4+1]<<16)|((unsigned int)p[i*4+2]<<8)|p[i*4+3];
    for(i=16;i<64;i++){unsigned int s0=rotr32(w[i-15],7)^rotr32(w[i-15],18)^(w[i-15]>>3);unsigned int s1=rotr32(w[i-2],17)^rotr32(w[i-2],19)^(w[i-2]>>10);w[i]=w[i-16]+s0+w[i-7]+s1;}
    a=c->h[0];b=c->h[1];cc=c->h[2];d=c->h[3];e=c->h[4];f=c->h[5];g=c->h[6];h=c->h[7];
    for(i=0;i<64;i++){unsigned int S1=rotr32(e,6)^rotr32(e,11)^rotr32(e,25);unsigned int ch=(e&f)^(~e&g);t1=h+S1+ch+shaK[i]+w[i];unsigned int S0=rotr32(a,2)^rotr32(a,13)^rotr32(a,22);unsigned int maj=(a&b)^(a&cc)^(b&cc);t2=S0+maj;h=g;g=f;f=e;e=d+t1;d=cc;cc=b;b=a;a=t1+t2;}
    c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}
static void sha256Init(SHA256_CTX *c){static const unsigned int iv[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};memcpy(c->h,iv,sizeof(iv));c->bits=0;c->used=0;}
static void sha256Update(SHA256_CTX *c,const unsigned char *p,size_t n){size_t take;c->bits+=(unsigned long long)n*8;while(n){take=64-c->used;if(take>n)take=n;memcpy(c->block+c->used,p,take);c->used+=take;p+=take;n-=take;if(c->used==64){sha256Block(c,c->block);c->used=0;}}}
static void sha256Final(SHA256_CTX *c,unsigned char out[32]){int i;c->block[c->used++]=0x80;if(c->used>56){while(c->used<64)c->block[c->used++]=0;sha256Block(c,c->block);c->used=0;}while(c->used<56)c->block[c->used++]=0;for(i=7;i>=0;i--)c->block[c->used++]=(unsigned char)(c->bits>>(i*8));sha256Block(c,c->block);for(i=0;i<8;i++){out[i*4]=(unsigned char)(c->h[i]>>24);out[i*4+1]=(unsigned char)(c->h[i]>>16);out[i*4+2]=(unsigned char)(c->h[i]>>8);out[i*4+3]=(unsigned char)c->h[i];}}
static void sha256Hex(const unsigned char *data,size_t len,char hex[65]){static const char d[]="0123456789ABCDEF";unsigned char out[32];SHA256_CTX c;int i;sha256Init(&c);sha256Update(&c,data,len);sha256Final(&c,out);for(i=0;i<32;i++){hex[i*2]=d[out[i]>>4];hex[i*2+1]=d[out[i]&15];}hex[64]='\0';}
static void hmacSha256Hex(const char *key,const unsigned char *data,size_t len,char hex[65]){unsigned char k[64]={0},inner[32],pad[64];SHA256_CTX c;size_t kl=strlen(key);int i;if(kl>64){sha256Init(&c);sha256Update(&c,(const unsigned char*)key,kl);sha256Final(&c,k);kl=32;}else memcpy(k,key,kl);for(i=0;i<64;i++)pad[i]=(unsigned char)(k[i]^0x36);sha256Init(&c);sha256Update(&c,pad,64);sha256Update(&c,data,len);sha256Final(&c,inner);for(i=0;i<64;i++)pad[i]=(unsigned char)(k[i]^0x5c);sha256Init(&c);sha256Update(&c,pad,64);sha256Update(&c,inner,32);sha256Final(&c,inner);{static const char d[]="0123456789ABCDEF";for(i=0;i<32;i++){hex[i*2]=d[inner[i]>>4];hex[i*2+1]=d[inner[i]&15];}hex[64]='\0';}}

// 1. DINH NGHIA CAU TRUC MOT CAU HOI
struct CauHoi {
    char noiDung[300]; // Noi dung cau hoi
    char a[150];       // Dap an A
    char b[150];       // Dap an B
    char c[150];       // Dap an C
    char d[150];       // Dap an D
    char dapAnDung;    // Dap an dung ('A', 'B', 'C', hoac 'D')
    float diem;        // Diem so cua cau: 0.5, 1.0 hoac 1.5
};

struct NguoiThi {
    char hoTen[56];
    char maSoSinhVien[30];
};

static HANDLE hOut;
static HANDLE hIn;

void phatAmThanh(void) {
    Beep(880, 35);
}

/* Bang mau giao dien: nen den, chu trang, vien vang */
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
    toNen(0x0F); /* Chu trang, nen den */
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
    SetConsoleTitle("Ky thi trac nghiem  -  Mon Lap trinh C va Triet hoc");
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
    toiViTri(x, y);
    caiDatMau(mau);
    printf("%s", s);
}

void inCauHoiXuongDong(int x, int y, int mau, const char *s) {
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


void veKhung(int x, int y, int w, int h, int mau) {
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
    inTai(33, 1, 0x17, "|  HE THONG THI TRAC NGHIEM");
    inTai(4, 2, 0x1B, phuDe);
    inTai(4, 3, 0x17, "A-D: tra loi  |  Mui ten: truoc/sau  |  F: danh dau  |  ESC: nop bai");
}





enum LoaiThaoTacThi {
    THI_HET_GIO,
    THI_CHON_DAP_AN,
    THI_CAU_TRUOC,
    THI_CAU_SAU,
    THI_BAT_TAT_CO,
    THI_NHAY_CAU,
    THI_NOP_BAI
};

struct ThaoTacThi {
    enum LoaiThaoTacThi loai;
    int giaTri;
    char dapAn;
};

struct ThaoTacThi taoThaoTac(enum LoaiThaoTacThi loai, int giaTri, char dapAn) {
    struct ThaoTacThi thaoTac;
    thaoTac.loai = loai;
    thaoTac.giaTri = giaTri;
    thaoTac.dapAn = dapAn;
    return thaoTac;
}

void inNoiDungTheoCot(int x, int y, int mau, const char *s,
                      int rong, int soDong, int tachTheoTu) {
    char dong[130];
    int dongHienTai = 0;

    while (*s && dongHienTai < soDong) {
        int dai = (int)strcspn(s, "\n");
        int lay;
        if (dai == 0) {
            s++;
            dongHienTai++;
            continue;
        }
        while (dai > 0 && dongHienTai < soDong) {
            lay = dai > rong ? rong : dai;
            if (tachTheoTu && dai > rong) {
                while (lay > 0 && s[lay] != ' ') lay--;
                if (lay == 0) lay = rong;
            }
            memcpy(dong, s, lay);
            dong[lay] = '\0';
            inTai(x, y + dongHienTai, mau, dong);
            s += lay;
            dai -= lay;
            while (*s == ' ') { s++; dai--; }
            dongHienTai++;
        }
        if (*s == '\n') s++;
    }
}

void veDapAnBang(int y, int chieuCao, char kyTu, const char *noiDung,
                 int dangHover, int daDuocChon) {
    int mau = dangHover ? 0x1E : 0x1B;
    int mauNoiDung = daDuocChon ? 0x0B : (dangHover ? 0xF0 : 0x1F);
    char nhan[6];
    veKhung(6, y, 72, chieuCao, mau);
    sprintf(nhan, daDuocChon ? "[%c]*" : "[%c] ", kyTu);
    inTai(8, y + 1, mauNoiDung, nhan);
    inNoiDungTheoCot(14, y + 1, mauNoiDung,
                     noiDung, 60, chieuCao - 2, 1);
}

void veNutCauHoi(int x, int y, int soCau, int hienTai,
                 char dapAn, int coDanhDau) {
    char nhan[8];
    int mau;

    if (coDanhDau) mau = 0x0E;
    else if (dapAn != ' ') mau = 0x0A;
    else mau = 0x0F;
    if (hienTai) mau = 0x0B;

    sprintf(nhan, "[%02d%c]", soCau + 1,
            coDanhDau ? '!' : (dapAn != ' ' ? '+' : ' '));
    toVung(x, y, 7, mau);
    inTai(x, y, mau, nhan);
}

void veBangCauHoi(int soCauThi, int cauHienTai,
                  const char cauTraLoi[], const int coDanhDau[]) {
    int i;
    veKhung(78, 6, 39, 33, 0x1B);
    inTai(81, 7, 0x1F, "BANG CAU HOI");
    for (i = 0; i < soCauThi; i++) {
        int cot = i % 4;
        int dong = i / 4;
        veNutCauHoi(80 + cot * 9, 9 + dong * 3, i, i == cauHienTai,
                    cauTraLoi[i], coDanhDau[i]);
    }
    inTai(80, 20, 0x0B, "> Cau hien tai");
    inTai(80, 21, 0x0A, "+ Da tra loi");
    inTai(80, 22, 0x0E, "! Co danh dau");
    inTai(80, 23, 0x0F, "  Chua tra loi");
    veKhung(80, 24, 17, 3, 0x1F);
    veKhung(98, 24, 18, 3, 0x1F);
    inTai(80, 25, 0x1F, "< CAU TRUOC");
    inTai(98, 25, 0x1F, "CAU SAU >");
    veKhung(80, 27, 36, 3, coDanhDau[cauHienTai] ? 0x0E : 0x17);
    inTai(80, 28, coDanhDau[cauHienTai] ? 0x0E : 0x17,
          coDanhDau[cauHienTai] ? "BO DANH DAU [F]" : "DANH DAU [F]");
    veKhung(80, 30, 36, 3, 0x0C);
    inTai(80, 31, 0x0C, "NOP BAI SOM [ESC]");
    inTai(80, 34, 0x17, "Click so cau de nhay");
    inTai(80, 35, 0x17, "Phim 1-9, 0 = cau 10");
    inTai(80, 36, 0x17, "G + so cau + Enter: nhay");
}

struct ThaoTacThi chonTrongBangCauHoi(struct CauHoi deThi[], int soCauThi,
        int cauHienTai, char cauTraLoi[], int coDanhDau[],
        DWORD batDauBai, DWORD tongThoiGianMs) {
    const int yTheThuong[4] = {11, 17, 23, 29};
    const int yTheMa[4] = {25, 28, 31, 34};
    const char *dapAn[4];
    INPUT_RECORD rec, suKienCho;
    DWORD docDuoc;
    DWORD msDaQua, msConLai;
    unsigned long giayConLai, phutConLai, giayLe;
    char dongDongHo[40], phuDe[80];
    int cauHoiMaNguon = strchr(deThi[cauHienTai].noiDung, '\n') != NULL;
    int yThe[4], hover = -1, i;
    int dangNhapSo = 0, soDangNhap = 0;

    dapAn[0] = deThi[cauHienTai].a;
    dapAn[1] = deThi[cauHienTai].b;
    dapAn[2] = deThi[cauHienTai].c;
    dapAn[3] = deThi[cauHienTai].d;
    if (cauTraLoi[cauHienTai] >= 'A' && cauTraLoi[cauHienTai] <= 'D')
        hover = cauTraLoi[cauHienTai] - 'A';
    for (i = 0; i < 4; i++) yThe[i] = cauHoiMaNguon ? yTheMa[i] : yTheThuong[i];

veLaiManThi:
    choConsoleDuKichThuoc();
    xoaManHinh();
    batCheDoChuot();
    anConTro();

    sprintf(phuDe, "Phong thi  |  Cau %02d / %02d  |  Tong gio: %lu phut",
            cauHienTai + 1, soCauThi,
            (unsigned long)((tongThoiGianMs + 59999UL) / 60000UL));
    veBannerDau(phuDe);
    inTai(80, 1, 0x17, "TIEN DO");
    veThanhTienDo(88, 1, 14, cauHienTai + 1, soCauThi);

    veKhung(6, 6, 72, cauHoiMaNguon ? 18 : 5, 0x1B);
    inTai(8, 6, 0x1B, " NOI DUNG CAU HOI ");
    inNoiDungTheoCot(8, 7, 0x1F, deThi[cauHienTai].noiDung,
                     65, cauHoiMaNguon ? 16 : 3, !cauHoiMaNguon);
    for (i = 0; i < 4; i++)
        veDapAnBang(yThe[i], cauHoiMaNguon ? 3 : 5,
                    (char)('A' + i), dapAn[i], i == hover,
                    cauTraLoi[cauHienTai] == (char)('A' + i));
    veBangCauHoi(soCauThi, cauHienTai, cauTraLoi, coDanhDau);

    FlushConsoleInputBuffer(hIn);
    while (1) {
        msDaQua = GetTickCount() - batDauBai;
        if (msDaQua >= tongThoiGianMs) {
            inTai(80, 3, 0x0C, "HET GIO!          ");
            Beep(440, 180);
            Sleep(500);
            return taoThaoTac(THI_HET_GIO, 0, ' ');
        }

        msConLai = tongThoiGianMs - msDaQua;
        giayConLai = (unsigned long)((msConLai + 999) / 1000);
        phutConLai = giayConLai / 60;
        giayLe = giayConLai % 60;
        sprintf(dongDongHo, giayConLai <= 30 ? "SAP HET GIO: %02lu:%02lu" :
                "THOI GIAN BAI: %02lu:%02lu", phutConLai, giayLe);
        inTai(80, 3, giayConLai <= 30 ? 0x0C : 0x0B, dongDongHo);

        if (!PeekConsoleInput(hIn, &suKienCho, 1, &docDuoc) || docDuoc == 0) {
            Sleep(25);
            continue;
        }
        if (!ReadConsoleInput(hIn, &rec, 1, &docDuoc) || docDuoc == 0) continue;

        if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) goto veLaiManThi;

        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
            int vk = rec.Event.KeyEvent.wVirtualKeyCode;
            if (dangNhapSo && vk >= '0' && vk <= '9') {
                soDangNhap = soDangNhap * 10 + (vk - '0');
                if (soDangNhap > soCauThi) { dangNhapSo = 0; soDangNhap = 0; }
                continue;
            }
            if (vk == 'G' || vk == 'g') {
                dangNhapSo = 1;
                soDangNhap = 0;
                inTai(80, 36, 0x0E, "Nhap so cau roi Enter");
                continue;
            }
            if (vk == VK_RETURN && dangNhapSo) {
                if (soDangNhap >= 1 && soDangNhap <= soCauThi)
                    return taoThaoTac(THI_NHAY_CAU, soDangNhap - 1, ' ');
                dangNhapSo = 0;
                continue;
            }
            if (vk == VK_LEFT || vk == 'P' || vk == 'p')
                return taoThaoTac(THI_CAU_TRUOC, 0, ' ');
            if (vk == VK_RIGHT || vk == 'N' || vk == 'n')
                return taoThaoTac(THI_CAU_SAU, 0, ' ');
            if (vk == 'F' || vk == 'f')
                return taoThaoTac(THI_BAT_TAT_CO, 0, ' ');
            if (vk == VK_ESCAPE)
                return taoThaoTac(THI_NOP_BAI, 0, ' ');
            if (vk >= '1' && vk <= '9' && vk - '1' < soCauThi)
                return taoThaoTac(THI_NHAY_CAU, vk - '1', ' ');
            if (vk == '0' && soCauThi >= 10)
                return taoThaoTac(THI_NHAY_CAU, 9, ' ');
            if (vk == VK_RETURN && hover >= 0) {
                phatAmThanh();
                return taoThaoTac(THI_CHON_DAP_AN, 0, (char)('A' + hover));
            }
            if (vk >= 'A' && vk <= 'D') {
                phatAmThanh();
                return taoThaoTac(THI_CHON_DAP_AN, 0, (char)vk);
            }
            if (vk >= 'a' && vk <= 'd') {
                phatAmThanh();
                return taoThaoTac(THI_CHON_DAP_AN, 0, (char)(vk - 'a' + 'A'));
            }
        }

        if (rec.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD m = rec.Event.MouseEvent;
            int mx = m.dwMousePosition.X;
            int my = m.dwMousePosition.Y;
            int moi = -1;
            int bam = (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                    && m.dwEventFlags != MOUSE_MOVED;

            if (bam && my >= 9 && my <= 18 && (my - 9) % 3 == 0
                && mx >= 80 && mx <= 114 && (mx - 80) % 9 < 7) {
                int cot = (mx - 80) / 9;
                int dong = (my - 9) / 3;
                int target = dong * 4 + cot;
                if (target < soCauThi)
                    return taoThaoTac(THI_NHAY_CAU, target, ' ');
            }
            if (bam && my >= 24 && my <= 26) {
                if (mx >= 80 && mx <= 96) return taoThaoTac(THI_CAU_TRUOC, 0, ' ');
                if (mx >= 98 && mx <= 115) return taoThaoTac(THI_CAU_SAU, 0, ' ');
            }
            if (bam && my >= 27 && my <= 29 && mx >= 80 && mx <= 115)
                return taoThaoTac(THI_BAT_TAT_CO, 0, ' ');
            if (bam && my >= 30 && my <= 32 && mx >= 80 && mx <= 115)
                return taoThaoTac(THI_NOP_BAI, 0, ' ');

            if (mx >= 6 && mx <= 77) {
                for (i = 0; i < 4; i++) {
                    int h = cauHoiMaNguon ? 2 : 4;
                    if (my >= yThe[i] && my <= yThe[i] + h) { moi = i; break; }
                }
            }
            if (moi != hover) {
                hover = moi;
                for (i = 0; i < 4; i++)
                    veDapAnBang(yThe[i], cauHoiMaNguon ? 3 : 5,
                                (char)('A' + i), dapAn[i], i == hover,
                                cauTraLoi[cauHienTai] == (char)('A' + i));
            }
            if (bam && moi >= 0) {
                phatAmThanh();
                return taoThaoTac(THI_CHON_DAP_AN, 0, (char)('A' + moi));
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
                phatAmThanh();
                return hover;
            }
            if (vk >= '1' && vk <= '9') {
                phatAmThanh();
                return vk - '1';
            }
            if (vk == '0' || vk == VK_ESCAPE) return soThe - 1;
            if (vk == 'Y' || vk == 'y') {
                phatAmThanh();
                return 0;
            }
            if (vk == 'N' || vk == 'n') {
                phatAmThanh();
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
                phatAmThanh();
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
    inTai(25, 11, 0x17, "Chon mon thi. De thi duoc xao tron ngau nhien theo do kho.");

    yThe[0] = 15;
    yThe[1] = 20;
    yThe[2] = 25;

    veKhung(25, 15, 70, 4, 0x1F);
    inTai(29, 16, 0x1F, "01   TRIET HOC MAC - LENIN       Thang diem 10");

    veKhung(25, 20, 70, 4, 0x1F);
    inTai(29, 21, 0x1F, "02   LY THUYET LAP TRINH C       Thang diem 10");

    veKhung(25, 25, 70, 4, 0x17);
    inTai(29, 26, 0x17, "ESC  THOAT                       Ket thuc chuong trinh");

    inTai(25, 31, 0x17, "DUNG PHIM 1 / 2 / ESC HOAC CLICK CHON MON THI");
    ketQua = choClickThe(3, yThe, 4, 25, 94);
    if (ketQua == -2) goto veLaiMenu;
    return ketQua;
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
            inTai(25, 25, 0x0F, "                                                            ");
            return;
        }
        inTai(25, 25, 0x0C, "Thong tin khong duoc de trong. Hay nhap lai.                ");
        toiViTri(x, y);
    }
}

void nhapThongTinNguoiThi(struct NguoiThi *thiSinh) {
    DWORD inputMode;
    int i, hopLe;

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
    do {
        toiViTri(47, 21);
        docThongTin(thiSinh->maSoSinhVien, (int)sizeof(thiSinh->maSoSinhVien), 47, 21, 10);
        hopLe = (strlen(thiSinh->maSoSinhVien) == 10);
        for (i = 0; hopLe && i < 10; i++) {
            if (!isdigit((unsigned char)thiSinh->maSoSinhVien[i]))
                hopLe = 0;
        }
        if (!hopLe) {
            inTai(25, 25, 0x0C, "MSSV phai gom dung 10 chu so. Hay nhap lai!                 ");
        } else {
            inTai(25, 25, 0x0F, "                                                            ");
        }
    } while (!hopLe);
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

// 2. HAM TRON DE THI NGAU NHIEN (Thuat toan Fisher-Yates)
void tronDeThi(struct CauHoi ds[], int soLuong) {
    int i;
    for (i = 0; i < soLuong; i++) {
        int viTriNgauNhien = rand() % soLuong;
        struct CauHoi tam = ds[i];
        ds[i] = ds[viTriNgauNhien];
        ds[viTriNgauNhien] = tam;
    }
}

void xoaPhaoHoaHaiBen(void) {
    int x, y;
    for (y = 10; y <= 35; y++) {
        for (x = 3; x <= 15; x++) inTai(x, y, 0x0F, " ");
        for (x = 104; x <= 117; x++) inTai(x, y, 0x0F, " ");
    }
}

void veCumPhaoHoa(int tamX, int tamY, int banKinh, int mau) {
    static const int huongX[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    static const int huongY[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int i;
    inTai(tamX, tamY, mau, ".");
    for (i = 0; i < 8; i++) {
        int x = tamX + huongX[i] * banKinh;
        int y = tamY + huongY[i] * banKinh;
        inTai(x, y, mau, "*");
        if (banKinh > 1)
            inTai(tamX + huongX[i] * (banKinh - 1),
                  tamY + huongY[i] * (banKinh - 1), 0x0E, "+");
    }
}

void hieuUngPhaoHoaBenLe(void) {
    static const int mauSac[4] = {0x0C, 0x0E, 0x0B, 0x0D};
    int nhip;
    for (nhip = 0; nhip < 4; nhip++) {
        int r = nhip + 1;
        xoaPhaoHoaHaiBen();
        veCumPhaoHoa(9, 16, r, mauSac[nhip]);
        veCumPhaoHoa(111, 16, r, mauSac[(nhip + 1) % 4]);
        veCumPhaoHoa(9, 28, r, mauSac[(nhip + 2) % 4]);
        veCumPhaoHoa(111, 28, r, mauSac[(nhip + 3) % 4]);
        Sleep(140);
    }
}

#define ADMIN_HASH_MAC_DINH "E86F78A8A3CAF0B60D8E74E5942AA6D86DC150CD3C03338AEF25B7D2D7E3ACC7"
#define KHOA_KY_PHIEU "PHIEU_THI_SHA256_LOCAL_V1_7b1dc143"

void docMatKhauAdmin(char *matKhau, int maxLen) {
    int i=0, ch; hienConTro();
    while((ch=getch())!='\r') {
        if(ch==8) { if(i>0){i--;printf("\b \b");} }
        else if(ch>=32 && ch<=126 && i<maxLen-1){matKhau[i++]=(char)ch;putchar('*');}
    }
    matKhau[i]='\0'; putchar('\n'); anConTro();
}

void napHashAdmin(char hash[65]) {
    FILE *f=fopen("admin.hash","r");
    if(f){if(fgets(hash,65,f)==NULL)hash[0]='\0';fclose(f);}
    else {strcpy(hash,ADMIN_HASH_MAC_DINH);f=fopen("admin.hash","w");if(f){fprintf(f,"%s\n",hash);fclose(f);}}
    if(strlen(hash)!=64)strcpy(hash,ADMIN_HASH_MAC_DINH);
}

int dangNhapAdmin(void) {
    char pass[128], hash[65], nhapHash[65]; int lan;
    napHashAdmin(hash);
    for(lan=0;lan<3;lan++){
        xoaManHinh();veBannerDau("XAC THUC QUAN TRI VIEN");veKhung(25,10,70,15,0x1B);
        inTai(31,13,0x1E,"Nhap mat khau Admin (ky tu se duoc an):");
        veKhung(34,15,52,4,0x17);toiViTri(37,16);docMatKhauAdmin(pass,sizeof(pass));
        sha256Hex((const unsigned char*)pass,strlen(pass),nhapHash);
        memset(pass,0,sizeof(pass));
        if(strcmp(nhapHash,hash)==0)return 1;
        inTai(40,20,0x0C,"SAI MAT KHAU!");Beep(440,180);Sleep(550);
    }
    xoaManHinh();veBannerDau("CANH BAO BAO MAT");inTai(28,16,0x0C,"Sai 3 lan. Quay lai cong chon vai tro.");Sleep(1200);return 0;
}

void veTheVaiTro(int y,int dangHover,const char *tieuDe,const char *moTa) {
    int mau=dangHover?0xF0:0x1F;
    veKhung(28,y,64,5,mau);
    inTai(32,y+1,mau,tieuDe);
    inTai(32,y+3,dangHover?0x1E:0x17,moTa);
}

int congChonVaiTro(void) {
    const int yThe[3]={9,17,25};
    const char *tieuDe[3]={"THI SINH VAO THI","QUAN TRI VIEN / GIANG VIEN","THOAT CHUONG TRINH"};
    const char *moTa[3]={"Nhap ten, MSSV va bat dau lam bai","Dang nhap de mo bang dieu khien Admin","Click de dong chuong trinh"};
    INPUT_RECORD rec;DWORD docDuoc;int hover=-1,i=-1;
    xoaManHinh();veBannerDau("HE THONG THI TRAC NGHIEM TRUC TUYEN");
    veKhung(18,6,84,30,0x1B);
    inTai(36,7,0x1E,"VUI LONG CHON VAI TRO");
    for(i=0;i<3;i++)veTheVaiTro(yThe[i],0,tieuDe[i],moTa[i]);
    inTai(39,33,0x17,"Re chuot va click vao muc muon chon");
    FlushConsoleInputBuffer(hIn);batCheDoChuot();
    while(1){
        if(!ReadConsoleInput(hIn,&rec,1,&docDuoc)||docDuoc==0)continue;
        if(rec.EventType==KEY_EVENT&&rec.Event.KeyEvent.bKeyDown&&rec.Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE)return 0;
        if(rec.EventType==WINDOW_BUFFER_SIZE_EVENT){
            xoaManHinh();veBannerDau("HE THONG THI TRAC NGHIEM TRUC TUYEN");veKhung(18,6,84,30,0x1B);
            inTai(36,7,0x1E,"VUI LONG CHON VAI TRO");for(i=0;i<3;i++)veTheVaiTro(yThe[i],i==hover,tieuDe[i],moTa[i]);
            inTai(39,33,0x17,"Re chuot va click vao muc muon chon");continue;
        }
        if(rec.EventType==MOUSE_EVENT){
            MOUSE_EVENT_RECORD m=rec.Event.MouseEvent;int moi=-1;
            for(i=0;i<3;i++)if(m.dwMousePosition.X>=28&&m.dwMousePosition.X<92&&m.dwMousePosition.Y>=yThe[i]&&m.dwMousePosition.Y<yThe[i]+5){moi=i;break;}
            if(moi!=hover){if(hover>=0)veTheVaiTro(yThe[hover],0,tieuDe[hover],moTa[hover]);hover=moi;if(hover>=0)veTheVaiTro(yThe[hover],1,tieuDe[hover],moTa[hover]);}
            if(moi>=0&&(m.dwButtonState&FROM_LEFT_1ST_BUTTON_PRESSED)&&m.dwEventFlags!=MOUSE_MOVED){
                phatAmThanh();if(moi==0)return 1;if(moi==1)return 2;return 0;
            }
        }
    }
}

void xemNganHangAdmin(struct CauHoi ds[],int n) {
    int trang=0,i,batDau,soTrangThuong=(n-1+9)/10,tongTrang=soTrangThuong+1;
    char dong[180],*opts[4];
    do{
        xoaManHinh();veBannerDau("ADMIN | NGAN HANG CAU HOI VA DAP AN");
        if(trang<soTrangThuong){
            int ketThuc;
            veKhung(2,4,116,32,0x1B);batDau=trang*10;ketThuc=batDau+10;
            if(ketThuc>n-1)ketThuc=n-1;
            for(i=batDau;i<ketThuc;i++){
                int y=5+(i-batDau)*3;opts[0]=ds[i].a;opts[1]=ds[i].b;opts[2]=ds[i].c;opts[3]=ds[i].d;
                sprintf(dong,"C%02d. %.105s",i+1,ds[i].noiDung);inTai(4,y,0x1E,dong);
                sprintf(dong,"A:%.17s  B:%.17s  C:%.17s  D:%.17s | DUNG: %c",opts[0],opts[1],opts[2],opts[3],ds[i].dapAnDung);inTai(6,y+1,0x1F,dong);
            }
        }else{
            int j,y;struct CauHoi *q=&ds[n-1];
            veKhung(2,4,116,32,0x1B);
            sprintf(dong,"CAU %02d - NOI DUNG MA NGUON",n);inTai(5,5,0x1E,dong);
            inCauHoiNguon(6,7,0x1F,q->noiDung,15);
            opts[0]=q->a;opts[1]=q->b;opts[2]=q->c;opts[3]=q->d;
            for(j=0;j<4;j++){
                const char *dapAn=opts[j];y=23+j*3;
                veKhung(7,y,106,3,('A'+j)==q->dapAnDung?0x0A:0x17);
                sprintf(dong,"[%c] %.96s",'A'+j,dapAn);
                inTai(10,y+1,('A'+j)==q->dapAnDung?0x0A:0x1F,dong);
            }
        }
        sprintf(dong,"Mui ten trai/phai: chuyen trang | ESC: quay lai | Trang %d/%d",trang+1,tongTrang);
        inTai(4,37,0x17,dong);
        {int k=getch();if(k==27)break;if(k==0||k==224){k=getch();if(k==77&&trang+1<tongTrang)trang++;else if(k==75&&trang>0)trang--;}}
    }while(1);
}

int caiDatThoiGianThi(void) {
    FILE *f=fopen("thoi_gian_thi.cfg","r");int phut=20;if(f){fscanf(f,"%d",&phut);fclose(f);}if(phut<1||phut>240)phut=20;return phut*60;
}

void cauHinhThoiGianAdmin(int *gioiHanGiay) {
    char s[16];int p;xoaManHinh();veBannerDau("ADMIN | CAU HINH THOI GIAN THI");veKhung(18,7,84,25,0x1B);
    inTai(27,11,0x1F,"Nhap so phut thi (1-240):");veKhung(27,13,48,4,0x17);
    toiViTri(30,14);hienConTro();fgets(s,sizeof(s),stdin);anConTro();p=atoi(s);
    if(p>=1&&p<=240){FILE*f=fopen("thoi_gian_thi.cfg","w");if(f){fprintf(f,"%d\n",p);fclose(f);*gioiHanGiay=p*60;inTai(27,20,0x0A,"Da cap nhat thoi gian thi.");}else inTai(27,20,0x0C,"Khong ghi duoc file cau hinh.");}
    else inTai(27,20,0x0C,"Thoi gian khong hop le.");
    inTai(27,29,0x17,"Nhan phim bat ky de quay lai");getch();
}

void doiMatKhauAdmin(void) {
    char p1[128],p2[128],h[65];FILE*f;
    xoaManHinh();veBannerDau("ADMIN | DOI MAT KHAU");veKhung(18,6,84,30,0x1B);
    inTai(28,9,0x1F,"MAT KHAU MOI:");veKhung(28,10,64,4,0x17);
    inTai(31,12,0x1F," ");toiViTri(31,12);docMatKhauAdmin(p1,sizeof(p1));
    inTai(28,16,0x1F,"NHAP LAI MAT KHAU MOI:");veKhung(28,17,64,4,0x17);
    inTai(31,19,0x1F," ");toiViTri(31,19);docMatKhauAdmin(p2,sizeof(p2));
    if(strlen(p1)<6||strcmp(p1,p2)!=0)inTai(28,24,0x0C,"Hai mat khau phai trung nhau va dai it nhat 6 ky tu.");
    else{sha256Hex((const unsigned char*)p1,strlen(p1),h);f=fopen("admin.hash","w");if(f){fprintf(f,"%s\n",h);fclose(f);inTai(28,24,0x0A,"Da luu hash mat khau moi.");}else inTai(28,24,0x0C,"Khong ghi duoc admin.hash.");}
    memset(p1,0,sizeof(p1));memset(p2,0,sizeof(p2));inTai(28,31,0x17,"Nhan phim bat ky de tro lai bang Admin");getch();
}

void kiemTraPhieuDiemAdmin(void);

void veTheAdmin(int muc,int hover,const char *tieuDe,const char *moTa) {
    static const int yThe[5]={8,13,18,23,28};
    int mau=hover?0xF0:0x1F;
    veKhung(25,yThe[muc],70,4,mau);
    inTai(28,yThe[muc]+1,mau,tieuDe);
    inTai(28,yThe[muc]+2,hover?0x1E:0x17,moTa);
}

void veBangDieuKhienAdmin(int gioiHanGiay,int hover) {
    static const char *tieuDe[5]={"KIEM TRA PHIEU DIEM","CAU HINH THOI GIAN THI","NGAN HANG CAU HOI VA DAP AN","DOI MAT KHAU ADMIN","DANG XUAT"};
    static const char *moTa[5]={"Nhap MSSV va kiem tra tinh toan ven phieu diem","Thoi gian thi hien tai co the duoc thay doi","Xem cac cau hoi cung dap an dung","Cap nhat mat khau quan tri vien","Quay lai cong chon vai tro"};
    char dong[100];int i;
    xoaManHinh();veBannerDau("BANG DIEU KHIEN GIANG VIEN");veKhung(17,5,86,33,0x1B);
    sprintf(dong,"Thoi gian thi hien tai: %d phut",gioiHanGiay/60);inTai(25,6,0x0E,dong);
    for(i=0;i<5;i++)veTheAdmin(i,i==hover,tieuDe[i],moTa[i]);
    inTai(30,34,0x17,"Re chuot va click vao muc | ESC: dang xuat");
}

void bangDieuKhienAdmin(struct CauHoi ds[],int n,int *gioiHanGiay) {
    INPUT_RECORD rec;DWORD docDuoc;int hover=-1,i=-1;
    const int yThe[5]={8,13,18,23,28};
    const char *tieuDe[5]={"KIEM TRA PHIEU DIEM","CAU HINH THOI GIAN THI","NGAN HANG CAU HOI VA DAP AN","DOI MAT KHAU ADMIN","DANG XUAT"};
    const char *moTa[5]={"Nhap MSSV va kiem tra tinh toan ven phieu diem","Thoi gian thi hien tai co the duoc thay doi","Xem cac cau hoi cung dap an dung","Cap nhat mat khau quan tri vien","Quay lai cong chon vai tro"};
    batCheDoChuot();
    for(;;){
        veBangDieuKhienAdmin(*gioiHanGiay,hover);FlushConsoleInputBuffer(hIn);
        while(ReadConsoleInput(hIn,&rec,1,&docDuoc)&&docDuoc>0){
            if(rec.EventType==KEY_EVENT&&rec.Event.KeyEvent.bKeyDown){
                int vk=rec.Event.KeyEvent.wVirtualKeyCode;
                if(vk==VK_ESCAPE||vk=='0')return;
                if(vk>='1'&&vk<='4'){i=vk-'1';break;}
            }
            if(rec.EventType==WINDOW_BUFFER_SIZE_EVENT){hover=-1;veBangDieuKhienAdmin(*gioiHanGiay,hover);continue;}
            if(rec.EventType==MOUSE_EVENT){
                MOUSE_EVENT_RECORD m=rec.Event.MouseEvent;int moi=-1,j;
                for(j=0;j<5;j++)if(m.dwMousePosition.X>=25&&m.dwMousePosition.X<95&&m.dwMousePosition.Y>=yThe[j]&&m.dwMousePosition.Y<yThe[j]+4){moi=j;break;}
                if(moi!=hover){if(hover>=0)veTheAdmin(hover,0,tieuDe[hover],moTa[hover]);hover=moi;if(hover>=0)veTheAdmin(hover,1,tieuDe[hover],moTa[hover]);}
                if(moi>=0&&(m.dwButtonState&FROM_LEFT_1ST_BUTTON_PRESSED)&&m.dwEventFlags!=MOUSE_MOVED){phatAmThanh();i=moi;break;}
            }
        }
        if(i==0)kiemTraPhieuDiemAdmin();
        else if(i==1)cauHinhThoiGianAdmin(gioiHanGiay);
        else if(i==2)xemNganHangAdmin(ds,n);
        else if(i==3)doiMatKhauAdmin();
        else if(i==4)return;
        i=-1;hover=-1;
    }
}

void taoTenPhieu(char *path,const char *mssv){sprintf(path,"phieu_diem_%s.txt",mssv);}

void xuatPhieuDiem(const struct NguoiThi *ts,const char *mon,struct CauHoi ds[],char traLoi[],int n,int dung,float diem,int giay) {
    char path[80],body[12000],sig[65];int used=0,i;FILE*f;taoTenPhieu(path,ts->maSoSinhVien);
    used+=sprintf(body+used,"HE THONG THI TRAC NGHIEM\r\nHo ten: %s\r\nMSSV: %s\r\nMon thi: %s\r\nSo cau dung: %d/%d\r\nDiem: %.2f/10\r\nThoi gian: %02d:%02d:%02d\r\n",ts->hoTen,ts->maSoSinhVien,mon,dung,n,diem,giay/3600,(giay%3600)/60,giay%60);
    for(i=0;i<n;i++)used+=sprintf(body+used,"Cau %d: Thi sinh chon %c | Dap an dung %c | Diem %.1f\r\n",i+1,traLoi[i]==' '?'_':traLoi[i],ds[i].dapAnDung,ds[i].diem);
    hmacSha256Hex(KHOA_KY_PHIEU,(const unsigned char*)body,(size_t)used,sig);
    f=fopen(path,"wb");if(f){fwrite(body,1,(size_t)used,f);fprintf(f,"CHU_KY_SHA256: %s\r\n",sig);fclose(f);xoaManHinh();veBannerDau("DA XUAT PHIEU DIEM");inTai(10,14,0x0A,"Phieu diem va danh sach dap an da duoc luu.");inTai(10,16,0x1F,path);}else{inTai(10,14,0x0C,"Khong tao duoc file phieu diem.");}Sleep(1300);
}

void kiemTraPhieuDiemAdmin(void) {
    char mssv[32],path[80],data[16000],sig[65],calc[65],dong[180];
    FILE *f;size_t len,bodyLen;char *mark,*line;int ok=0,daTimThay=0,i,hopLe=1,y;
    xoaManHinh();veBannerDau("ADMIN | KIEM TRA CHU KY PHIEU DIEM");
    inTai(10,10,0x1F,"Nhap MSSV 10 chu so:");veKhung(37,9,30,3,0x17);toiViTri(39,10);hienConTro();
    fgets(mssv,sizeof(mssv),stdin);anConTro();mssv[strcspn(mssv,"\r\n")]='\0';
    if(strlen(mssv)==10){
        for(i=0;i<10;i++)if(!isdigit((unsigned char)mssv[i]))hopLe=0;
        if(hopLe){
            taoTenPhieu(path,mssv);f=fopen(path,"rb");
            if(f){
                daTimThay=1;len=fread(data,1,sizeof(data)-1,f);fclose(f);data[len]='\0';
                mark=strstr(data,"CHU_KY_SHA256: ");
                if(mark){
                    bodyLen=(size_t)(mark-data);
                    if(sscanf(mark,"CHU_KY_SHA256: %64[0-9A-Fa-f]",sig)==1){
                        hmacSha256Hex(KHOA_KY_PHIEU,(const unsigned char*)data,bodyLen,calc);
                        ok=(strcmp(calc,sig)==0);
                    }
                    *mark='\0';
                }
            }
        }
    }
    if(!daTimThay){
        inTai(10,14,0x0C,"KHONG TIM THAY PHIEU DIEM CHO MSSV NAY.");
    }else{
        inTai(10,13,ok?0x0A:0x0C,ok?"CHU KY HOP LE - THONG TIN CHUA BI THAY DOI.":"CANH BAO: PHIEU DIEM DA THAY DOI THONG TIN!");
        inTai(10,15,0x1E,"THONG TIN PHIEU DIEM:");
        y=17;line=strtok(data,"\r\n");
        while(line&&y<35){sprintf(dong,"%.150s",line);inTai(10,y++,0x1F,dong);line=strtok(NULL,"\r\n");}
    }
    inTai(10,36,0x17,"Nhan phim bat ky de quay lai");getch();
}

int main() {
    struct NguoiThi thiSinh;
    int thoiGianGioiHan = caiDatThoiGianThi();
    srand((unsigned)time(NULL));
    khoiTaoConsole();
    
    // 3. NGAN HANG 40 CAU HOI TONG HOP
    // - 20 cau dau (0 - 19): Mon Triet hoc Mac - Lenin
    // - 20 cau sau (20 - 39): Mon Lap trinh C (10 cau De, 5 cau TB, 5 cau Kho)
    struct CauHoi nganHang[40] = {
        // --- 20 CAU HOI MON TRIET HOC MAC - LENIN ---
        // 10 cau De (0.5d)
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
        
        // 5 cau Trung binh (1.0d)
        { "Khang dinh con nguoi nhan thuc duoc the gioi thuoc mat nao?", "Mat ban the luan.", "Mat nhan thuc luan.", "Ca hai mat.", "Khong thuoc van de co ban.", 'B', 1.0f },
        { "Quan diem \"Su vat la tap hop cac cam giac\" thuoc truong phai nao?", "Duy vat sieu hinh.", "Duy tam khach quan.", "Duy tam chu quan.", "Nhi nguyen luan.", 'C', 1.0f },
        { "Quan niem xem xet su vat chuyen hoa lan nhau the hien phuong phap gi?", "Phuong phap sieu hinh.", "Phuong phap bien chung.", "Phuong phap nguy bien.", "Phuong phap chiet trung.", 'B', 1.0f },
        { "Quy luat nao giai thich viec sinh vien tich luy kien thuc de vuot ky thi?", "Quy luat mau thuan.", "Quy luat luong - chat.", "Quy luat phu dinh cua phu dinh.", "Tinh doc lap cua y thuc.", 'B', 1.0f },
        { "Tu nguyen ly lien he pho bien rut ra quan diem phuong phap luan nao?", "Toan dien va lich su - cu the.", "Phat trien thuan tuy.", "Phien dien, chiet trung.", "Chu quan, duy y chi.", 'A', 1.0f },
        
        // 5 cau Kho (1.5d)
        { "Han che lon nhat cua chu nghia duy vat truoc Mac la gi?", "Sieu hinh va duy tam ve xa hoi.", "Phu nhan vat chat.", "Coi y thuc la ban nguyen.", "Tuyet doi hoa ton giao.", 'A', 1.5f },
        { "Vai tro cua Triet hoc Mac - Lenin doi voi con nguoi la gi?", "Cung cap the gioi quan va phuong phap luan khoa hoc.", "Thay the moi khoa hoc cu the.", "Dua ra loi giai san co.", "Chi co gia tri ly luan thuan tuy.", 'A', 1.5f },
        { "Khai niem \"phat trien\" khac \"van dong\" o diem co ban nao?", "Van dong di len, tu thap den cao.", "Bao gom ca su suy thoai.", "Chi tang giam ve luong.", "Lap lai hoan toan cai cu.", 'A', 1.5f },
        { "Nguyen tac phuong phap luan quan trong nhat tu moi quan he vat chat - y thuc?", "Ton trong khach quan, phat huy chu quan.", "Xuat phat tu y muon chu quan.", "Thu dong cho doi.", "Tuyet doi hoa ly thuyet.", 'A', 1.5f },
        { "Khuynh huong phat trien cua quy luat \"Phu dinh cua phu dinh\"?", "Duong thang tap.", "Duong tron khep kin.", "Duong xoay tron oc ke thua va phat trien.", "Duong dut doan ngau nhien.", 'C', 1.5f },
         
        // --- 20 CAU HOI MON LAP TRINH C ---
        // 10 cau De (0.5d)
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

        // 5 cau Trung binh (1.0d)
        { "Neu switch thieu lenh break o case, hien tuong gi xay ra?", "Bao loi bien dich.", "Tu dong thoat.", "Chay xuyen sang cac case ben duoi (fall-through).", "Quay lai tu dau.", 'C', 1.0f },
        { "Duyet mang N phan tu theo chieu nguoc, vong lap for thiet lap ra sao?", "i = N, i > 0, giam i", "i = N - 1, i >= 0, giam i", "i = N - 1, i > 0, giam i", "i = N, i >= 0, giam i", 'B', 1.0f },
        { "Vong lap while neu dieu kien sai ngay tu dau chay may lan?", "0 lan.", "1 lan.", "Lap vo han.", "Phu thuoc du lieu.", 'A', 1.0f },
        { "Truy cap vuot qua chi so mang trong C gay ra hau qua gi?", "Tu dong mo rong mang.", "Bao loi cu phap luc bien dich.", "Loi truy cap o nho (Undefined Behavior).", "Gia tri tu dong ve 0.", 'C', 1.0f },
        { "Trong chuoi if - else if - else, C xu ly the nao?", "Kiem tra tat ca dieu kien.", "Kiem tra tuan tu, dung nhanh dau tien thi thuc thi roi dung.", "Chi kiem tra nhanh else.", "Danh gia ngau nhien.", 'B', 1.0f },

        // 5 cau Kho (1.5d)
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
    int vaiTro;

    for (;;) {
        vaiTro = congChonVaiTro();
        if (vaiTro == 0) { hienConTro(); return 0; }
        if (vaiTro == 2) {
            if (dangNhapAdmin()) bangDieuKhienAdmin(nganHang, 40, &thoiGianGioiHan);
            continue;
        }
        nhapThongTinNguoiThi(&thiSinh);
        break;
    }

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

        // Xac dinh vi tri bat dau trong mang tong (Triet tu 0, C tu 20)
        int viTriBatDau = (luaChon == 1) ? 0 : 20;
        char tenMon[50];
        sprintf(tenMon, "%s", (luaChon == 1) ? "Triet hoc Mac - Lenin" : "Ly thuyet Lap trinh C");

        // Tach 20 cau cua mon thanh 3 nhom do kho chuan: 10 cau De, 5 cau TB, 5 cau Kho
        struct CauHoi nhomDe[10], nhomTB[5], nhomKho[5];
        int countDe = 0, countTB = 0, countKho = 0;
        int i;

        for (i = 0; i < 20; i++) {
            struct CauHoi q = nganHang[viTriBatDau + i];
            if (i < 10) {
                nhomDe[countDe++] = q;   // 10 cau dau: De (0.5 diem)
            } else if (i < 15) {
                nhomTB[countTB++] = q;   // 5 cau tiep: Trung binh (1.0 diem)
            } else {
                nhomKho[countKho++] = q; // 5 cau cuoi: Kho (1.5 diem)
            }
        }

        // Tron ngau nhien tung nhom do kho
        tronDeThi(nhomDe, 10);
        tronDeThi(nhomTB, 5);
        tronDeThi(nhomKho, 5);

        // THUAT TOAN TAO DE CHUAN HOA TONG = 10 DIEM
        int loaiDe = rand() % 3;
        int numDe = 0, numTB = 0, numKho = 0;

        if (loaiDe == 0) {
            // De 9 cau: 2 cau De (1.0d) + 3 cau TB (3.0d) + 4 cau Kho (6.0d) = 10.0d
            numDe = 2; numTB = 3; numKho = 4;
        } else if (loaiDe == 1) {
            // De 10 cau: 4 cau De (2.0d) + 2 cau TB (2.0d) + 4 cau Kho (6.0d) = 10.0d
            numDe = 4; numTB = 2; numKho = 4;
        } else {
            // De 13 cau: 8 cau De (4.0d) + 3 cau TB (3.0d) + 2 cau Kho (3.0d) = 10.0d
            numDe = 8; numTB = 3; numKho = 2;
        }

        int soCauThi = numDe + numTB + numKho;
        struct CauHoi deThi[MAX_CAU_THI];
        int idx = 0;

        for (i = 0; i < numDe; i++) deThi[idx++] = nhomDe[i];
        for (i = 0; i < numTB; i++) deThi[idx++] = nhomTB[i];
        for (i = 0; i < numKho; i++) deThi[idx++] = nhomKho[i];

        // Tron tong the de cac cau De/TB/Kho xen ke ngau nhien
        tronDeThi(deThi, soCauThi);

        // Mang luu dap an thi sinh da chon
        char cauTraLoi[MAX_CAU_THI];
        int coDanhDau[MAX_CAU_THI];
        int cauHienTai = 0;
        for (i = 0; i < soCauThi; i++) {
            cauTraLoi[i] = ' ';
            coDanhDau[i] = 0;
        }

        // 4. BAT DAU DEM THOI GIAN (20 phut = 1200 giay)
        time_t thoiGianBatDau = time(NULL);
        DWORD batDauBai = GetTickCount();
        DWORD gioiHanMs = (DWORD)thoiGianGioiHan * 1000UL;
        while (1) {
            struct ThaoTacThi thaoTac;
            thaoTac = chonTrongBangCauHoi(deThi, soCauThi, cauHienTai,
                                          cauTraLoi, coDanhDau,
                                          batDauBai, gioiHanMs);
            if (thaoTac.loai == THI_HET_GIO || thaoTac.loai == THI_NOP_BAI) break;
            if (thaoTac.loai == THI_CAU_TRUOC) {
                if (cauHienTai > 0) cauHienTai--;
            } else if (thaoTac.loai == THI_CAU_SAU) {
                if (cauHienTai + 1 < soCauThi) cauHienTai++;
            } else if (thaoTac.loai == THI_BAT_TAT_CO) {
                coDanhDau[cauHienTai] = !coDanhDau[cauHienTai];
            } else if (thaoTac.loai == THI_NHAY_CAU) {
                if (thaoTac.giaTri >= 0 && thaoTac.giaTri < soCauThi)
                    cauHienTai = thaoTac.giaTri;
            } else if (thaoTac.loai == THI_CHON_DAP_AN) {
                cauTraLoi[cauHienTai] = thaoTac.dapAn;
                if (cauHienTai + 1 < soCauThi) cauHienTai++;
            }
        }

        // Tinh thoi gian lam bai thuc te
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

        for (i = 0; i < soCauThi; i++) {
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
        xuatPhieuDiem(&thiSinh, tenMon, deThi, cauTraLoi, soCauThi,
                      soCauDung, tongDiem, (int)thoiGianLamBai);

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
            inTai(37, 17, 0x1F, "CAM ON BAN DA SU DUNG CHUONG TRINH!");
            hienConTro();
            Sleep(700);
            break;
        }
    } while (1);

    return 0;
}

