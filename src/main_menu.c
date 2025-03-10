#include "zakky.h"
#include <stdlib.h>  // Tambahkan ini untuk exit()

GameState DrawMenu(int screenWidth, int screenHeight) {
    GameState state = MENU;

    // Gambar teks judul
    DrawText("Flappy Bird", screenWidth / 2 - 100, 100, 40, DARKBLUE);

    // Tombol Start
    Rectangle startButton = { screenWidth / 2 - 100, 250, 200, 50 };
    DrawRectangleRec(startButton, GREEN);
    DrawText("START", screenWidth / 2 - 40, 265, 20, WHITE);

    // Tombol Exit
    Rectangle exitButton = { screenWidth / 2 - 100, 320, 200, 50 };
    DrawRectangleRec(exitButton, RED);
    DrawText("EXIT", screenWidth / 2 - 30, 335, 20, WHITE);

    // Cek klik tombol
    Vector2 mousePoint = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, startButton)) {
            state = GAMEPLAY;  // Masuk ke game
        }
        if (CheckCollisionPointRec(mousePoint, exitButton)) {
            CloseWindow(); // Tutup jendela game
            exit(0);       // Keluar dari program sepenuhnya
        }
    }

    return state;
}
