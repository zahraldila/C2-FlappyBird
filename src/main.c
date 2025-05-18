// File: main.c
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "raylib.h"
#include "dava.h"
#include "zakky.h"
#include "bird_struct.h"
#include "alexandrio.h"
#include "zahra.h"
#include "qlio.h"
#include "sound.h"
#include "game_state.h"
#include "pipa_ll.h"  // untuk Buat_pipa_linkedlist, ResetList, freeList


// Deklarasi global list pipa
Singlelinkedlist *plist;
Singlelinkedlist *tplist;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    bgX = 0;
    Texture2D cityBg = LoadTexture("city.png");

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY;
    PauseState tmblpause;
    jedapause(&tmblpause);

    BirdNode *myBird = InitBird();

    bool scoreSaved = false;
    bool passedPipe[3] = { false };
    InitSkor();

    InitAudioDevice();
    InitSounds();
    bool menuMusicStarted = false;

    AwanNode *awanList = NULL;
    for (int j = 0; j < JUMLAH_AWAN; j++) {
        float x = SCREEN_WIDTH + j * 200;
        float y = rand() % 150;
        insertAwan(&awanList, x, y);
    }

    plist = malloc(sizeof(Singlelinkedlist));
    tplist = malloc(sizeof(Singlelinkedlist));
    initList(plist);
    initList(tplist);
    Buat_pipa_linkedlist();

    while (!WindowShouldClose()) {
        if (gameOverState != GAME_OVER && IsKeyPressed(KEY_P)) {
            tombolpause(&tmblpause);
        }

        UpdateMusic();

        if (!tmblpause.isPause) {
            bgX -= 0.5f;
            if (bgX <= -SCREEN_WIDTH) bgX = 0;
            updateAwan(awanList);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawBackground(cityBg, bgX);
        gambarAwan(awanList);

        if (currentState == MENU) {
            gameOverState = GAME_READY;
            if (!menuMusicStarted) {
                PlayMenuMusic();
                menuMusicStarted = true;
            }

            currentState = DrawMenu();

            if (currentState == GAMEPLAY) {
                StopMenuMusic();
                menuMusicStarted = false;
                myBird->bird.position.y = SCREEN_HEIGHT / 2;
                myBird->bird.speed = 0;
                ResetList(plist, tplist);
                Buat_pipa_linkedlist();
                score = 0;
                scoreSaved = false;
                for (int i = 0; i < 3; i++) passedPipe[i] = false;
            }
        }

        else if (currentState == GAMEPLAY) {
            if (!tmblpause.isPause) {
                if (gameOverState == GAME_READY) {
                    myBird->bird.position.y = SCREEN_HEIGHT / 2;
                    myBird->bird.speed = 0;

                    if (IsKeyPressed(KEY_SPACE)) {
                        gameOverState = GAME_ACTIVE;
                        myBird->bird.speed = FLAP_STRENGTH;
                        PlaySoundEffect(SOUND_FLAP);
                    }
                } else if (gameOverState == GAME_ACTIVE) {
                    UpdateBird(myBird);
                    Pergerakan_pipa();

                    GameOverState prevState = gameOverState;
                    gameOverState = UpdateGameCollision(myBird->bird, plist, tplist);

                    if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                        PlaySoundEffect(SOUND_COLLIDE);
                        PlaySoundEffect(SOUND_GAME_OVER);
                        printf(">> GAME OVER PADA SCORE: %d\n", score);
                    }

                    if (gameOverState == GAME_ACTIVE) {
                        address p = plist->head;
                        while (p != NULL) {
                            float pipeRightX = p->korx + LEBAR_PIPA;
                            float birdRightX = myBird->bird.position.x + myBird->bird.texture.width;

                            if (!p->status && birdRightX > pipeRightX) {
                                TambahSkor();
                                PlaySoundEffect(SOUND_SCORE);
                                p->status = 1;
                                break;
                            }
                            p = p->next;
                        }

                        if (IsKeyPressed(KEY_SPACE)) {
                            myBird->bird.speed = FLAP_STRENGTH;
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
                        ResetGame(&myBird->bird, NULL, NULL);
                        ResetSkor();
                        ResetList(plist, tplist);
                        Buat_pipa_linkedlist();
                        scoreSaved = false;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) {
                        currentState = MENU;
                        ResetGame(&myBird->bird, NULL, NULL);
                        ResetSkor();
                        Pipa_berhenti(true);
                        scoreSaved = false;
                        menuMusicStarted = false;
                    }
                }
            }

            Gambar_pipa(score);
            DrawBird(myBird);
            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 60, 10, 30, BLACK);
            DrawText(TextFormat("Highscore: %d", highscore), SCREEN_WIDTH / 2 - 80, 40, 25, DARKGRAY);

            if (gameOverState == GAME_READY) {
                DrawText("GET READY!", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 30, 40, LIME);
                DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, LIME);
            } else if (gameOverState == GAME_ACTIVE) {
                DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
                DrawText("Press P to Pause", 10, 30, 20, DARKGRAY);
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

    SimpanHighscore();
    UnloadBird(myBird);
    UnloadSounds();
    CloseAudioDevice();
    freeAwan(&awanList);
    freeList(plist);
    freeList(tplist);
    free(plist);
    free(tplist);
    CloseWindow();

    return 0;
}
