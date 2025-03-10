#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;  // Skala untuk ukuran burung
} Bird;

Bird CreateBird(float x, float y, const char *texturePath, float scale);
void DrawBird(Bird bird);
void UnloadBird(Bird *bird);


typedef struct {
    Vector2 position;
    float width;
    float height;
    Color color;
} Pipe;
Pipe CreatePipe(float x, float y, float width, float height, Color color);
void DrawPipe(Pipe pipe);

#endif // 