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
#define KOSONG NULL

extern Color TANAH;
extern Color RUMPUT;
extern Singlelinkedlist *plist;
extern Singlelinkedlist *tplist;


void Buat_pipa(); //Modul untuk inisialisasi objek pipa beserta pergerakan vertikalnya ketika skornya sudah lebih dari 14
void Pergerakan_pipa(); //Modul untuk mengatur pergerakan pipa secara horizontal dari kanan ke kiri
void Gambar_pipa(int skor); //modul yang berisi proses untuk menampilkan pipa ke layar
void Munculkan_Pipa(address pipaTemp,address tpipaTemp);//Modul untuk memunculkan gambar objek pipa ke layar 
void Pipa_berhenti(bool cek);//Modul untuk menghentikan pergerakan pipa (dipanggil saat game over)
void Hapus_semua_pipa(); //Modul untuk menghapus semua pipa agar tidak terjadi memory leak


#endif //ALEXANDRIO_H   