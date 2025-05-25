#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h" // Jika ada dependensi ke struct Bird, jika tidak, bisa dihapus dari sini
#include "dava.h"        // Untuk BackgroundSelector

#define KECEPATAN_AWAN 1
#define JUMLAH_AWAN 5
#define KECEPATAN_BACKGROUND_SCROLL 0.5f // Kecepatan background yang sudah kita atur

extern float bgX; // Variabel global untuk posisi X background

// Konstanta layar, pastikan konsisten atau ambil dari header utama seperti alexandrio.h atau dava.h
#ifndef LEBAR_LAYAR // Lindungi agar tidak redefinisi jika sudah ada di header lain
#define LEBAR_LAYAR 800
#endif
#ifndef SCREEN_HEIGHT // Lindungi agar tidak redefinisi
#define SCREEN_HEIGHT 450
#endif


typedef enum {
    MENU,
    GAMEPLAY,
    BACKGROUND_SELECTION,
    SKIN,                 // State baru untuk pemilihan skin (placeholder)
    LEADERBOARD,          // Placeholder
    CREDIT                // Placeholder (jika diperlukan)
    // EXIT_GAME         // Opsi lain untuk menangani keluar game dengan lebih bersih
} GameState;

typedef struct
{
    bool isPause;
} PauseState;

void jedapause (PauseState *tmblPause);
void tombolpause (PauseState *tmblPause);
void DrawPauseScreen(PauseState *tmblPause);

// Fungsi Menu Utama
void setupMenu(void);
GameState pilihMenu(void);
GameState DrawMenu(void); // Ini adalah kombinasi setupMenu dan pilihMenu

// Fungsi untuk Layar Pemilihan Background
void InitBackgroundSelectionScreen(void);
GameState UpdateBackgroundSelectionScreen(BackgroundSelector *bgSelector, GameState currentGameState);
void DrawBackgroundSelectionScreen(BackgroundSelector *bgSelector);

// Fungsi untuk Layar Pemilihan Skin (Placeholder untuk masa depan)
// void InitSkinSelectionScreen(void);
// GameState UpdateSkinSelectionScreen(Bird *currentBird, GameState currentGameState);
// void DrawSkinSelectionScreen(Bird *currentBird);


// Fungsi Awan
typedef struct AwanNode {
    float x;
    float y;
    struct AwanNode *next;
} AwanNode;

AwanNode* createAwan (float x, float y);
void insertAwan(AwanNode **head, float x, float y);
void updateAwan(AwanNode *head);
void gambarAwan(AwanNode *head);
void freeAwan(AwanNode **head);

#endif // ZAKKY_H