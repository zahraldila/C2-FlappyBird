// File: main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "dava.h"
#include "zakky.h"
#include "alexandrio.h"
#include "zahra.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    // Variabel untuk posisi background
    bgX = 0;
    Texture2D cityBg = LoadTexture("city.png");

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY; // Status awal: menunggu input
    PauseState tmblpause;
    jedapause(&tmblpause);




    Bird birds[MAX_BIRDS];
    InitBirds(birds, MAX_BIRDS);
    Bird bird = CreateBird(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f);
    Buat_pipa(Pipa, TutupPipa);
    
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_P))
        {
            tombolpause(&tmblpause);
        }
        
        if (!tmblpause.isPause)
        {
            // Update background position
            bgX -= 0.5f;
            if (bgX <= -SCREEN_WIDTH) 
            {
                bgX = 0;
            }
        }
        
        BeginDrawing();
        ClearBackground(SKYBLUE);
        
        // Gambar background di belakang
        DrawBackground(cityBg, bgX);
        
        if (currentState == MENU) {
            gameOverState = GAME_READY; // Reset gameOverState ketika di menu
            currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
            
            // Reset posisi burung dan pipa ketika kembali ke menu
            if (currentState == GAMEPLAY) {
                birds[0].position.y = SCREEN_HEIGHT / 2;
                birds[0].speed = 0;
                Buat_pipa(Pipa, TutupPipa);
            }
        } 
        else if (currentState == GAMEPLAY) 
        {
            if(!tmblpause.isPause)
            {
                 // State GAME_READY - Menunggu input pemain untuk memulai
            if (gameOverState == GAME_READY) {
                // Burung hanya mengambang di tempat
                birds[0].position.y = SCREEN_HEIGHT / 2;
                birds[0].speed = 0;
                
                // Ketika SPACE ditekan, mulai game
                if (IsKeyPressed(KEY_SPACE)) {
                    gameOverState = GAME_ACTIVE;
                    birds[0].speed = FLAP_STRENGTH; // Berikan lompatan awal
                }
            }
            // State GAME_ACTIVE - Game berjalan normal
            else if (gameOverState == GAME_ACTIVE) {
                UpdateBirds(birds, MAX_BIRDS);
                Pergerakan_pipa(Pipa, TutupPipa);
                
                // Periksa collision
                gameOverState = UpdateGameCollision(birds[0], Pipa, TutupPipa);
            } 
            // State GAME_OVER - Menunggu input untuk restart
            else {
                if (IsKeyPressed(KEY_ENTER)) {
                    gameOverState = GAME_READY;
                    ResetGame(&birds[0], Pipa, TutupPipa);
                } else if (IsKeyPressed(KEY_ESCAPE)) {
                    currentState = MENU;
                    ResetGame(&birds[0], Pipa, TutupPipa);
                }
            }
        }
           
            
            // Gambar objek game
            DrawBirds(birds, MAX_BIRDS);
            Gambar_pipa(Pipa, TutupPipa);
            
            // Pesan bantuan sesuai dengan status game
            if (gameOverState == GAME_READY) {
                // Tampilkan pesan "Get Ready" di tengah layar
                DrawText("GET READY!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, 40, DARKGRAY);
                DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, DARKGRAY);
            }
            else if (gameOverState == GAME_ACTIVE) {
                DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
                DrawText("Press P to Pause", 10, 30, 20, DARKGRAY);
            }
            
            // Jika game over, tampilkan layar game over
            if (gameOverState == GAME_OVER) {
                DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            }
        }

        if (tmblpause.isPause)
        {
            DrawPauseScreen(&tmblpause);
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