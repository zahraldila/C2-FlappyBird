#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"
#include "bird_struct.h"

#define MAX_BIRDS 1
#define GRAVITY 0.5f
#define FLAP_STRENGTH -8.0f

// Fungsi untuk linked list
BirdNode* InitBirdsLinkedList(int count);
void UpdateBirds(BirdNode *head);
void DrawBirds(BirdNode *head);
void UnloadBirds(BirdNode *head);

void InitBackground(Texture2D *bg);
void UpdateBackground(float *bgX);
void DrawBackground(Texture2D bg, float bgX);

#endif
