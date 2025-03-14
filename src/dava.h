#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"
#include "bird_struct.h"

#define MAX_BIRDS 1
#define GRAVITY 1.4f
#define FLAP_STRENGTH -14.0f

// Deklarasi fungsi
void InitBirds(Bird bird[], int count);
void UpdateBirds(Bird bird[], int count);
void DrawBirds(Bird bird[], int count);
void UnloadBirds(Bird bird[], int count);
void InitBackground(Texture2D *bg);
void UpdateBackground(float *bgX);
void DrawBackground(Texture2D bg, float bgX);

#endif
