#ifndef ALEXANDRIO_H
#define ALEXANDRIO_H

#include "pipa_ll.h"
#include "raylib.h"


#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450
#define LEBAR_PIPA 80
#define JARAK_PIPA_ATAS_BAWAH 150
#define KECEPATAN_PIPA 2
#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800

extern Color TANAH;
extern Color RUMPUT;
// extern int Pipa[3][3];
// extern int TutupPipa[3][3];
extern Singlelinkedlist *plist;
extern Singlelinkedlist *tplist;


void Buat_pipa();
void Pergerakan_pipa();
void Gambar_pipa(int s);
void Pipa_naik_turun();
void Munculkan_Pipa();
void Pipa_berhenti(bool cek);


#endif //ALEXANDRIO_H   