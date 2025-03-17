#ifndef ZAHRA_H
#define ZAHRA_H

#include "raylib.h"
#include "bird_struct.h"
#include "zakky.h"
#include "alexandrio.h"

// Status permainan
typedef enum {
    GAME_READY,   // Status menunggu input pertama
    GAME_ACTIVE,  // Status permainan aktif
    GAME_OVER     // Status game over
} GameOverState;

// Deklarasi fungsi collision
bool CheckBirdPipeCollision(Bird bird, int Pipa[3][3], int TutupPipa[3][3]);
bool CheckBirdGroundCollision(Bird bird);
GameOverState UpdateGameCollision(Bird bird, int Pipa[3][3], int TutupPipa[3][3]);
void DrawGameOver(int screenWidth, int screenHeight, int score);
void ResetGame(Bird *bird, int Pipa[3][3], int TutupPipa[3][3]);

// Fungsi debugging
void DrawHitboxes(Bird bird, int Pipa[3][3], int TutupPipa[3][3]);

#endif // ZAHRA_H