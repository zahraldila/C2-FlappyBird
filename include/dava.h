#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"
#include "bird_struct.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define GRAVITY 0.25f
#define FLAP_STRENGTH -5.5f
#define LEBAR_LAYAR 800

typedef struct BirdGame {
    Bird bird;
    BirdNode* yTrackHead;   // Head DLL posisi Y
    BirdNode* yTrackNow;    // Pointer ke node saat ini (terbaru)
} BirdGame;

// Fungsi utama
BirdGame* InitBird();
void UpdateBird(BirdGame* bg);
void DrawBird(BirdGame* bg);
void UnloadBird(BirdGame* bg);

// Fungsi latar belakang
void InitBackground(Texture2D* bg);
void UpdateBackground(float* bgX);
void DrawBackground(Texture2D bg, float bgX);

#endif
