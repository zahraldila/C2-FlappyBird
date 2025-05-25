#include "zakky.h"
#include "dava.h"
#include "qlio.h" // Untuk DrawLeaderboardScreenContent
#include <stdlib.h>
#include <stdio.h>

static Rectangle backButtonRec_BgSelect = { 20, 20, 150, 40 };
// Tombol kembali untuk leaderboard, bisa sama atau beda
static Rectangle backButtonRec_Leaderboard = { 20, 20, 150, 40 };


void setupMenu() {
    DrawText("Flappy Bird", LEBAR_LAYAR / 2 - MeasureText("Flappy Bird", 60) / 2, 50, 60, YELLOW);

    float buttonWidth = 250;
    float buttonHeight = 50;
    float spacing = 10; // Jarak antar tombol
    float startY = 150;
    float currentY = startY;

    Rectangle startButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    DrawRectangleRec(startButton, BLUE);
    DrawText("START GAME", startButton.x + startButton.width / 2 - MeasureText("START GAME", 20) / 2, startButton.y + 15, 20, WHITE);
    currentY += buttonHeight + spacing;

    Rectangle bckButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    DrawRectangleRec(bckButton, BLUE);
    DrawText("CHOOSE BACKGROUND", bckButton.x + bckButton.width/2 - MeasureText("CHOOSE BACKGROUND",20)/2, bckButton.y + 15, 20, WHITE);
    currentY += buttonHeight + spacing;

    Rectangle skinButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    DrawRectangleRec(skinButton, BLUE);
    DrawText("CHOOSE SKIN", skinButton.x + skinButton.width/2 - MeasureText("CHOOSE SKIN",20)/2, skinButton.y + 15, 20, WHITE);
    currentY += buttonHeight + spacing;

    Rectangle scoreButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    DrawRectangleRec(scoreButton, DARKBLUE); // Warna beda untuk tombol aktif
    DrawText("LEADERBOARD", scoreButton.x + scoreButton.width/2 - MeasureText("LEADERBOARD",20)/2, scoreButton.y + 15, 20, WHITE);
    currentY += buttonHeight + spacing;

    Rectangle exitButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    DrawRectangleRec(exitButton, RED);
    DrawText("EXIT GAME", exitButton.x + exitButton.width / 2 - MeasureText("EXIT GAME", 20) / 2, exitButton.y + 15, 20, WHITE);
}

GameState pilihMenu() {
    GameState nextState = MENU;
    Vector2 mousePoint = GetMousePosition();

    float buttonWidth = 250;
    float buttonHeight = 50;
    float spacing = 10;
    float startY = 150;
    float currentY = startY;

    Rectangle startButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    currentY += buttonHeight + spacing;
    Rectangle bckButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    currentY += buttonHeight + spacing;
    Rectangle skinButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    currentY += buttonHeight + spacing;
    Rectangle scoreButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };
    currentY += buttonHeight + spacing;
    Rectangle exitButton = { LEBAR_LAYAR / 2 - buttonWidth / 2, currentY, buttonWidth, buttonHeight };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, startButton)) nextState = GAMEPLAY;
        if (CheckCollisionPointRec(mousePoint, bckButton)) nextState = BACKGROUND_SELECTION;
        if (CheckCollisionPointRec(mousePoint, skinButton)) {
            // nextState = SKIN;
            TraceLog(LOG_INFO, "Tombol CHOOSE SKIN ditekan (placeholder).");
        }
        if (CheckCollisionPointRec(mousePoint, scoreButton )) {
            nextState = LEADERBOARD; // Arahkan ke state leaderboard
            LoadLeaderboard(); // Muat data leaderboard saat tombol ditekan
        }
        if (CheckCollisionPointRec(mousePoint, exitButton)) {
            TraceLog(LOG_INFO, "Tombol EXIT ditekan. Menutup window.");
            CloseWindow();
        }
    }
    return nextState;
}

GameState DrawMenu() {
    setupMenu();
    return pilihMenu();
}

void jedapause (PauseState *tmblPause) { tmblPause->isPause = false; }
void tombolpause (PauseState *tmblPause) { tmblPause->isPause = !tmblPause->isPause; }

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
    DrawText("KIRI/KANAN: Ganti | ENTER: Pilih | BACKSPACE: Kembali", 50, GetScreenHeight() - 55, 20, RAYWHITE);
    DrawRectangleRec(backButtonRec_BgSelect, Fade(DARKBLUE, 0.7f));
    DrawText("KEMBALI", backButtonRec_BgSelect.x + backButtonRec_BgSelect.width/2 - MeasureText("KEMBALI", 20)/2, backButtonRec_BgSelect.y + 10, 20, WHITE);
}

// --- Fungsi untuk Layar Leaderboard ---
void InitLeaderboardScreen() {
    LoadLeaderboard(); // Pastikan data terbaru dimuat saat masuk layar
    TraceLog(LOG_INFO, "Memasuki layar leaderboard.");
}

GameState UpdateLeaderboardScreen(GameState currentGameState) {
    GameState nextState = currentGameState;
    if (IsKeyPressed(KEY_BACKSPACE) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), backButtonRec_Leaderboard))) {
        nextState = MENU;
    }
    return nextState;
}

void DrawLeaderboardScreen(Font font) { // Menerima Font sebagai parameter
    // Gambar background menu (atau background statis khusus leaderboard)
    // ClearBackground(DARKGRAY); // Contoh background sederhana
    // Untuk konsistensi, bisa gambar background yang sama dengan menu atau background game default
    // LoopDrawSelectedBackground(backgroundSelector, &bgX_menu_dummy); // Jika ingin background game di sini

    DrawLeaderboardScreenContent(font); // Memanggil fungsi dari qlio.c (via qlio.h)

    // Tombol "Kembali"
    DrawRectangleRec(backButtonRec_Leaderboard, Fade(DARKBLUE, 0.7f));
    DrawText("KEMBALI", backButtonRec_Leaderboard.x + backButtonRec_Leaderboard.width/2 - MeasureText("KEMBALI", 20)/2, backButtonRec_Leaderboard.y + 10, 20, WHITE);
}