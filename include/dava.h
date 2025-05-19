#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"
#include "bird_struct.h"

#define GRAVITY 0.25f
#define FLAP_STRENGTH -5.5f
#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450

// Bird functions
BirdNode* InitBird();
void UpdateBirdYMovement(BirdNode *birdNode, YNode **yHead);
void DrawBird(BirdNode *birdNode);
void UnloadBird(BirdNode *birdNode);

// Y movement linked list
void AddPositionY(YNode **head, float y);
void FreeYMovementList(YNode *head);

// Background
void InitBackground(Texture2D *bg);
void UpdateBackground(float *bgX);
void DrawBackground(Texture2D bg, float bgX);

#endif
