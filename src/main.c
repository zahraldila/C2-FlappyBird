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
    SetRandomSeed(time(NULL));

    // Variabel untuk posisi background
    float bgX = 0;
    Texture2D cityBg = LoadTexture("city.png");

    GameState currentState = MENU;

    Bird birds[MAX_BIRDS];
    InitBirds(birds, MAX_BIRDS);
    Bird bird = CreateBird(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f);

    int Pipa[3][3];
    int TutupPipa[3][3];
    Buat_pipa;

    while (!WindowShouldClose()) {
        // Update background position
        bgX -= 0.5f;
        if (bgX <= -SCREEN_WIDTH) {
            bgX = 0;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Gambar background di belakang
        DrawBackground(cityBg, bgX);

        if (currentState == MENU) {
            currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
        } 
        else if (currentState == GAMEPLAY) {
            // Update objek game
            UpdateBirds(birds, MAX_BIRDS);
        
            // Gambar objek game
            DrawBirds(birds, MAX_BIRDS);
            Pergerakan_pipa(Pipa, TutupPipa);
            Gambar_pipa(Pipa, TutupPipa);
            DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
        }

        EndDrawing();
    }

    // Unload assets
    UnloadBirds(birds, MAX_BIRDS);
    UnloadBird(&bird);
    UnloadTexture(cityBg);
    CloseWindow();

    return 0;
}