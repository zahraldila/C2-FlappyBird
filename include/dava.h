#ifndef DAVA_H
#define DAVA_H

#include "bird_struct.h"
#include "raylib.h"
#include "zakky.h"

// Konstanta game
#define SCREEN_WIDTH 800    // Lebar layar game.
#define SCREEN_HEIGHT 450   // Tinggi layar game.

#define GRAVITY 0.25f         // Nilai gravitasi untuk burung.
#define FLAP_STRENGTH -5.5f   // Kekuatan kepakan/lompatan burung.

// Fungsi terkait Burung
Bird* InitBird(void);
void UpdateBird(Bird* bird);
void DrawBird(Bird* bird);
void UnloadBird(Bird* bird);

// Bagian untuk sistem pemilihan background.
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
void InitBackgroundSelectionScreen(void);
GameState UpdateBackgroundSelectionScreen(BackgroundSelector *bgSelector, GameState currentGameState);
void DrawBackgroundSelectionScreen(BackgroundSelector *bgSelector);

#endif