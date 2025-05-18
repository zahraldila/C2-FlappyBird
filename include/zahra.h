// zahra.h
#ifndef ZAHRA_H
#define ZAHRA_H

#include "bird_struct.h"
#include "alexandrio.h"

typedef enum {
    GAME_READY,   // Menunggu input pertama
    GAME_ACTIVE,  // Permainan sedang berjalan
    GAME_OVER     // Game over
} GameOverState;

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAMEOVER
} ZahraGameState;

GameOverState UpdateGameCollision(Bird bird, Singlelinkedlist *plist, Singlelinkedlist *tplist);

#endif
