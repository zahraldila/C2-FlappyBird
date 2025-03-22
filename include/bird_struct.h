#ifndef BIRD_STRUCT_H
#define BIRD_STRUCT_H

#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450

#include "raylib.h"
typedef struct {
    Vector2 position;
    float speed;
    float scale;
    Texture2D texture;
} Bird;


#endif
