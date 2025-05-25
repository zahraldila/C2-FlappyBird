#ifndef BIRD_STRUCT_H
#define BIRD_STRUCT_H

#include "raylib.h"

// ======================== Struktur untuk Burung ========================
typedef struct {
    Texture2D texture;
    Vector2 position;
    float speed;
    float scale; // Meskipun tekstur di-resize, scale bisa untuk penyesuaian tambahan jika perlu
    float collisionWidth;   // Lebar bounding box untuk tabrakan (lebih kecil dari texture.width)
    float collisionHeight;  // Tinggi bounding box untuk tabrakan (lebih kecil dari texture.height)
    // Mungkin perlu juga collisionOffsetX dan collisionOffsetY jika bounding box tidak berpusat pada position
} Bird;

#endif