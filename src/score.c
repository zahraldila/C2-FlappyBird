#include "qlio.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Untuk strcpy, strlen, sprintf

int score = 0;
int highscore = 0;

LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
const char* LEADERBOARD_FILE = "leaderboard_data.txt"; // Nama file bisa diubah
const char* HIGHSCORE_FILE = "highscore.txt";

void InitSkor() {
    highscore = bacaHighScore();
    score = 0;
    LoadLeaderboard();
}

void TambahSkor() {
    score++;
    if (score > highscore) {
        highscore = score;
    }
}

void SimpanHighscore() {
    // ... (fungsi SimpanHighscore tetap sama seperti sebelumnya) ...
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
    // ... (fungsi ini tetap sama) ...
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    int skorFile = 0;
    if (file != NULL) {
        if (fscanf(file, "%d", &skorFile) != 1) skorFile = 0;
        fclose(file);
    }
    return skorFile;
}

int bacaHighScore() {
    // ... (fungsi ini tetap sama) ...
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
    // ... (fungsi ini tetap sama) ...
    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);
    DrawTextEx(font, scoreText, (Vector2){10, 10}, 30, 2, BLACK);

    char highscoreText[32];
    sprintf(highscoreText, "Best: %d", highscore);
    DrawTextEx(font, highscoreText, (Vector2){10, 45}, 25, 2, DARKGRAY);
}


// --- Fungsi Leaderboard dengan Nama ---

static int compareScores(const void *a, const void *b) {
    LeaderboardEntry *entryA = (LeaderboardEntry *)a;
    LeaderboardEntry *entryB = (LeaderboardEntry *)b;
    return (entryB->scoreValue - entryA->scoreValue);
}

void LoadLeaderboard() {
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
        strcpy(leaderboard[i].name, "---"); // Nama default
        leaderboard[i].scoreValue = 0;
    }

    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (file != NULL) {
        for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
            // Format file: NAMA SKOR\n
            if (fscanf(file, "%s %d", leaderboard[i].name, &leaderboard[i].scoreValue) != 2) {
                break; // Akhir file atau format salah
            }
        }
        fclose(file);
        qsort(leaderboard, MAX_LEADERBOARD_ENTRIES, sizeof(LeaderboardEntry), compareScores);
        // TraceLog(LOG_INFO, "Leaderboard dimuat dari %s", LEADERBOARD_FILE);
    } else {
        // TraceLog(LOG_INFO, "File %s tidak ditemukan, memulai leaderboard kosong.", LEADERBOARD_FILE);
    }
}

bool IsScoreHighEnough(int newScore) {
    // Jika leaderboard belum penuh, atau skor baru lebih tinggi dari skor terendah di leaderboard
    if (newScore > 0 && (leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue == 0 || newScore > leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue)) {
        return true;
    }
    // Cek juga jika ada entri yang masih "---" dengan skor 0, yang berarti masih ada slot kosong
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
        if (leaderboard[i].scoreValue == 0 && strcmp(leaderboard[i].name, "---") == 0) {
            return true; // Ada slot kosong, jadi skor berapapun (di atas 0) bisa masuk
        }
    }
    return false;
}


