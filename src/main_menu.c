#include "zakky.h"
#include "dava.h"
#include "qlio.h" // For DrawLeaderboardScreenContent
#include "leaderboard.h" // Include the new leaderboard header
#include <stdlib.h>
#include <stdio.h>

static Rectangle backButtonRec_BgSelect = { 20, 20, 150, 40 };
// Tombol kembali untuk leaderboard, bisa sama atau beda
static Rectangle backButtonRec_Leaderboard = { 20, 20, 150, 40 };
static Rectangle backButtonRec_Credit = { 20, 20, 150, 40 };


void setupMenu()
{
    GameState state = MENU;

    DrawText("Flappy Bird", LEBAR_LAYAR / 2 - 170, 100, 60, YELLOW);

    // Gambar tombol "START"
    Rectangle startButton = { LEBAR_LAYAR / 3 - 120, 250, 220, 50 };
    DrawRectangleRec(startButton, BLUE);
    DrawText("Start", LEBAR_LAYAR / 3 - 40, 265, 20, WHITE);

    Rectangle scoreButton = { LEBAR_LAYAR / 2, 250, 220, 50 };
    DrawRectangleRec(scoreButton, BLUE);
    DrawText("Leaderboard", LEBAR_LAYAR / 2 + 50, 265, 20, WHITE);

    // Gambar tombol "choose background"
    Rectangle bckButton = { LEBAR_LAYAR / 3 - 120, 320, 220, 50 };
    DrawRectangleRec(bckButton, BLUE);
    DrawText("Choose background", LEBAR_LAYAR / 3 - 110, 335, 20, WHITE);

    Rectangle helpButton = { LEBAR_LAYAR / 2, 320, 220, 50 };
    DrawRectangleRec(helpButton, GREEN);
    DrawText("Help", LEBAR_LAYAR / 2 + 90, 335, 20, WHITE);

    Rectangle exitButton = { LEBAR_LAYAR / 2 - 100, 380, 190, 50 };
    DrawRectangleRec(exitButton, RED);
    DrawText("Exit", LEBAR_LAYAR / 2 - 25, 395, 20, WHITE);

    Rectangle creditButton = { 740, 20, 40, 40 };
    DrawRectangleRec(creditButton, DARKBLUE);
    DrawText("C", 750, 25, 30, WHITE);

}

GameState pilihMenu()
{
    GameState nextState = MENU;
    Vector2 mousePoint = GetMousePosition();

    Rectangle startButton = { LEBAR_LAYAR / 3 - 100, 250, 200, 50 };
    Rectangle exitButton = { LEBAR_LAYAR / 2 - 100, 380, 200, 50 };
    Rectangle scoreButton = { LEBAR_LAYAR / 2, 250, 200, 50 };
    Rectangle bckButton = { LEBAR_LAYAR / 3 - 100, 320, 200, 50 };
    Rectangle helpButton = { LEBAR_LAYAR / 2, 320, 200, 50 };
    Rectangle creditButton = { 740, 20, 40, 40 };


    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePoint, startButton))
        {
            nextState = GAMEPLAY;
        }
        if (CheckCollisionPointRec(mousePoint, bckButton))
        {
            nextState = BACKGROUND_SELECTION;
        }
        if (CheckCollisionPointRec(mousePoint, scoreButton))
        {
            nextState = LEADERBOARD;
            LoadLeaderboard(); // Load leaderboard data when entering leaderboard screen
        }

        if (CheckCollisionPointRec(mousePoint, exitButton))
        {
            TraceLog(LOG_INFO, "Tombol EXIT ditekan. Menutup window.");
            CloseWindow();
            exit(0);
        }

        if (CheckCollisionPointRec(mousePoint, helpButton))
        {
            nextState = STATE_HELP;
        }

        if (CheckCollisionPointRec(mousePoint, creditButton))
        {
            nextState = CREDIT;
        }
        
    }
    return nextState;
}

GameState DrawMenu() 
{
    setupMenu();
    return pilihMenu();
}

void DrawCreditScreen(Font font) {
    const char *titleText = "CREDITS";
    DrawTextEx(font, titleText, (Vector2){ (float)GetScreenWidth()/2 - MeasureTextEx(font, titleText, 40, 2).x/2, 50 }, 40, 2, YELLOW); // Menggunakan DrawTextEx untuk konsistensi dengan font

    const char* name1 = "1. Alexandrio Vega Bonito (241511067)";
    const char* name2 = "2. Dava Ramadhan (241511070)";
    const char* name3 = "3. Qlio Amanda Febriany (241511087)";
    const char* name4 = "4. Zahra Aldila (241511094)";
    const char* name5 = "5. Zakky Zhillan M.I (241511095)";
    
    int fontSize = 20;
    int startY = 150;
    int spacing = 40;

    DrawTextEx(font, name1, (Vector2){ (float)GetScreenWidth()/2 - MeasureTextEx(font, name1, fontSize, 1).x/2, (float)startY }, fontSize, 1, RAYWHITE);
    DrawTextEx(font, name2, (Vector2){ (float)GetScreenWidth()/2 - MeasureTextEx(font, name2, fontSize, 1).x/2, (float)(startY + spacing * 1) }, fontSize, 1, RAYWHITE);
    DrawTextEx(font, name3, (Vector2){ (float)GetScreenWidth()/2 - MeasureTextEx(font, name3, fontSize, 1).x/2, (float)(startY + spacing * 2) }, fontSize, 1, RAYWHITE);
    DrawTextEx(font, name4, (Vector2){ (float)GetScreenWidth()/2 - MeasureTextEx(font, name4, fontSize, 1).x/2, (float)(startY + spacing * 3) }, fontSize, 1, RAYWHITE);
    DrawTextEx(font, name5, (Vector2){ (float)GetScreenWidth()/2 - MeasureTextEx(font, name5, fontSize, 1).x/2, (float)(startY + spacing * 4) }, fontSize, 1, RAYWHITE);

    // Gambar tombol kembali
    DrawRectangleRec(backButtonRec_Credit, Fade(DARKBLUE, 0.7f));
    DrawTextEx(font, "KEMBALI", (Vector2){ backButtonRec_Credit.x + backButtonRec_Credit.width/2 - MeasureTextEx(font, "KEMBALI", 20, 1).x/2, backButtonRec_Credit.y + 10 }, 20, 1, WHITE);
}

