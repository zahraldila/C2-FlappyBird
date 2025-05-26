#ifndef QLIO_H
#define QLIO_H

#include <raylib.h>

#define MAX_LEADERBOARD_ENTRIES 5
#define MAX_PLAYER_NAME_LENGTH 6 // Hanya 3 huruf untuk sederhana (AAA, BBB, dst.)
                                 // Bisa ditambah jika diinginkan, misal 8 atau 15 + null terminator

extern int score;
extern int highscore;

typedef struct {
    char name[MAX_PLAYER_NAME_LENGTH + 1]; // +1 untuk null terminator
    int scoreValue;
} LeaderboardEntry;

extern LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];

void InitSkor(void);
void TambahSkor(void);
void SimpanHighscore(void); // Menyimpan highscore tunggal (bisa tetap ada)
void TampilkanSkor(Font font);
void ResetSkor(void);

// Fungsi Leaderboard
void LoadLeaderboard(void);
bool IsScoreHighEnough(int newScore); // Fungsi baru untuk cek apakah skor layak masuk
void AddScoreToLeaderboard(const char* playerName, int newScore); // Sekarang menerima nama
void SaveLeaderboard(void);
void DrawLeaderboardScreenContent(Font font);

int bacaHighScore(void);
int bacaHighScoreDariFileTanpaLog(void);

#endif