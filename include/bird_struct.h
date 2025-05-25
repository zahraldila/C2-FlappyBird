#ifndef BIRD_STRUCT_H
#define BIRD_STRUCT_H

#include "raylib.h"

// ======================== Struktur untuk Burung ========================
typedef struct {
    Texture2D texture;
    Vector2 position;
    float speed;
} Bird;

#endif
