#ifndef QLIO_H
#define QLIO_H

#include <raylib.h>

extern int score;
extern int highscore;

void InitSkor();
void TambahSkor();
void SimpanHighscore();
void TampilkanSkor(Font font);
void ResetSkor();

int bacaHighScore();
void simpanSkorKeFile(int skor);

#endif