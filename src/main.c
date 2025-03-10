#include "raylib.h"

int main(void)
{
    // Inisialisasi jendela
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib Example");

    Vector2 circlePosition = { screenWidth / 2, screenHeight / 2 };
    const float circleRadius = 25.0f;
    const float speed = 4.0f;

    SetTargetFPS(60);
    
    while (!WindowShouldClose()) // Loop utama
    {
        // Update posisi lingkaran berdasarkan input
        if (IsKeyDown(KEY_RIGHT)) circlePosition.x += speed;
        if (IsKeyDown(KEY_LEFT)) circlePosition.x -= speed;
        if (IsKeyDown(KEY_UP)) circlePosition.y -= speed;
        if (IsKeyDown(KEY_DOWN)) circlePosition.y += speed;

        // Gambar objek
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(circlePosition, circleRadius, MAROON);
        DrawText("Gunakan tombol panah untuk menggerakkan lingkaran", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow(); // Tutup jendela
    
    return 0;
}
