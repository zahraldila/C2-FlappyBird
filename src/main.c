#include "raylib.h"
#include "zakky.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Flappy Bird with Image");

    // Buat burung di tengah layar dengan skala 0.4 (40% ukuran aslinya)
    Bird bird = CreateBird(screenWidth / 6, screenHeight / 2, "flappy.png", 0.4f);

    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawBird(bird);
        EndDrawing();
    }

    UnloadBird(&bird);
    CloseWindow();

    return 0;
}
