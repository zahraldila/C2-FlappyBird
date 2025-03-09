#include "raylib.h"
#include "bird.h"

Bird birds[MAX_BIRDS];

int main() {
    InitWindow(800, 600, "Flappy Bird");
    InitBirds(birds);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        UpdateBirds(birds);
        DrawBirds(birds);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
