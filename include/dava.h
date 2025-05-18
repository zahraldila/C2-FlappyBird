#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"
#include "bird_struct.h"

#define GRAVITY 0.25f
#define FLAP_STRENGTH -5.5f
#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450

// Inisialisasi burung dengan Doubly Linked List

// Update posisi burung (naik turun)
void UpdateBird(BirdNode *birdNode);

// Gambar burung
void DrawBird(BirdNode *birdNode);

// Hapus resource burung
void UnloadBird(BirdNode *birdNode);

// Background
void InitBackground(Texture2D *bg);
void UpdateBackground(float *bgX);
void DrawBackground(Texture2D bg, float bgX);

#endif
