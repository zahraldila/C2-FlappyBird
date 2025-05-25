#include "qlio.h"
#include "raylib.h" // Untuk DrawText, MeasureText, Font
#include <stdio.h>
#include <stdlib.h> // Untuk qsort
#include <string.h> // Untuk sprintf

int score = 0;
int highscore = 0; // Highscore tunggal

LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
const char* LEADERBOARD_FILE = "leaderboard.txt"; // File baru untuk multiple scores
const char* HIGHSCORE_FILE = "highscore.txt";   // File lama untuk single highscore

void InitSkor() {
    highscore = bacaHighScore(); // Baca highscore tunggal dari file lama
    score = 0;
    LoadLeaderboard(); // Muat leaderboard baru
}

void TambahSkor() {
    score++;
    if (score > highscore) {
        highscore = score;
    }
}

// Menyimpan highscore tunggal (fungsi lama, bisa dipertahankan atau dihapus jika leaderboard sudah cukup)
void SimpanHighscore() {
    int fileHighscore = bacaHighScoreDariFileTanpaLog();
    if (highscore > fileHighscore) {
        FILE *file = fopen(HIGHSCORE_FILE, "w");
        if (file != NULL) {
            fprintf(file, "%d", highscore);
            fclose(file);
            // TraceLog(LOG_INFO, ">> HIGHSCORE TERSIMPAN: %d", highscore);
        } else {
            // TraceLog(LOG_ERROR, ">> ERROR: Tidak dapat menyimpan highscore!");
        }
    }
}

int bacaHighScoreDariFileTanpaLog() {
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    int skorFile = 0;
    if (file != NULL) {
        if (fscanf(file, "%d", &skorFile) != 1) {
            skorFile = 0; // Jika gagal baca, anggap 0
        }
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
            // TraceLog(LOG_WARNING, ">> Gagal membaca highscore dari file, mulai dari 0");
            skorFile = 0;
        }
        fclose(file);
    } else {
        // TraceLog(LOG_INFO, ">> INFO: File highscore.txt belum ada, mulai dari 0");
    }
    return skorFile;
}

void ResetSkor() {
    score = 0;
}

void TampilkanSkor(Font font) { // Menampilkan skor saat game berjalan
    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);
    DrawTextEx(font, scoreText, (Vector2){10, 10}, 30, 2, BLACK);

    char highscoreText[32];
    sprintf(highscoreText, "Best: %d", highscore); // Menampilkan highscore tunggal
    DrawTextEx(font, highscoreText, (Vector2){10, 45}, 25, 2, DARKGRAY);
}


// --- Fungsi Leaderboard BARU ---

// Fungsi pembanding untuk qsort (mengurutkan dari tertinggi ke terendah)
static int compareScores(const void *a, const void *b) {
    LeaderboardEntry *entryA = (LeaderboardEntry *)a;
    LeaderboardEntry *entryB = (LeaderboardEntry *)b;
    return (entryB->scoreValue - entryA->scoreValue); // Descending
}

void LoadLeaderboard() {
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
        leaderboard[i].scoreValue = 0; // Inisialisasi dengan skor 0
        // strcpy(leaderboard[i].name, "---"); // Jika pakai nama
    }

    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (file != NULL) {
        for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
            // Format: SKOR (tanpa nama untuk sekarang)
            if (fscanf(file, "%d", &leaderboard[i].scoreValue) != 1) {
                // Jika gagal baca atau akhir file, berhenti
                break;
            }
        }
        fclose(file);
        // Urutkan setelah memuat untuk memastikan
        qsort(leaderboard, MAX_LEADERBOARD_ENTRIES, sizeof(LeaderboardEntry), compareScores);
        // TraceLog(LOG_INFO, "Leaderboard dimuat dari %s", LEADERBOARD_FILE);
    } else {
        // TraceLog(LOG_INFO, "File leaderboard.txt tidak ditemukan, memulai dengan leaderboard kosong.");
        // File akan dibuat saat SaveLeaderboard() pertama kali.
    }
}

void AddScoreToLeaderboard(int newScore) {
    // Cek apakah skor baru layak masuk leaderboard
    if (newScore > leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue) {
        leaderboard[MAX_LEADERBOARD_ENTRIES - 1].scoreValue = newScore;
        // strcpy(leaderboard[MAX_LEADERBOARD_ENTRIES - 1].name, "YOU"); // Placeholder nama

        // Urutkan leaderboard
        qsort(leaderboard, MAX_LEADERBOARD_ENTRIES, sizeof(LeaderboardEntry), compareScores);
        SaveLeaderboard(); // Simpan perubahan ke file
        // TraceLog(LOG_INFO, "Skor %d ditambahkan ke leaderboard.", newScore);
    }
}

void SaveLeaderboard() {
    FILE *file = fopen(LEADERBOARD_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
            if (leaderboard[i].scoreValue > 0) { // Hanya simpan skor yang valid
                fprintf(file, "%d\n", leaderboard[i].scoreValue);
            }
        }
        fclose(file);
        // TraceLog(LOG_INFO, "Leaderboard disimpan ke %s", LEADERBOARD_FILE);
    } else {
        // TraceLog(LOG_ERROR, "Gagal menyimpan leaderboard ke %s", LEADERBOARD_FILE);
    }
}

void DrawLeaderboardScreenContent(Font font) { // Fungsi untuk menggambar isi layar leaderboard
    // Judul
    const char* title = "LEADERBOARD";
    DrawTextEx(font, title, (Vector2){ GetScreenWidth()/2.0f - MeasureTextEx(font, title, 40, 2).x/2.0f, 50 }, 40, 2, YELLOW);

    // Tampilkan Top 5 skor
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
        if (leaderboard[i].scoreValue > 0) { // Hanya tampilkan jika ada skor
            char entryText[64];
            // sprintf(entryText, "%d. %s ..... %d", i + 1, leaderboard[i].name, leaderboard[i].scoreValue); // Jika pakai nama
            sprintf(entryText, "%d. ..... %d", i + 1, leaderboard[i].scoreValue);

            Vector2 textSize = MeasureTextEx(font, entryText, 30, 2);
            DrawTextEx(font, entryText,
                       (Vector2){ GetScreenWidth()/2.0f - textSize.x/2.0f, 120.0f + (i * 40.0f) },
                       30, 2, RAYWHITE);
        } else { // Jika entri kosong atau skor 0
            char entryText[64];
            sprintf(entryText, "%d. ..... ---", i + 1);
             Vector2 textSize = MeasureTextEx(font, entryText, 30, 2);
            DrawTextEx(font, entryText,
                       (Vector2){ GetScreenWidth()/2.0f - textSize.x/2.0f, 120.0f + (i * 40.0f) },
                       30, 2, GRAY);
        }
    }
    // Instruksi kembali
    const char* backInstruction = "Tekan BACKSPACE untuk Kembali ke Menu";
    Vector2 backTextSize = MeasureTextEx(font, backInstruction, 20, 2);
    DrawTextEx(font, backInstruction, (Vector2){ GetScreenWidth()/2.0f - backTextSize.x/2.0f, GetScreenHeight() - 50.0f }, 20, 2, LIGHTGRAY);
}