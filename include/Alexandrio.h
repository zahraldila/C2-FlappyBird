#ifndef ALEXANDRIO_H
#define ALEXANDRIO_H

#include "pipa_ll.h"
#include "raylib.h"

#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450
#define LEBAR_PIPA 80 // Pastikan ini adalah lebar visual badan pipa utama
#define JARAK_PIPA_ATAS_BAWAH 150
#define KECEPATAN_PIPA 2
#define SCREEN_HEIGHT 450 // Konsisten dengan TINGGI_LAYAR
#define SCREEN_WIDTH 800  // Konsisten dengan LEBAR_LAYAR

extern Color TANAH;
extern Color RUMPUT;
extern Singlelinkedlist *plist;
extern Singlelinkedlist *tplist;

// Deklarasi fungsi disesuaikan
void Buat_pipa_linkedlist(void); // Diubah dari Buat_pipa()
void Pergerakan_pipa(void);
void Gambar_pipa(int s);
// void Pipa_naik_turun(); // Tidak dideklarasikan di sini, logika ada di Gambar_pipa
// void Munculkan_Pipa(); // Tidak dideklarasikan di sini, logika ada di Gambar_pipa
void Pipa_berhenti(bool cek);

#endif //ALEXANDRIO_H