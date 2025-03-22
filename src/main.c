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
#include "sound.h"  

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

    Buat_awan(Awan);
    Buat_pipa(Pipa, TutupPipa);

    // === Sistem Skor ===
    bool scoreSaved = false;
    bool passedPipe[3] = { false };

    // Inisialisasi sistem skor
    InitSkor();
    
    // === Inisialisasi Sound ===
    InitAudioDevice();
    InitSounds();
    
    // Flag untuk mengelola menu music
    bool menuMusicStarted = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P)) {
            tombolpause(&tmblpause);
        }

        UpdateMusic();  // Update music stream

        if (!tmblpause.isPause) 
        {
            // Background bergerak
            Pergerakan_awan(Awan);
            bgX -= 0.5f;
            if (bgX <= -SCREEN_WIDTH) bgX = 0;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawBackground(cityBg, bgX);
        Gambar_awan(Awan);

        if (currentState == MENU) {
            gameOverState = GAME_READY;
            
            // Hanya memulai musik menu sekali ketika memasuki state menu
            if (!menuMusicStarted) {
                PlayMenuMusic();
                menuMusicStarted = true;
            }
            
            currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);

            if (currentState == GAMEPLAY) {
                StopMenuMusic(); // Stop musik saat masuk gameplay
                menuMusicStarted = false; // Reset flag

                birds[0].position.y = SCREEN_HEIGHT / 2;
                birds[0].speed = 0;
                Buat_pipa(Pipa, TutupPipa);
                score = 0; // Gunakan variabel global dari qlio.h
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
                        PlaySoundEffect(SOUND_FLAP);
                    }
                } else if (gameOverState == GAME_ACTIVE) {
                    // Update posisi burung
                    UpdateBirds(birds, MAX_BIRDS);
                    
                    // Update posisi pipa
                    Pergerakan_pipa(Pipa, TutupPipa);
                    
                    for (int i = 0; i < 3; i++) {
                        if (Pipa[i][0] > LEBAR_LAYAR && passedPipe[i]) {
                            passedPipe[i] = false;
                        }
                    }

                    GameOverState prevState = gameOverState;
                    gameOverState = UpdateGameCollision(birds[0], Pipa, TutupPipa);

                    // Cek collision sound
                    if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                        PlaySoundEffect(SOUND_COLLIDE);
                        PlaySoundEffect(SOUND_GAME_OVER);
                        printf(">> GAME OVER PADA SCORE: %d\n", score);
                    }

                    if (gameOverState == GAME_ACTIVE) {
                        float birdRightX = birds[0].position.x + birds[0].texture.width;

                        for (int i = 0; i < 3; i++) {
                            float pipeRightX = Pipa[i][0] + LEBAR_PIPA;

                            if (!passedPipe[i] && birdRightX > pipeRightX) {
                                // Gunakan fungsi TambahSkor untuk menambah score dan update highscore
                                TambahSkor();
                                PlaySoundEffect(SOUND_SCORE);
                                passedPipe[i] = true;

                                printf(">> BURUNG LEWAT PIPA[%d] | SCORE: %d\n", i, score);
                            }
                        }

                        if (IsKeyPressed(KEY_SPACE)) {
                            birds[0].speed = FLAP_STRENGTH;
                            PlaySoundEffect(SOUND_FLAP);
                        }
                    }
                } else { // GAME_OVER
                    if (!scoreSaved) {
                        // Simpan highscore hanya sekali saat game over
                        SimpanHighscore();
                        scoreSaved = true;
                    }

                    if (IsKeyPressed(KEY_ENTER)) {
                        gameOverState = GAME_READY;
                        ResetGame(&birds[0], Pipa, TutupPipa);
                        ResetSkor(); // Gunakan fungsi reset skor
                        scoreSaved = false;
                        for (int i = 0; i < 3; i++) passedPipe[i] = false;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        currentState = MENU;
                        ResetGame(&birds[0], Pipa, TutupPipa);
                        ResetSkor(); // Gunakan fungsi reset skor
                        scoreSaved = false;
                        for (int i = 0; i < 3; i++) passedPipe[i] = false;
                        
                        // Jangan memulai musik menu di sini, biarkan flag menanganinya
                        menuMusicStarted = false;
                    }
                }
            }

            // Gambar game
            DrawBirds(birds, MAX_BIRDS);
            Gambar_pipa(Pipa, TutupPipa, score);

            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 60, 10, 30, BLACK);
            DrawText(TextFormat("Highscore: %d", highscore), SCREEN_WIDTH / 2 - 80, 40, 25, DARKGRAY);

            if (gameOverState == GAME_READY) {
                DrawText("GET READY!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, 40, DARKGRAY);
                DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, DARKGRAY);
            } else if (gameOverState == GAME_ACTIVE) {
                DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
            }

            if (gameOverState == GAME_OVER) {
                DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score);
            }
        }

        if (tmblpause.isPause) {
            DrawPauseScreen(&tmblpause);
        }

        EndDrawing();
    }

    // Simpan highscore sebelum keluar (jika perlu)
    SimpanHighscore(); // Fungsi yang diperbarui hanya akan menyimpan jika nilai highscore berbeda

    // Unload dan cleanup
    UnloadBirds(birds, MAX_BIRDS);
    UnloadBird(&bird);
    UnloadTexture(cityBg);
    UnloadSounds();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}