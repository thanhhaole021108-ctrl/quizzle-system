#ifndef QUIZ_H
#define QUIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

// Đinh nghia cau truc cau hoi
struct CauHoi {
    char noiDung[300]; // NOI DUNG CAU OI
    char a[150];       // DAP AN A
    char b[150];       // DAP AN B
    char c[150];       // DAP AN C
    char d[150];       // DAP AN D
    char dapAnDung;    // DAP AN DUNG A,B,C,D
    float diem;        // DIEM SO
};

// Nguyên mẫu hàm (Function Prototype) NGUYEN MAU HAM ( FUNCTION PROTOTYPE)
void tronDeThi(struct CauHoi ds[], int soLuong);

#endif // QUIZ_H