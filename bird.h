#ifndef BIRD_H
#define BIRD_H

#include "raylib.h"

#define MAX_BIRDS 1  // Untuk konsep array (bisa diperluas nanti)
#define GRAVITY 0.5
#define FLAP_STRENGTH -8

typedef struct {
    Vector2 position;
    float velocity;
    float rotation;
    Texture2D texture;
} Bird;

void InitBirds(Bird birds[]);
void UpdateBirds(Bird birds[]);
void DrawBirds(Bird birds[]);

#endif
