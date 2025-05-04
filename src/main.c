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
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    // Background
    float bgX = 0;
    Texture2D cityBg = LoadTexture("city.png");

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY;
    PauseState tmblpause;
    jedapause(&tmblpause);

    // Burung menggunakan Singly Linked List
    BirdNode *birdList = InitBirdsLinkedList(MAX_BIRDS);
    birdList->bird.position.y = SCREEN_HEIGHT / 2;  // posisi burung agak kanan
    birdList->bird.speed = 0;

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
        if (gameOverState != GAME_OVER && IsKeyPressed(KEY_P)) {
            tombolpause(&tmblpause);
        }

        UpdateMusic();  // Update music stream

        if (!tmblpause.isPause) {
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

            if (!menuMusicStarted) {
                PlayMenuMusic();
                menuMusicStarted = true;
            }

            currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);

            if (currentState == GAMEPLAY) {
                StopMenuMusic();
                menuMusicStarted = false;

                birdList->bird.position.y = SCREEN_HEIGHT / 2;
                birdList->bird.speed = 0;
                Buat_pipa(Pipa, TutupPipa);
                score = 0;
                scoreSaved = false;
                for (int i = 0; i < 3; i++) passedPipe[i] = false;
            }
        } else if (currentState == GAMEPLAY) {
            if (!tmblpause.isPause) {
                if (gameOverState == GAME_READY) {
                    birdList->bird.position.y = SCREEN_HEIGHT / 2;
                    birdList->bird.speed = 0;

                    if (IsKeyPressed(KEY_SPACE)) {
                        gameOverState = GAME_ACTIVE;
                        birdList->bird.speed = FLAP_STRENGTH;
                        PlaySoundEffect(SOUND_FLAP);
                    }
                } else if (gameOverState == GAME_ACTIVE) {
                    UpdateBirds(birdList);
                    Pergerakan_pipa(Pipa, TutupPipa);

                    for (int i = 0; i < 3; i++) {
                        if (Pipa[i][0] > LEBAR_LAYAR && passedPipe[i]) {
                            passedPipe[i] = false;
                        }
                    }

                    GameOverState prevState = gameOverState;
                    gameOverState = UpdateGameCollision(birdList->bird, Pipa, TutupPipa);

                    if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                        PlaySoundEffect(SOUND_COLLIDE);
                        PlaySoundEffect(SOUND_GAME_OVER);
                        printf(">> GAME OVER PADA SCORE: %d\n", score);
                    }

                    if (gameOverState == GAME_ACTIVE) {
                        float birdRightX = birdList->bird.position.x + birdList->bird.texture.width;

                        for (int i = 0; i < 3; i++) {
                            float pipeRightX = Pipa[i][0] + LEBAR_PIPA;

                            if (!passedPipe[i] && birdRightX > pipeRightX) {
                                TambahSkor();
                                PlaySoundEffect(SOUND_SCORE);
                                passedPipe[i] = true;

                                printf(">> BURUNG LEWAT PIPA[%d] | SCORE: %d\n", i, score);
                            }
                        }

                        if (IsKeyPressed(KEY_SPACE)) {
                            birdList->bird.speed = FLAP_STRENGTH;
                            PlaySoundEffect(SOUND_FLAP);
                        }
                    }
                } else {
                    if (!scoreSaved) {
                        SimpanHighscore();
                        Pipa_berhenti(false);
                        scoreSaved = true;
                    }

                    if (IsKeyPressed(KEY_ENTER)) {
                        gameOverState = GAME_READY;
                        Pipa_berhenti(true);
                        ResetGame(&birdList->bird, Pipa, TutupPipa);
                        ResetSkor();
                        scoreSaved = false;
                        for (int i = 0; i < 3; i++) passedPipe[i] = false;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        currentState = MENU;
                        ResetGame(&birdList->bird, Pipa, TutupPipa);
                        ResetSkor();
                        Pipa_berhenti(true);
                        scoreSaved = false;
                        for (int i = 0; i < 3; i++) passedPipe[i] = false;
                        menuMusicStarted = false;
                    }
                }
            }

            DrawBirds(birdList);
            Gambar_pipa(Pipa, TutupPipa, score);

            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 60, 10, 30, BLACK);
            DrawText(TextFormat("Highscore: %d", highscore), SCREEN_WIDTH / 2 - 80, 40, 25, DARKGRAY);

            if (gameOverState == GAME_READY) {
                DrawText("GET READY!", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 30, 40, LIME);
                DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, LIME);
            } else if (gameOverState == GAME_ACTIVE) {
                DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
                DrawText("Press P to Pause", 10, 30, 20, DARKGRAY);
            } else if (gameOverState == GAME_OVER) {
                DrawText("GAME OVER", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 30, 40, RED);
                DrawText("Press ENTER to Restart", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, DARKGRAY);
            }
        }

        EndDrawing();
    }

    // Unload resources
    UnloadBirds(birdList);
    UnloadTexture(cityBg);
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
