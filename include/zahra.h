#ifndef ZAHRA_H
#define ZAHRA_H

#include "raylib.h"
#include "bird_struct.h"
#include "alexandrio.h"
#include "pipa_ll.h"
#include "zakky.h"
#include "dava.h"

typedef enum {
    GAME_READY,
    GAME_ACTIVE,
    GAME_OVER
} GameOverState;

GameOverState UpdateGameCollision(Bird bird, Singlelinkedlist *plist, Singlelinkedlist *tplist);
void DrawGameOver(int screenWidth, int screenHeight, int currentScore);

#endif // ZAHRA_H