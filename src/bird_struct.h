#ifndef BIRD_STRUCT_H
#define BIRD_STRUCT_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    float speed;
    float scale;
    Texture2D texture;
} Bird;

#endif
