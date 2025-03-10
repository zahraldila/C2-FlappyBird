#include "raylib.h"
#include "zakky.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Flappy Bird with Menu");

    GameState currentState = MENU;

    // Buat burung di tengah dan sedikit ke kiri
    float birdX = screenWidth * 0.3f;
    float birdY = screenHeight / 2;
    Bird bird = CreateBird(birdX, birdY, "flappy.png", 0.3f);

    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (currentState == MENU) {
            currentState = DrawMenu(screenWidth, screenHeight);
        } 
        else if (currentState == GAMEPLAY) {
            DrawBird(bird);
        }

        EndDrawing();
    }

    UnloadBird(&bird);
    CloseWindow();

    return 0;
}
