#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Bird InitBird();
void UpdateBird(Bird* bird);
void DrawBird(Bird* bird);
void UnloadBird(Bird* bird);

// ======================== Background Selector (Linked List) ========================
typedef struct BackgroundNode {
    Texture2D texture;
    const char* filePath;
    struct BackgroundNode* next;
    struct BackgroundNode* prev;
} BackgroundNode;

typedef struct {
    BackgroundNode* head;
    BackgroundNode* current;
    int total;
} BackgroundSelector;

BackgroundSelector* InitBackgroundSelector();
void DrawSelectedBackground(BackgroundSelector* selector);
void NextBackground(BackgroundSelector* selector);
void PreviousBackground(BackgroundSelector* selector);
void UnloadBackgroundSelector(BackgroundSelector* selector);
void LoopDrawSelectedBackground(BackgroundSelector* selector, float* bgX);

#endif
