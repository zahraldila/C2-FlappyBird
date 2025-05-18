#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h"
#define KECEPATAN_AWAN 1
extern float bgX;

Bird CreateBird(float x, float y, const char *texturePath, float scale);
// void DrawBird(Bird bird);
// void UnloadBird(Bird *bird);


typedef enum {
    MENU,
    GAMEPLAY
} GameState;

typedef struct 
{
    bool isPause;
}PauseState;

void jedapause (PauseState *tmblPause);
void tombolpause (PauseState *tmblPause);
void DrawPauseScreen(PauseState *tmblPause);

void setupMenu();
GameState pilihMenu();
GameState DrawMenu();
 
extern int Awan[5][2];

void Buat_awan(int Awan[5][2]);
void Pergerakan_awan(int Awan[5][2]);
void Gambar_awan(int Awan[5][2]);

#endif // 