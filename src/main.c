// File: main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "dava.h"
#include "zakky.h"
#include "alexandrio.h"
#include "zahra.h"
#include "qlio.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    // Background
    bgX = 0;
    Texture2D cityBg = LoadTexture("city.png");

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY;
    PauseState tmblpause;
    jedapause(&tmblpause);

    Bird birds[MAX_BIRDS];
    InitBirds(birds, MAX_BIRDS);
    Bird bird = CreateBird(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f); // posisi burung agak kanan

    Buat_pipa(Pipa, TutupPipa);

    // === Sistem Skor ===
    int score = 0;
    int highscore = 0;
    bool scoreSaved = false;
    bool passedPipe[3] = { false };

    InitSkor();
    highscore = bacaHighScore();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P)) {
            tombolpause(&tmblpause);
        }

        if (!tmblpause.isPause) {
            // Background bergerak
            bgX -= 0.5f;
            if (bgX <= -SCREEN_WIDTH) bgX = 0;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawBackground(cityBg, bgX);

        if (currentState == MENU) {
            gameOverState = GAME_READY;
            currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);

            if (currentState == GAMEPLAY) {
                birds[0].position.y = SCREEN_HEIGHT / 2;
                birds[0].speed = 0;
                Buat_pipa(Pipa, TutupPipa);
                score = 0;
                scoreSaved = false;
                for (int i = 0; i < 3; i++) passedPipe[i] = false;
            }
        } else if (currentState == GAMEPLAY) {
            if (!tmblpause.isPause) {
                if (gameOverState == GAME_READY) {
                    birds[0].position.y = SCREEN_HEIGHT / 2;
                    birds[0].speed = 0;

                    if (IsKeyPressed(KEY_SPACE)) {
                        gameOverState = GAME_ACTIVE;
                        birds[0].speed = FLAP_STRENGTH;
                    }
                } else if (gameOverState == GAME_ACTIVE) {
                    // Update posisi burung
                    UpdateBirds(birds, MAX_BIRDS);
                    
                    // Update posisi pipa
                    Pergerakan_pipa(Pipa, TutupPipa);
                    
                    // Debug posisi pipa dan flag
                    for (int i = 0; i < 3; i++) {
                        // Reset flag passedPipe ketika pipa telah bergerak kembali ke kanan layar
                        if (Pipa[i][0] > LEBAR_LAYAR) {
                            if (passedPipe[i]) {
                                passedPipe[i] = false;
                                printf(">> PIPA[%d] RESET KE POSISI X: %d, FLAG DIRESET\n", i, Pipa[i][0]);
                            }
                        }
                    }
                    
                    // Cek collision
                    GameOverState prevState = gameOverState;
                    gameOverState = UpdateGameCollision(birds[0], Pipa, TutupPipa);
                    
                    // Jika baru saja game over, simpan debug info
                    if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                        printf(">> GAME OVER PADA SCORE: %d\n", score);
                    }
                    
                    // Jika masih aktif, baru hitung skor
                    if (gameOverState == GAME_ACTIVE) {
                        // Posisi penting burung
                        float birdRightX = birds[0].position.x + birds[0].texture.width;
                        
                        for (int i = 0; i < 3; i++) {
                            // Debug info
                            // printf("Pipa[%d] posX: %d, flag: %s\n", i, Pipa[i][0], passedPipe[i] ? "true" : "false");
                            
                            // Posisi penting pipa
                            float pipeRightX = Pipa[i][0] + LEBAR_PIPA;
                            
                            // Burung telah melewati pipa dan belum dihitung untuk pipa ini
                            if (!passedPipe[i] && birdRightX > pipeRightX) {
                                score++;
                                TambahSkor();
                                if (score > highscore) highscore = score;
                                passedPipe[i] = true;
                                
                                printf(">> BURUNG LEWAT PIPA[%d] | SCORE: %d\n", i, score);
                            }
                        }
                    }
                } else { // GAME_OVER
                    if (!scoreSaved) {
                        simpanSkorKeFile(score);
                        scoreSaved = true;
                    }

                    if (IsKeyPressed(KEY_ENTER)) {
                        gameOverState = GAME_READY;
                        ResetGame(&birds[0], Pipa, TutupPipa);
                        score = 0;
                        scoreSaved = false;
                        for (int i = 0; i < 3; i++) passedPipe[i] = false;
                    } else if (IsKeyPressed(KEY_ESCAPE)) {
                        currentState = MENU;
                        ResetGame(&birds[0], Pipa, TutupPipa);
                        score = 0;
                        scoreSaved = false;
                        for (int i = 0; i < 3; i++) passedPipe[i] = false;
                    }
                }
            }

            // Gambar game
            DrawBirds(birds, MAX_BIRDS);
            Gambar_pipa(Pipa, TutupPipa);

            // Skor tampil
            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 60, 10, 30, BLACK);
            DrawText(TextFormat("Highscore: %d", highscore), SCREEN_WIDTH / 2 - 80, 40, 25, DARKGRAY);

            // Pesan status
            if (gameOverState == GAME_READY) {
                DrawText("GET READY!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, 40, DARKGRAY);
                DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, DARKGRAY);
            } else if (gameOverState == GAME_ACTIVE) {
                DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
            }

            if (gameOverState == GAME_OVER) {
                // Tampilkan skor di layar game over
                DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score);
            }
        }

        if (tmblpause.isPause) {
            DrawPauseScreen(&tmblpause);
        }

        EndDrawing();
    }

    // Simpan highscore sebelum keluar
    SimpanHighscore();
    
    UnloadBirds(birds, MAX_BIRDS);
    UnloadBird(&bird);
    UnloadTexture(cityBg);
    CloseWindow();
    return 0;
}