// score.c
#include "qlio.h"
#include "alexandrio.h"
#include <stdio.h>

int score = 0;
int highscore = 0;

void InitSkor() {
    // Baca highscore dari file
    highscore = bacaHighScore();
    score = 0;
}

void TambahSkor() {
    score++;
    // Perbarui highscore jika score lebih tinggi
    if (score > highscore) {
        highscore = score;
    }
}

void SimpanHighscore() {
    // Hanya simpan jika highscore saat ini berbeda dari yang ada di file
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

// Fungsi tambahan untuk membaca highscore tanpa print log
int bacaHighScoreDariFileTanpaLog() {
    FILE *file = fopen("highscore.txt", "r");
    int skor = 0;
    
    if (file != NULL) {
        fscanf(file, "%d", &skor);
        fclose(file);
    }
    
    return skor;
}

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

void ResetSkor() {
    score = 0;
}

void TampilkanSkor(Font font) {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    DrawTextEx(font, scoreText, (Vector2){LEBAR_LAYAR/2 - 80, 10}, 30, 2, BLACK);
    
    char highscoreText[20];
    sprintf(highscoreText, "Best: %d", highscore);
    DrawTextEx(font, highscoreText, (Vector2){LEBAR_LAYAR/2 - 60, 45}, 25, 2, DARKGRAY);
}