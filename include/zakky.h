#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h"
#include "Alexandrio.h"

#define KECEPATAN_AWAN 1.5f
#define JUMLAH_AWAN 5
#define KECEPATAN_BACKGROUND_SCROLL 0.5f

extern float bgX;

typedef enum {
    MENU,
    GAMEPLAY,
    BACKGROUND_SELECTION,
    SKIN,
    LEADERBOARD,
    CREDIT,
    STATE_HELP 
} GameState;

typedef struct {
    bool isPause;
} PauseState;

void jedapause (PauseState *tmblPause);
void tombolpause (PauseState *tmblPause);
void DrawPauseScreen(PauseState *tmblPause);

void setupMenu();
GameState pilihMenu();
GameState DrawMenu();

typedef struct AwanNode {
    float x;
    float y;
    struct AwanNode *next;
} AwanNode;

AwanNode* createAwan (float x, float y);
void insertAwan(AwanNode **head, float x, float y);
void updateAwan(AwanNode *head);
void gambarAwan(AwanNode *head);
void freeAwan(AwanNode **head);

#endif // ZAKKY_H