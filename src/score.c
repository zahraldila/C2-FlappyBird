// score.c
#include "qlio.h"
#include "alexandrio.h"  // Untuk LEBAR_LAYAR (SCREEN_WIDTH)
#include <stdio.h>
#include <raylib.h>      // Untuk Font dan DrawTextEx

int score = 0;
int highscore = 0;

// Fungsi membaca highscore dari file dengan log
int bacaHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    int skor = 0;

    if (file != NULL) {
        fscanf(file, "%d", &skor);
        fclose(file);
        printf(">> HIGHSCORE TERBACA: %d\n", skor);
    } else {
        printf(">> INFO: File highscore belum ada, mulai dari 0\n");
    }

    return skor;
}

// Fungsi membaca highscore tanpa log (untuk internal checking)
int bacaHighScoreDariFileTanpaLog() {
    FILE *file = fopen("highscore.txt", "r");
    int skor = 0;

    if (file != NULL) {
        fscanf(file, "%d", &skor);
        fclose(file);
    }

    return skor;
}

// Inisialisasi skor awal
void InitSkor() {
    highscore = bacaHighScore();
    score = 0;
}

// Tambah skor dan cek apakah melebihi highscore
void TambahSkor() {
    score++;
    if (score > highscore) {
        highscore = score;
        SimpanHighscore();
    }
}

// Menyimpan highscore ke file jika lebih besar dari sebelumnya
void SimpanHighscore() {
    int fileHighscore = bacaHighScoreDariFileTanpaLog();
    if (highscore > fileHighscore) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", highscore);
            fclose(file);
            printf(">> HIGHSCORE TERSIMPAN: %d\n", highscore);
        } else {
            printf(">> ERROR: Tidak dapat menyimpan highscore!\n");
        }
    }
}

// Mereset skor ke 0
void ResetSkor() {
    score = 0;
}

// Menampilkan skor dan highscore di layar
void TampilkanSkor(Font font) {
    char scoreText[32];
    sprintf(scoreText, "Score: %d", score);
    DrawTextEx(font, scoreText, (Vector2){LEBAR_LAYAR / 2 - 80, 10}, 30, 2, BLACK);

    char highscoreText[32];
    sprintf(highscoreText, "Best: %d", highscore);
    DrawTextEx(font, highscoreText, (Vector2){LEBAR_LAYAR / 2 - 60, 45}, 25, 2, DARKGRAY);
}