void AddScoreToLeaderboard(const char* playerName, int newScore) {
    if (!IsScoreHighEnough(newScore)) {
        return; // Skor tidak cukup tinggi atau sudah penuh dengan skor lebih tinggi
    }

    // Ganti entri dengan skor terendah (atau entri kosong pertama jika ada)
    // Karena qsort akan mengurutkan, kita cukup ganti yang terakhir (paling rendah)
    // atau entri kosong pertama.
    int replaceIndex = -1;

    // Cari entri "---" dengan skor 0 terlebih dahulu
    for(int i = MAX_LEADERBOARD_ENTRIES - 1; i >= 0; i--) {
        if (leaderboard[i].scoreValue == 0 && strcmp(leaderboard[i].name, "---") == 0) {
            replaceIndex = i; // Ambil slot kosong paling bawah
            break;
        }
    }
    
    // Jika tidak ada slot "---", dan skor baru lebih tinggi dari yang terendah
    if (replaceIndex == -1 && newScore > leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue) {
        replaceIndex = MAX_LEADERBOARD_ENTRIES - 1;
    }


    if (replaceIndex != -1) {
        strncpy(leaderboard[replaceIndex].name, playerName, MAX_PLAYER_NAME_LENGTH);
        leaderboard[replaceIndex].name[MAX_PLAYER_NAME_LENGTH] = '\0'; // Pastikan null-terminated
        leaderboard[replaceIndex].scoreValue = newScore;

        qsort(leaderboard, MAX_LEADERBOARD_ENTRIES, sizeof(LeaderboardEntry), compareScores);
        SaveLeaderboard();
        // TraceLog(LOG_INFO, "Skor %d untuk %s ditambahkan ke leaderboard.", newScore, playerName);
    } else if (newScore > 0 && leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue == 0) {
        // Kasus khusus jika semua slot terisi tapi ada yang skornya 0 (misal dari file lama)
        // dan skor baru lebih besar dari 0.
        strncpy(leaderboard[MAX_LEADERBOARD_ENTRIES - 1].name, playerName, MAX_PLAYER_NAME_LENGTH);
        leaderboard[MAX_LEADERBOARD_ENTRIES - 1].name[MAX_PLAYER_NAME_LENGTH] = '\0';
        leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue = newScore;
        qsort(leaderboard, MAX_LEADERBOARD_ENTRIES, sizeof(LeaderboardEntry), compareScores);
        SaveLeaderboard();
    }
}


void SaveLeaderboard() {
    FILE *file = fopen(LEADERBOARD_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
            // Hanya simpan entri yang valid (skor > 0 atau nama bukan default "---")
            if (leaderboard[i].scoreValue > 0 || strcmp(leaderboard[i].name, "---") != 0) {
                 fprintf(file, "%s %d\n", leaderboard[i].name, leaderboard[i].scoreValue);
            }
        }
        fclose(file);
        // TraceLog(LOG_INFO, "Leaderboard disimpan ke %s", LEADERBOARD_FILE);
    } else {
        // TraceLog(LOG_ERROR, "Gagal menyimpan leaderboard ke %s", LEADERBOARD_FILE);
    }
}

void DrawLeaderboardScreenContent(Font font) {
    const char* title = "LEADERBOARD";
    DrawTextEx(font, title, (Vector2){ GetScreenWidth()/2.0f - MeasureTextEx(font, title, 40, 2).x/2.0f, 50 }, 40, 2, YELLOW);

    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
        char entryText[64];
        // Tampilkan "---" jika nama default dan skor 0
        if (strcmp(leaderboard[i].name, "---") == 0 && leaderboard[i].scoreValue == 0) {
            sprintf(entryText, "%d. --- ..... ---", i + 1);
        } else {
            sprintf(entryText, "%d. %s ..... %d", i + 1, leaderboard[i].name, leaderboard[i].scoreValue);
        }

        Vector2 textSize = MeasureTextEx(font, entryText, 30, 2);
        Color textColor = (strcmp(leaderboard[i].name, "---") == 0 && leaderboard[i].scoreValue == 0) ? GRAY : RAYWHITE;
        DrawTextEx(font, entryText,
                   (Vector2){ GetScreenWidth()/2.0f - textSize.x/2.0f, 120.0f + (i * 40.0f) },
                   30, 2, textColor);
    }
    const char* backInstruction = "Tekan BACKSPACE untuk Kembali ke Menu";
    Vector2 backTextSize = MeasureTextEx(font, backInstruction, 20, 2);
    DrawTextEx(font, backInstruction, (Vector2){ GetScreenWidth()/2.0f - backTextSize.x/2.0f, GetScreenHeight() - 50.0f }, 20, 2, LIGHTGRAY);
}