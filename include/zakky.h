#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h"
#include "dava.h"

#define KECEPATAN_AWAN 1
#define JUMLAH_AWAN 5
#define KECEPATAN_BACKGROUND_SCROLL 0.5f

extern float bgX;

#ifndef LEBAR_LAYAR
#define LEBAR_LAYAR 800
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 450
#endif

typedef enum {
    MENU,
    GAMEPLAY,
    BACKGROUND_SELECTION,
    SKIN,
    LEADERBOARD, // State baru
    CREDIT
} GameState;

typedef struct {
    bool isPause;
} PauseState;

void jedapause (PauseState *tmblPause);
void tombolpause (PauseState *tmblPause);
void DrawPauseScreen(PauseState *tmblPause);

void setupMenu(void);
GameState pilihMenu(void);
GameState DrawMenu(void);

void InitBackgroundSelectionScreen(void);
GameState UpdateBackgroundSelectionScreen(BackgroundSelector *bgSelector, GameState currentGameState);
void DrawBackgroundSelectionScreen(BackgroundSelector *bgSelector);

// Fungsi untuk Layar Leaderboard (ditempatkan di main_menu.c)
void InitLeaderboardScreen(void); // Mungkin tidak perlu jika LoadLeaderboard sudah cukup
GameState UpdateLeaderboardScreen(GameState currentGameState);
void DrawLeaderboardScreen(Font font); // Butuh font untuk menggambar teks


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