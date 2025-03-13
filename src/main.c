#include "raylib.h"
#include "dava.h"
#include "zakky.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MAX_BIRDS 1

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird - Combined Version");
    SetTargetFPS(60);

    // Inisialisasi burung menggunakan metode dari bird.c
    Bird birds[MAX_BIRDS];
    InitBirds(birds, MAX_BIRDS);
    
    // Inisialisasi burung menggunakan metode dari birdd.c
    Bird bird = CreateBird(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f);
    
    while (!WindowShouldClose()) {
        UpdateBirds(birds, MAX_BIRDS); // Update gerakan burung dari bird.c
        
        BeginDrawing();
        ClearBackground(SKYBLUE);
        
        // Gambar burung dari bird.c
        DrawBirds(birds, MAX_BIRDS);
        
        // Gambar burung dari birdd.
        
        DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    UnloadBirds(birds, MAX_BIRDS); // Bersihkan memori burung dari bird.c
    UnloadBird(&bird); // Bersihkan memori burung dari birdd.c
    CloseWindow();
    
    return 0;
}