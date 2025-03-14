#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h"

typedef struct {
    Vector2 position;
    float width;
    float height;
    Color color;
} Pipe;


extern float bgX;
Bird CreateBird(float x, float y, const char *texturePath, float scale);
void DrawBird(Bird bird);
void UnloadBird(Bird *bird);


typedef enum {
    MENU,
    GAMEPLAY
} GameState;

GameState DrawMenu(int screenWidth, int screenHeight);

#endif // 