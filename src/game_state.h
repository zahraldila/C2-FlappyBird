// game_state.h
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include "bird_struct.h"
#include "alexandrio.h"
#include "zahra.h"

void HandleMenuState(GameState *currentState, GameOverState *gameOverState, Bird *bird, int Pipa[3][3], int TutupPipa[3][3]);
void HandleGameplayState(GameState *currentState, GameOverState *gameOverState, Bird *bird, int Pipa[3][3], int TutupPipa[3][3]);
void RenderGameState(GameState currentState, GameOverState gameOverState, Bird *birds, int Pipa[3][3], int TutupPipa[3][3]);

#endif