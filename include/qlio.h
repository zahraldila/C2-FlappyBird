#ifndef QLIO_H
#define QLIO_H

#include <raylib.h>

extern int score;
extern int highscore;

// Struktur LeaderboardEntry dan array leaderboard dihapus dari sini.
// Sekarang dideklarasikan di leaderboard.h

void InitSkor(void);
void TambahSkor(void);
void SimpanHighscore(void); // Menyimpan highscore tunggal (bisa tetap ada)
void TampilkanSkor(Font font);
void ResetSkor(void);

// Fungsi Leaderboard yang berhubungan langsung dengan leaderboard.h
// Fungsi-fungsi ini akan dihapus dari sini dan dideklarasikan di leaderboard.h
// void LoadLeaderboard(void); // Dihapus
// bool IsScoreHighEnough(int newScore); // Dihapus
// void AddScoreToLeaderboard(const char* playerName, int newScore); // Dihapus
// void SaveLeaderboard(void); // Dihapus
// void DrawLeaderboardScreenContent(Font font); // Dihapus (sekarang ada di leaderboard.h)

int bacaHighScore(void);
int bacaHighScoreDariFileTanpaLog(void);

#endif