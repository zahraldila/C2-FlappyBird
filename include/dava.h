#ifndef DAVA_H
#define DAVA_H

#include "bird_struct.h"
#include "raylib.h"

// Konstanta game
#define SCREEN_WIDTH 800    // Lebar layar game.
#define SCREEN_HEIGHT 450   // Tinggi layar game.

#define GRAVITY 0.25f         // Nilai gravitasi untuk burung.
#define FLAP_STRENGTH -5.5f   // Kekuatan kepakan/lompatan burung.

// Fungsi terkait Burung
// Catatan: InitBird() mengembalikan pointer Bird* untuk konsistensi dengan bird.c (malloc).
Bird* InitBird(void);           // Deklarasi fungsi inisialisasi burung.
void UpdateBird(Bird* bird);    // Deklarasi fungsi update logika burung.
void DrawBird(Bird* bird);      // Deklarasi fungsi menggambar burung.
void UnloadBird(Bird* bird);    // Deklarasi fungsi unload sumber daya burung.

// ======================== Background Selector (Linked List) ========================
// Bagian untuk sistem pemilihan background.
typedef struct BackgroundNode {   // Node untuk linked list background.
    Texture2D texture;            // Tekstur background.
    const char* filePath;         // Path file gambar background.
    struct BackgroundNode* next;  // Pointer ke node background selanjutnya.
    struct BackgroundNode* prev;  // Pointer ke node background sebelumnya.
} BackgroundNode;

typedef struct {              // Pengelola untuk linked list background.
    BackgroundNode* head;     // Pointer ke background pertama.
    BackgroundNode* current;  // Pointer ke background yang sedang aktif.
    int total;                // Jumlah total background.
} BackgroundSelector;

BackgroundSelector* InitBackgroundSelector(void);                          // Deklarasi fungsi inisialisasi background selector.
void DrawSelectedBackground(BackgroundSelector* selector);                 // Deklarasi fungsi menggambar background terpilih (statis).
void NextBackground(BackgroundSelector* selector);                         // Deklarasi fungsi untuk memilih background berikutnya.
void PreviousBackground(BackgroundSelector* selector);                     // Deklarasi fungsi untuk memilih background sebelumnya.
void UnloadBackgroundSelector(BackgroundSelector* selector);               // Deklarasi fungsi unload sumber daya background selector.
void LoopDrawSelectedBackground(BackgroundSelector* selector, float* bgX); // Deklarasi fungsi menggambar background terpilih dengan scrolling.

#endif