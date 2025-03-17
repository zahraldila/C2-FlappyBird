// score.c
#include "qlio.h"
#include "alexandrio.h"  // Tambahkan include ini untuk SCREEN_WIDTH
#include <stdio.h>

int score = 0;
int highscore = 0;

void InitSkor() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highscore);
        fclose(file);
    } else {
        highscore = 0;
    }
    score = 0;
}

void TambahSkor() {
    score++;
    if (score > highscore) {
        highscore = score;
        // Simpan highscore langsung
        SimpanHighscore();
    }
}

void SimpanHighscore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highscore);
        fclose(file);
        printf(">> HIGHSCORE TERSIMPAN: %d\n", highscore);
    } else {
        printf(">> ERROR: Tidak dapat menyimpan highscore!\n");
    }
}

void simpanSkorKeFile(int skor) {
    if (skor > highscore) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", skor);
            fclose(file);
            highscore = skor;
            printf(">> HIGHSCORE BARU TERSIMPAN: %d\n", skor);
        } else {
            printf(">> ERROR: Tidak dapat menyimpan skor!\n");
        }
    }
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