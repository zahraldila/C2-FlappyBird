// game_state.h
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include "bird_struct.h"
#include "alexandrio.h"
#include "zahra.h"
#include "zakky.h"     // Untuk fungsi UpdateBird dan struktur pipa
#include "dava.h"      // Untuk Gambar_pipa dan pergerakan pipa

void HandleMenuState(GameState *currentState, GameOverState *gameOverState, Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist);
void HandleGameplayState(GameState *currentState, GameOverState *gameOverState, Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist);
void RenderGameState(GameState currentState, GameOverState gameOverState, Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist);
void ResetGame(Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist); 

void DrawGameOver(int screenWidth, int screenHeight, int currentScore);

#endif
