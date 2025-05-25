#include "zakky.h"
#include "dava.h"    // Untuk BackgroundSelector dan fungsi Next/PreviousBackground
#include "qlio.h"
#include <stdlib.h>  // Untuk exit() jika masih dipakai
#include <stdio.h>   // Untuk TraceLog

// Variabel untuk tombol kembali di layar pemilihan background
static Rectangle backButtonRec_BgSelect = { 20, 20, 150, 40 }; // Posisi kiri atas

void DrawGameOver(int screenWidth, int screenHeight, int currentScore) {
    const char* gameOverText = "GAME OVER";
    const char* scoreTextMsg = TextFormat("SCORE: %d", currentScore);
    const char* restartText = "Press ENTER to Restart";
    const char* menuText = "Press BACKSPACE for Menu";

    DrawText(gameOverText, screenWidth/2 - MeasureText(gameOverText, 40)/2, screenHeight/2 - 60, 40, RED);
    DrawText(scoreTextMsg, screenWidth/2 - MeasureText(scoreTextMsg, 20)/2, screenHeight/2 - 10, 20, BLACK);
    DrawText(restartText, screenWidth/2 - MeasureText(restartText, 20)/2, screenHeight/2 + 20, 20, DARKGRAY);
    DrawText(menuText, screenWidth/2 - MeasureText(menuText, 20)/2, screenHeight/2 + 50, 20, DARKGRAY);
}