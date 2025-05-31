#ifndef QLIO_H
#define QLIO_H

#include <raylib.h>
#include "zakky.h"

extern int score;
extern int highscore;

void InitSkor(void);
void TambahSkor(void);
void SimpanHighscore(void); // Menyimpan highscore tunggal
void TampilkanSkor(Font font);
void ResetSkor(void);
GameState UpdateLeaderboardScreen(GameState currentGameState);
void DrawLeaderboardScreen(Font font); // Butuh font untuk menggambar teks
int bacaHighScore(void);
int bacaHighScoreDariFileTanpaLog(void);
void InitLeaderboardScreen(void); // Mungkin tidak perlu jika LoadLeaderboard sudah cukup
GameState UpdateLeaderboardScreen(GameState currentGameState);
void DrawLeaderboardScreen(Font font); // Butuh font untuk menggambar teks
void DrawCreditScreen(Font font);
GameState UpdateCreditScreen(GameState currentGameState);


#endif