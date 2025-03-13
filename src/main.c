// File: main.c
#include "raylib.h"
#include "dava.h"
#include "zakky.h"
#include "alexandrio.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MAX_BIRDS 1

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird - Combined Version");
    SetTargetFPS(60);
    
    GameState currentState = MENU;

    Bird birds[MAX_BIRDS];
    InitBirds(birds, MAX_BIRDS);
    Bird bird = CreateBird(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f);
    
    int Pipa[3][3];
    Buat_pipa(Pipa);
    
    while (!WindowShouldClose()) {
        UpdateBirds(birds, MAX_BIRDS);
        Pergerakan_pipa(Pipa);
        
        if (currentState == MENU) {
            currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
        } 
        else if (currentState == GAMEPLAY) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawBirds(birds, MAX_BIRDS);
        
        Gambar_pipa(Pipa);
        DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
        }

        EndDrawing();
    }
    
    UnloadBirds(birds, MAX_BIRDS);
    UnloadBird(&bird);
    CloseWindow();
    
    return 0;
}