#ifndef QLIO_H
#define QLIO_H

#include <raylib.h>

extern int score;
extern int highscore;

void InitSkor(void);
void TambahSkor(void);
void SimpanHighscore(void); // Menyimpan highscore tunggal
void TampilkanSkor(Font font);
void ResetSkor(void);

int bacaHighScore(void);
int bacaHighScoreDariFileTanpaLog(void);

#endif