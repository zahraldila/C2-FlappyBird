// score.c
#include "qlio.h"
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
    }
}

void SimpanHighscore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highscore);
        fclose(file);
    }
}

void simpanSkorKeFile(int skor) {
    if (skor > highscore) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", skor);
            fclose(file);
            highscore = skor;
        }
    }
}

int bacaHighScore() {
    return highscore;
}
