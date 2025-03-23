#include "zakky.h"
#include <stdlib.h>  // Tambahkan ini untuk exit()

void setupMenu(int screenWidth, int screenHeight) 
{
    GameState state = MENU;

    DrawText("Flappy Bird", screenWidth / 2 - 170, 100, 60, DARKBLUE);

    // Gambar tombol "START"
    Rectangle startButton = { screenWidth / 2 - 100, 250, 200, 50 };
    DrawRectangleRec(startButton, GREEN);
    DrawText("START", screenWidth / 2 - 40, 265, 20, WHITE);

    // Gambar tombol "EXIT"
    Rectangle exitButton = { screenWidth / 2 - 100, 320, 200, 50 };
    DrawRectangleRec(exitButton, RED);
    DrawText("EXIT", screenWidth / 2 - 30, 335, 20, WHITE);
}

GameState pilihMenu(int screenWidth, int screenHeight) 
{
    GameState state = MENU;

    // Dapatkan posisi mouse
    Vector2 mousePoint = GetMousePosition();

    // Definisikan area tombol
    Rectangle startButton = { screenWidth / 2 - 100, 250, 200, 50 };
    Rectangle exitButton = { screenWidth / 2 - 100, 320, 200, 50 };

    // Periksa input mouse
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, startButton)) {
            state = GAMEPLAY;
        }
        if (CheckCollisionPointRec(mousePoint, exitButton)) {
            CloseWindow();
            exit(0);
        }
    }
    return state;
}

GameState DrawMenu(int screenWidth, int screenHeight) {
    // Gambar UI menu
    setupMenu(screenWidth, screenHeight);

    // Handle input dan kembalikan state yang sesuai
    return pilihMenu(screenWidth, screenHeight);
}

void jedapause (PauseState *tmblPause)
{
    tmblPause->isPause = false;
}

void tombolpause (PauseState *tmblPause)
{
    tmblPause->isPause = !tmblPause->isPause;
}

void DrawPauseScreen(PauseState *tmblPause)
{
    if (tmblPause->isPause)
    {
        DrawText("Game Pause\nTekan P untuk Meneruskan", 10, 50, 20, RED);
    }
}