#ifndef ALEXANDRIO_H
#define ALEXANDRIO_H


#include "raylib.h"


#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450
#define LEBAR_PIPA 80
#define JARAK_PIPA_ATAS_BAWAH 150
#define KECEPATAN_PIPA 2
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MAX_BIRDS 1

extern Color TANAH;
extern Color RUMPUT;
extern int Pipa[3][3];
extern int TutupPipa[3][3];

void Buat_pipa(int Pipa[3][3], int TutupPipa[3][3]);
void Pergerakan_pipa(int Pipa[3][3], int TutupPipa[3][3]);
void Gambar_pipa(int Pipa[3][3], int TutupPipa[3][3], int s);
void Pipa_naik_turun(int i);
void Munculkan_Pipa(int i);


#endif //ALEXANDRIO_H