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
    Bird bird = CreateBird(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f);

    Buat_pipa(Pipa, TutupPipa);

    // Sistem Skor
    int score = 0;
    int highscore = 0;
    bool scoreSaved = false;
    bool passedPipe[3] = { false };
    InitSkor();
    highscore = bacaHighScore();

    // Sound
    InitAudioDevice();
    InitSounds();
    bool menuMusicStarted = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P)) {
            tombolpause(&tmblpause);
        }

        UpdateMusic();

        if (!tmblpause.isPause) {
            bgX -= 0.5f;
            if (bgX <= -SCREEN_WIDTH) bgX = 0;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawBackground(cityBg, bgX);

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
                        PlaySoundEffect(SOUND_FLAP);
                    }

                } else if (gameOverState == GAME_ACTIVE) {
                    UpdateBirds(birds, MAX_BIRDS);
                    Pergerakan_pipa(Pipa, TutupPipa);

                    for (int i = 0; i < 3; i++) {
                        if (Pipa[i][0] > LEBAR_LAYAR && passedPipe[i]) {
                            passedPipe[i] = false;
                        }
                    }

                    GameOverState prevState = gameOverState;
                    gameOverState = UpdateGameCollision(birds[0], Pipa, TutupPipa);

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
                                score++;
                                PlaySoundEffect(SOUND_SCORE);
                                TambahSkor();
                                if (score > highscore) highscore = score;
                                passedPipe[i] = true;

                                printf(">> BURUNG LEWAT PIPA[%d] | SCORE: %d\n", i, score);
                            }
                        }

                        if (IsKeyPressed(KEY_SPACE)) {
                            birds[0].speed = FLAP_STRENGTH;
                            PlaySoundEffect(SOUND_FLAP);
                        }
                    }

                } else if (gameOverState == GAME_OVER) {
                    if (!scoreSaved) {
                        if (score > highscore) {
                            highscore = score;
                            SimpanHighscore();
                        }
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
                        menuMusicStarted = false;
                    }
                }
            }

            // Draw objek gameplay
            DrawBirds(birds, MAX_BIRDS);
            Gambar_pipa(Pipa, TutupPipa);

            // Draw skor
            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 60, 10, 30, BLACK);
            DrawText(TextFormat("Highscore: %d", highscore), SCREEN_WIDTH / 2 - 80, 50, 25, DARKGRAY);
        }

        EndDrawing();
    }

    UnloadTexture(cityBg);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
