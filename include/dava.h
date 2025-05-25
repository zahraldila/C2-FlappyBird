#ifndef DAVA_H
#define DAVA_H

#include "bird_struct.h" // WAJIB di-include SEBELUM tipe 'Bird' digunakan.
#include "raylib.h"

// Konstanta game
#define SCREEN_WIDTH 800
// SCREEN_HEIGHT 600 ini akan berkonflik jika Alexandrio.h (dengan 450) juga di-include di main.c.
// Pilih salah satu definisi yang konsisten untuk seluruh proyek.
// Untuk saat ini, kita biarkan sesuai file yang Anda berikan.
#define SCREEN_HEIGHT 450

#define GRAVITY 0.25f
#define FLAP_STRENGTH -5.5f

// Fungsi terkait Burung
// Deklarasi InitBird() diubah untuk mengembalikan pointer ke Bird (Bird*)
// agar konsisten dengan implementasi di bird.c yang menggunakan malloc.
Bird* InitBird(void); 
void UpdateBird(Bird* bird); // Parameter sudah benar (Bird*)
void DrawBird(Bird* bird);   // Parameter sudah benar (Bird*)
void UnloadBird(Bird* bird); // Parameter sudah benar (Bird*)

// ======================== Background Selector (Linked List) ========================
// Bagian ini diasumsikan sudah benar dan tidak menyebabkan error pada log terakhir
typedef struct BackgroundNode {
    Texture2D texture;
    const char* filePath;
    struct BackgroundNode* next;
    struct BackgroundNode* prev;
} BackgroundNode;

typedef struct {
    BackgroundNode* head;
    BackgroundNode* current;
    int total;
} BackgroundSelector;

BackgroundSelector* InitBackgroundSelector(void);
void DrawSelectedBackground(BackgroundSelector* selector);
void NextBackground(BackgroundSelector* selector);
void PreviousBackground(BackgroundSelector* selector);
void UnloadBackgroundSelector(BackgroundSelector* selector);
void LoopDrawSelectedBackground(BackgroundSelector* selector, float* bgX);

#endif // DAVA_H