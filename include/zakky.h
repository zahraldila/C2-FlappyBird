#ifndef ZAKKY_H
#define ZAKKY_H

#include "raylib.h"
#include "bird_struct.h"
#define KECEPATAN_AWAN 1
#define JUMLAH_AWAN 5
extern float bgX;

Bird CreateBird(float x, float y, const char *texturePath, float scale);

typedef enum {
    MENU,
    GAMEPLAY,
    BACKGROUND,
    SKIN,
    LEADERBOARD,
    CREDIT
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







#endif // 