GameState UpdateCreditScreen(GameState currentGameState) 
{
    GameState nextState = currentGameState;
    Vector2 mousePoint = GetMousePosition();

    // Cek jika tombol "KEMBALI" diklik
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, backButtonRec_Credit)) {
        nextState = MENU;
    }

    // Cek jika tombol KEY_BACKSPACE atau KEY_ESCAPE ditekan
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)) {
        nextState = MENU;
    }
    return nextState;
}

void jedapause (PauseState *tmblPause) 
{ 
    tmblPause->isPause = false; 
}

void tombolpause (PauseState *tmblPause) 
{ 
    tmblPause->isPause = !tmblPause->isPause; 
}

void DrawPauseScreen(PauseState *tmblPause) {
    if (tmblPause->isPause) {
        DrawRectangle(0,0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        const char* pauseText = "GAME PAUSED";
        const char* resumeText = "Tekan P untuk Melanjutkan";
        DrawText(pauseText, GetScreenWidth()/2 - MeasureText(pauseText, 40)/2, GetScreenHeight()/2 - 40, 40, RAYWHITE);
        DrawText(resumeText, GetScreenWidth()/2 - MeasureText(resumeText, 20)/2, GetScreenHeight()/2 + 10, 20, RAYWHITE);
    }
}

void InitBackgroundSelectionScreen() { TraceLog(LOG_INFO, "Memasuki layar pemilihan background."); }

GameState UpdateBackgroundSelectionScreen(BackgroundSelector *bgSelector, GameState currentGameState) {
    GameState nextState = currentGameState;
    if (IsKeyPressed(KEY_RIGHT)) NextBackground(bgSelector);
    if (IsKeyPressed(KEY_LEFT)) PreviousBackground(bgSelector);
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE) ||
        (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), backButtonRec_BgSelect))) {
        nextState = MENU;
        if(IsKeyPressed(KEY_ENTER)) TraceLog(LOG_INFO, "Background dipilih: %s", bgSelector->current->filePath);
    }
    return nextState;
}

void DrawBackgroundSelectionScreen(BackgroundSelector *bgSelector) {
    if (bgSelector && bgSelector->current && bgSelector->current->texture.id != 0) {
        DrawTexture(bgSelector->current->texture, 0, 0, WHITE);
    } else {
        DrawText("Gagal memuat preview background", 50, GetScreenHeight()/2 - 20, 20, RED);
    }
    const char *titleText = "PILIH BACKGROUND";
    DrawText(titleText, GetScreenWidth()/2 - MeasureText(titleText, 40)/2, 30, 40, YELLOW);
    DrawRectangle(0, GetScreenHeight() - 70, GetScreenWidth(), 70, Fade(BLACK, 0.5f));
    DrawText("KIRI/KANAN: Ganti | ENTER: Pilih & Kembali", 50, GetScreenHeight() - 55, 20, RAYWHITE);
    DrawRectangleRec(backButtonRec_BgSelect, Fade(DARKBLUE, 0.7f));
    DrawText("KEMBALI", backButtonRec_BgSelect.x + backButtonRec_BgSelect.width/2 - MeasureText("KEMBALI", 20)/2, backButtonRec_BgSelect.y + 10, 20, WHITE);
}

// Fungsi untuk Layar Leaderboard 
void InitLeaderboardScreen() {
    LoadLeaderboard(); // Pastikan data terbaru dimuat saat masuk layar
    TraceLog(LOG_INFO, "Memasuki layar leaderboard.");
}

GameState UpdateLeaderboardScreen(GameState currentGameState) {
    GameState nextState = currentGameState;
    // Back to menu when BACKSPACE is pressed or back button is clicked
    if (IsKeyPressed(KEY_BACKSPACE) ||
        (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), backButtonRec_Leaderboard))) {
        nextState = MENU;
    }
    return nextState;
}

void DrawLeaderboardScreen(Font font) { // Menerima Font sebagai parameter


    DrawLeaderboardContent(font); // Memanggil fungsi dari leaderboard.c

    // Tombol "Kembali"
    DrawRectangleRec(backButtonRec_Leaderboard, Fade(DARKBLUE, 0.7f));
    DrawText("KEMBALI", backButtonRec_Leaderboard.x + backButtonRec_Leaderboard.width/2 - MeasureText("KEMBALI", 20)/2, backButtonRec_Leaderboard.y + 10, 20, WHITE);
}