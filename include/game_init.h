// game_init.h
#ifndef GAME_INIT_H
#define GAME_INIT_H

#include "raylib.h"
#include "bird_struct.h"

void InitGame(Bird *birds, Bird *bird, int Pipa[3][3], int TutupPipa[3][3], Texture2D *cityBg);
void CleanupGame(Bird *birds, Bird *bird, Texture2D cityBg);

#endif