#include "quizz.h"

// THUAT TOAN XAO TRON FISHER-YATES
void tronDeThi(struct CauHoi ds[], int soLuong) {
    for (int i = 0; i < soLuong; i++) {
        int viTriNgauNhien = rand() % soLuong;
        struct CauHoi tam = ds[i];
        ds[i] = ds[viTriNgauNhien];
        ds[viTriNgauNhien] = tam;
    }
}