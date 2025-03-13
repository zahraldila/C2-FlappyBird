#ifndef ALEXANDRIO_H
#define ALEXANDRIO_H


#include "raylib.h"


#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450
#define LEBAR_PIPA 80
#define JARAK_PIPA_ATAS_BAWAH 150
#define KECEPATAN_PIPA 2

extern Color TANAH;
extern Color RUMPUT;

void Buat_pipa(int Pipa[3][3], int TutupPipa[3][3]);
void Pergerakan_pipa(int Pipa[3][3], int TutupPipa[3][3]);
void Gambar_pipa(int Pipa[3][3], int TutupPipa[3][3]);

#endif //ALEXANDRIO_H