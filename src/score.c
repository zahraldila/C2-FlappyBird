#include "qlio.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Untuk strcpy, strlen, sprintf

int score = 0;
int highscore = 0;

// LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES]; // HAPUS BARIS INI
const char* LEADERBOARD_FILE = "leaderboard_data.txt"; // Konstanta ini sekarang hanya digunakan oleh leaderboard.c
const char* HIGHSCORE_FILE = "highscore.txt";

void InitSkor() {
    highscore = bacaHighScore();
    score = 0;
    // LoadLeaderboard(); // HAPUS PANGGILAN INI. InitLeaderboard akan dipanggil di main.c
}

void TambahSkor() {
    score++;
    if (score > highscore) {
        highscore = score;
    }
}

void SimpanHighscore() {
    int fileHighscore = bacaHighScoreDariFileTanpaLog();
    if (highscore > fileHighscore) {
        FILE *file = fopen(HIGHSCORE_FILE, "w");
        if (file != NULL) {
            fprintf(file, "%d", highscore);
            fclose(file);
        }
    }
}


int bacaHighScoreDariFileTanpaLog() {
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    int skorFile = 0;
    if (file != NULL) {
        if (fscanf(file, "%d", &skorFile) != 1) skorFile = 0;
        fclose(file);
    }
    return skorFile;
}

int bacaHighScore() {
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    int skorFile = 0;
    if (file != NULL) {
        if (fscanf(file, "%d", &skorFile) == 1) {
            // TraceLog(LOG_INFO, ">> HIGHSCORE TERBACA: %d", skorFile);
        } else {
            skorFile = 0;
        }
        fclose(file);
    }
    return skorFile;
}

void ResetSkor() {
    score = 0;
}

void TampilkanSkor(Font font) {
    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);
    DrawTextEx(font, scoreText, (Vector2){10, 10}, 30, 2, BLACK);

    char highscoreText[32];
    sprintf(highscoreText, "Best: %d", highscore);
    DrawTextEx(font, highscoreText, (Vector2){10, 45}, 25, 2, DARKGRAY);
}

// HAPUS SEMUA FUNGSI BERIKUT DARI SINI:
/*
static int compareScores(const void *a, const void *b) { ... }
void LoadLeaderboard() { ... }
bool IsScoreHighEnough(int newScore) { ... }
void AddScoreToLeaderboard(const char* playerName, int newScore) { ... }
void SaveLeaderboard() { ... }
void DrawLeaderboardScreenContent(Font font) { ... }
*/
