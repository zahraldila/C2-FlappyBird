#ifndef BIRD_STRUCT_H
#define BIRD_STRUCT_H

#include "raylib.h"

typedef struct Bird {
    Vector2 position;
    float speed;
    float scale;
    Texture2D texture;
} Bird;

typedef struct BirdNode {
    Bird bird;
    struct BirdNode *prev;
    struct BirdNode *next;
} BirdNode;

// Node untuk pergerakan Y
typedef struct YNode {
    float y;
    struct YNode *next;
} YNode;

#endif