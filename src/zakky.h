#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h"
#define KECEPATAN_AWAN 0.5

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

typedef struct 
{
    bool isPause;
    bool hitungMundur;
    float timer;
}PauseState;

void jedapause (PauseState *tmblPause);
void tombolpause (PauseState *tmblPause);
void DrawPauseScreen(PauseState *tmblPause);

GameState DrawMenu(int screenWidth, int screenHeight);

extern int Awan[3][2];

void Buat_awan(int Awan[3][2]);
void Pergerakan_awan(int Awan[3][2]);
void Gambar_awan(int Awan[3][2]);

#endif // 