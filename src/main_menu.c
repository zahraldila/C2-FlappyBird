#include "zakky.h"
#include <stdlib.h>  // Tambahkan ini untuk exit()

void setupMenu() 
{
    GameState state = MENU;

    DrawText("Flappy Bird", LEBAR_LAYAR / 2 - 170, 100, 60, YELLOW);

    // Gambar tombol "START"
    Rectangle startButton = { LEBAR_LAYAR / 3 - 100, 250, 200, 50 };
    DrawRectangleRec(startButton, BLUE);
    DrawText("START", LEBAR_LAYAR / 3 - 40, 265, 20, WHITE);

    Rectangle scoreButton = { LEBAR_LAYAR / 2, 250, 200, 50 };
    DrawRectangleRec(scoreButton, BLUE);
    DrawText("Leaderboard", LEBAR_LAYAR / 2 + 40, 265, 20, WHITE);
 
    // Gambar tombol "EXIT"
    Rectangle bckButton = { LEBAR_LAYAR / 3 - 100, 320, 200, 50 };
    DrawRectangleRec(bckButton, BLUE);
    DrawText("choose background", LEBAR_LAYAR / 5 + 5 , 335, 20, WHITE);

    Rectangle skinButton = { LEBAR_LAYAR / 2, 320, 200, 50 };
    DrawRectangleRec(skinButton, BLUE);
    DrawText("choose skin", LEBAR_LAYAR / 2 + 35, 335, 20, WHITE);   

    Rectangle exitButton = { LEBAR_LAYAR / 2 - 100, 380, 190, 50 };
    DrawRectangleRec(exitButton, RED);
    DrawText("EXIT", LEBAR_LAYAR / 2 - 50, 390, 20, WHITE);   
}
 
GameState pilihMenu() 
{
    GameState state = MENU;

    // Dapatkan posisi mouse
    Vector2 mousePoint = GetMousePosition();

    // Definisikan area tombol
    Rectangle startButton = { LEBAR_LAYAR / 3 - 100, 250, 200, 50 };
    Rectangle exitButton = { LEBAR_LAYAR / 2 - 100, 380, 200, 50 };

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

GameState DrawMenu() 
{
    // Gambar UI menu
    setupMenu();

    // Handle input dan kembalikan state yang sesuai
    return pilihMenu();
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