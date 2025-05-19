#ifndef BIRD_STRUCT_H
#define BIRD_STRUCT_H

#include "raylib.h"

typedef struct Bird {
    Texture2D texture;
    Vector2 position;
    float speed;
} Bird;

typedef struct BirdNode {
    float y;
    float speed;
    struct BirdNode* prev;
    struct BirdNode* next;
} BirdNode;

#endif
