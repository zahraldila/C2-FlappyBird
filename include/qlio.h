#ifndef QLIO_H
#define QLIO_H

#include "raylib.h"

#define MAX_LEADERBOARD_ENTRIES 5 // Jumlah skor yang disimpan di leaderboard
#define MAX_PLAYER_NAME_LENGTH 6 

extern int score;
extern int highscore; // highscore tunggal masih bisa dipertahankan untuk tampilan cepat

// Struktur untuk satu entri di leaderboard
typedef struct {
    // char name[16]; // Untuk nama pemain (opsional, bisa ditambahkan nanti)
    int scoreValue;
} LeaderboardEntry;

extern LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES]; // Array global untuk leaderboard

void InitSkor(void); // Inisialisasi skor dan highscore tunggal
void TambahSkor(void);
void SimpanHighscore(void); // Menyimpan highscore tunggal
void TampilkanSkor(Font font); // Menampilkan skor saat ini & highscore tunggal
void ResetSkor(void);

// Fungsi baru untuk Leaderboard
void LoadLeaderboard(void);    // Memuat leaderboard dari file
void AddScoreToLeaderboard(int newScore); // Menambahkan skor baru & mengurutkan
void SaveLeaderboard(void);    // Menyimpan leaderboard ke file
void DrawLeaderboardScreenContent(Font font); // Fungsi untuk menggambar isi leaderboard

// Fungsi lama yang mungkin masih relevan
int bacaHighScore(void); // Mungkin untuk highscore tunggal
int bacaHighScoreDariFileTanpaLog(void);

#endif