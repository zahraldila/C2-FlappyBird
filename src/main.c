// File: main.c
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "raylib.h"
#include "bird_struct.h"
#include "pipa_ll.h"
#include "dava.h"
#include "zakky.h"
#include "alexandrio.h"
#include "zahra.h"
#include "qlio.h"
#include "sound.h"
#include "game_state.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird C2"); //
    Image icon = LoadImage("assets/icon.png"); //
    if (icon.data != NULL) {
        SetWindowIcon(icon); //
        UnloadImage(icon); //
    } else {
        TraceLog(LOG_WARNING, "MAIN: Gagal memuat ikon 'assets/icon.png'"); //
    }
    SetTargetFPS(60); //
    SetRandomSeed((unsigned int)time(NULL)); //

    BackgroundSelector *backgroundSelector = InitBackgroundSelector(); //
    if (backgroundSelector == NULL || backgroundSelector->current == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi BackgroundSelector."); //
        CloseWindow();
        return 1;
    }

    GameState currentState = MENU; // State awal
    GameOverState gameOverState = GAME_READY; //
    PauseState tmblpause;
    jedapause(&tmblpause); //

    Bird *myBird = InitBird(); //
    if (myBird == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi burung!"); //
        UnloadBackgroundSelector(backgroundSelector); //
        CloseWindow();
        return 1;
    }
    if (myBird->texture.id == 0) {
        TraceLog(LOG_WARNING, "MAIN: Tekstur burung tidak valid setelah InitBird(). Burung mungkin tidak terlihat.");
    }

    bool scoreSaved = false; //
    InitSkor(); //

    InitAudioDevice(); //
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "MAIN: Gagal menginisialisasi audio device.");
    }
    InitSounds(); //
    bool menuMusicStarted = false; //

    AwanNode *awanList = NULL; //
    for (int j = 0; j < JUMLAH_AWAN; j++) { //
        float x_awan = (float)SCREEN_WIDTH + j * 200.0f;
        float y_awan = (float)(rand() % 150);
        insertAwan(&awanList, x_awan, y_awan); //
    }

    plist = (Singlelinkedlist*)malloc(sizeof(Singlelinkedlist)); //
    tplist = (Singlelinkedlist*)malloc(sizeof(Singlelinkedlist)); //
    if (plist == NULL || tplist == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal alokasi memori untuk list pipa!"); //
        if (myBird) UnloadBird(myBird); //
        UnloadBackgroundSelector(backgroundSelector); //
        if (IsAudioDeviceReady()) CloseAudioDevice(); //
        CloseWindow();
        return 1;
    }
    initList(plist); //
    initList(tplist); //
    Buat_pipa_linkedlist(); //

    bool shouldCloseGame = false; //

    while (!WindowShouldClose() && !shouldCloseGame) { //
        UpdateMusic(); // Untuk musik menu

        // --- UPDATE LOGIC PER STATE ---
        switch (currentState) {
            case MENU: {
                gameOverState = GAME_READY; //
                if (!menuMusicStarted) { //
                    PlayMenuMusic(); //
                    menuMusicStarted = true; //
                }
                // Pembaruan background dan awan untuk MENU
                bgX -= 0.5f; //
                if (backgroundSelector->current && backgroundSelector->current->texture.id != 0) { //
                     if (bgX <= -backgroundSelector->current->texture.width) bgX = 0; //
                } else if (bgX <= -SCREEN_WIDTH) { //
                     bgX = 0; //
                }
                updateAwan(awanList); //

                GameState nextStateFromMenu = pilihMenu(); // pilihMenu() dari zakky.h menghandle input & logika
                if (currentState != nextStateFromMenu) { // Jika ada perubahan state
                    currentState = nextStateFromMenu;
                    if (currentState == GAMEPLAY) { //
                        StopMenuMusic(); //
                        menuMusicStarted = false; //
                        myBird->position.y = SCREEN_HEIGHT / 2.0f; //
                        myBird->speed = 0; //
                        freeList();
                        Buat_pipa_linkedlist(); //
                        ResetSkor(); //
                        scoreSaved = false; //
                        Pipa_berhenti(true);
                        jedapause(&tmblpause); //
                    } else if (currentState == BACKGROUND_SELECTION) {
                        InitBackgroundSelectionScreen(); // Panggil sekali saat masuk state
                    }
                    // Tambahkan else if untuk SKIN, LEADERBOARD jika ada
                }
            } break;
            case BACKGROUND_SELECTION: {
                // Di layar ini, background utama game tidak perlu scroll
                currentState = UpdateBackgroundSelectionScreen(backgroundSelector, currentState);
            } break;
            case GAMEPLAY: {
                if (IsKeyPressed(KEY_P) && gameOverState != GAME_OVER) { //
                    tombolpause(&tmblpause); //
                }

                if (!tmblpause.isPause) { //
                    bgX -= 0.5f; //
                    if (backgroundSelector->current && backgroundSelector->current->texture.id != 0) { //
                         if (bgX <= -backgroundSelector->current->texture.width) bgX = 0; //
                    } else if (bgX <= -SCREEN_WIDTH) { //
                         bgX = 0; //
                    }
                    updateAwan(awanList); //

                    if (gameOverState == GAME_READY) { //
                        myBird->position.y = SCREEN_HEIGHT / 2.0f; //
                        myBird->speed = 0; //
                        if (IsKeyPressed(KEY_SPACE)) { //
                            gameOverState = GAME_ACTIVE; //
                            myBird->speed = FLAP_STRENGTH; //
                            PlaySoundEffect(SOUND_FLAP); //
                        }
                    } else if (gameOverState == GAME_ACTIVE) { //
                        UpdateBird(myBird); //
                         if (IsKeyPressed(KEY_SPACE)) { //
                            myBird->speed = FLAP_STRENGTH; //
                            PlaySoundEffect(SOUND_FLAP); //
                        }
                        Pergerakan_pipa();  //

                        GameOverState prevState = gameOverState; //
                        gameOverState = UpdateGameCollision(*myBird, plist, tplist); //

                        if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                        // Karena suara tabrakan spesifik sudah dimainkan di collision.c,
                        // di sini kita hanya perlu memainkan suara umum GAME_OVER jika ada.
                            PlaySoundEffect(SOUND_GAME_OVER); // Mainkan suara umum "Game Over"
                            printf(">> GAME OVER PADA SCORE: %d\n", score);
                        }

                        if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) { //
                            // Suara sudah dihandle di collision.c atau bisa di sini
                            printf(">> GAME OVER PADA SCORE: %d\n", score); //
                        }

                        if (gameOverState == GAME_ACTIVE) { //
                            address p_iter = plist->head; //
                            while (p_iter != NULL) { //
                                float birdRightX = myBird->position.x + myBird->collisionWidth;
                                float pipeBackX = p_iter->korx + LEBAR_PIPA; //

                                if (!p_iter->status && myBird->position.x > pipeBackX) { //
                                    TambahSkor(); //
                                    PlaySoundEffect(SOUND_SCORE); //
                                    p_iter->status = 1; //
                                }
                                p_iter = p_iter->next; //
                            }
                        }
                    } else { // GAME_OVER //
                        if (!scoreSaved) { //
                            SimpanHighscore(); //
                            Pipa_berhenti(false); //
                            scoreSaved = true; //
                        }

                        if (IsKeyPressed(KEY_ENTER)) { //
                            myBird->position.y = SCREEN_HEIGHT / 2.0f; //
                            myBird->speed = 0; //
                            freeList();
                            Buat_pipa_linkedlist(); //
                            ResetSkor(); //
                            Pipa_berhenti(true); //
                            scoreSaved = false; //
                            gameOverState = GAME_READY; //
                            jedapause(&tmblpause); //
                        } else if (IsKeyPressed(KEY_BACKSPACE)) { //
                            currentState = MENU; //
                            menuMusicStarted = false; //
                            Pipa_berhenti(true); //
                            gameOverState = GAME_READY; //
                        }
                    }
                }
            } break;
            default: break;
        }

        // --- DRAWING ---
        BeginDrawing(); //
        ClearBackground(SKYBLUE); //

        if (currentState != BACKGROUND_SELECTION) {
            LoopDrawSelectedBackground(backgroundSelector, &bgX); //
        }
        gambarAwan(awanList); //

        switch (currentState) {
            case MENU: {
                setupMenu(); // Dari zakky.h, hanya menggambar UI menu
            } break;
            case BACKGROUND_SELECTION: {
                DrawBackgroundSelectionScreen(backgroundSelector); // Dari zakky.h
            } break;
            case GAMEPLAY: {
                Gambar_pipa(score); //
                DrawBird(myBird); //

                char scoreTextVal[20]; //
                sprintf(scoreTextVal, "Score: %d", score); //
                DrawText(scoreTextVal, 10, 10, 30, BLACK); //

                char highscoreTextVal[20]; //
                sprintf(highscoreTextVal, "Best: %d", highscore); //
                DrawText(highscoreTextVal, 10, 45, 25, DARKGRAY); //

                if (gameOverState == GAME_READY && !tmblpause.isPause) { //
                    const char* getReadyMsg = "GET READY!"; //
                    const char* pressSpaceMsg = "Press SPACE to Start"; //
                    DrawText(getReadyMsg, SCREEN_WIDTH / 2 - MeasureText(getReadyMsg, 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, LIME); //
                    DrawText(pressSpaceMsg, SCREEN_WIDTH / 2 - MeasureText(pressSpaceMsg, 25) / 2, SCREEN_HEIGHT / 2 - 10, 25, LIME); //
                } else if (gameOverState == GAME_OVER && !tmblpause.isPause) { //
                    DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score); //
                }

                if (tmblpause.isPause) { //
                    DrawPauseScreen(&tmblpause); //
                }
            } break;
            default: break;
        }
        EndDrawing(); //
    }

    SimpanHighscore(); //
    UnloadBird(myBird); //
    UnloadBackgroundSelector(backgroundSelector); //
    UnloadSounds(); //
    if (IsAudioDeviceReady()) CloseAudioDevice(); //
    freeAwan(&awanList); //

    freeList(); //
    if (plist) free(plist); //
    if (tplist) free(tplist); //
    plist = NULL; //
    tplist = NULL; //

    CloseWindow(); //
    return 0;
}