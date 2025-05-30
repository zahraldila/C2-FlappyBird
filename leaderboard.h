#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "raylib.h"
#include <stdbool.h> // Untuk bool

#define MAX_LEADERBOARD_ENTRIES 10 // 10 skor teratas
#define MAX_PLAYER_NAME_LENGTH 6   // misalnya, AAA, BBB, hingga 6 karakter

// Struktur untuk setiap entri di papan peringkat
typedef struct LeaderboardEntry {
    char name[MAX_PLAYER_NAME_LENGTH + 1]; // +1 untuk terminator nol
    int scoreValue;
} LeaderboardEntry;

// Node untuk Doubly Linked List
typedef struct LeaderboardNode {
    LeaderboardEntry data;
    struct LeaderboardNode *next;
    struct LeaderboardNode *prev;
} LeaderboardNode;

// Struktur untuk Doubly Linked List Leaderboard
typedef struct LeaderboardList {
    LeaderboardNode *head;
    LeaderboardNode *tail;
    int count;
} LeaderboardList;

extern LeaderboardList *g_leaderboardList; // Pointer global ke daftar papan peringkat

// Fungsi-fungsi untuk manajemen papan peringkat
void InitLeaderboard(void);
void FreeLeaderboard(void);
void LoadLeaderboard(void);
void SaveLeaderboard(void);
void AddScoreToLeaderboard(const char* playerName, int newScore);
bool IsScoreHighEnough(int newScore);
void DrawLeaderboardContent(Font font);

#endif // LEADERBOARD_H
