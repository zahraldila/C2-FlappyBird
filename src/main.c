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
#include "sound.h"       // Untuk InitSounds, PlayMenuMusic, StopMenuMusic, UpdateMusic, IsMenuMusicCurrentlyPlaying
#include "game_state.h"

// plist dan tplist global dari PipaLinkedList.c

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird C2");
    Image icon = LoadImage("assets/icon.png");
    if (icon.data != NULL) {
        SetWindowIcon(icon);
        UnloadImage(icon);
    } else {
        TraceLog(LOG_WARNING, "MAIN: Gagal memuat ikon 'assets/icon.png'");
    }
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    BackgroundSelector *backgroundSelector = InitBackgroundSelector();
    if (backgroundSelector == NULL || backgroundSelector->current == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi BackgroundSelector.");
        CloseWindow();
        return 1;
    }

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY;
    PauseState tmblpause;
    jedapause(&tmblpause);

    Bird *myBird = InitBird();
    if (myBird == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi burung!");
        UnloadBackgroundSelector(backgroundSelector);
        CloseWindow();
        return 1;
     }
    if (myBird->texture.id == 0) {
        TraceLog(LOG_WARNING, "MAIN: Tekstur burung tidak valid setelah InitBird(). Burung mungkin tidak terlihat.");
    }

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "MAIN: Gagal menginisialisasi audio device. Tidak akan ada suara.");
    } else {
        InitSounds();
    }

    InitSkor();
    bool scoreSavedThisSession = false;

    AwanNode *awanList = NULL;
    for (int j = 0; j < JUMLAH_AWAN; j++) {
        float x_awan = (float)SCREEN_WIDTH + j * 200.0f;
        float y_awan = (float)(rand() % 150);
        insertAwan(&awanList, x_awan, y_awan);
    }

    plist = (Singlelinkedlist*)malloc(sizeof(Singlelinkedlist));
    tplist = (Singlelinkedlist*)malloc(sizeof(Singlelinkedlist));
    if (plist == NULL || tplist == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal alokasi memori untuk list pipa!");
        if (myBird) UnloadBird(myBird);
        UnloadBackgroundSelector(backgroundSelector);
        if (IsAudioDeviceReady()) CloseAudioDevice();
        CloseWindow();
        return 1;
    }
    initList(plist);
    initList(tplist);
    Buat_pipa_linkedlist();

    Font gameFont = GetFontDefault();

    bool shouldCloseGame = false;

    while (!WindowShouldClose() && !shouldCloseGame) {
        if (IsAudioDeviceReady()) UpdateMusic();

        // --- UPDATE LOGIC PER STATE ---
        switch (currentState) {
            case MENU: {
                gameOverState = GAME_READY;
                scoreSavedThisSession = false;
                if (IsAudioDeviceReady() && !IsMenuMusicCurrentlyPlaying()) {
                    PlayMenuMusic();
                }
                bgX -= KECEPATAN_BACKGROUND_SCROLL;
                if (backgroundSelector->current && backgroundSelector->current->texture.id != 0) {
                     if (bgX <= -backgroundSelector->current->texture.width) bgX = 0;
                } else if (bgX <= -SCREEN_WIDTH) {
                     bgX = 0;
                }
                updateAwan(awanList);

                GameState nextStateFromMenu = pilihMenu();
                if (currentState != nextStateFromMenu) {
                    currentState = nextStateFromMenu;
                    if (currentState == GAMEPLAY) {
                        if (IsAudioDeviceReady()) StopMenuMusic();
                        myBird->position.y = SCREEN_HEIGHT / 2.0f - myBird->collisionHeight / 2.0f;
                        myBird->speed = 0;
                        freeListPasangan();
                        Buat_pipa_linkedlist();
                        ResetSkor();
                        Pipa_berhenti(true);
                        jedapause(&tmblpause);
                    } else if (currentState == BACKGROUND_SELECTION) {
                        InitBackgroundSelectionScreen();
                    } else if (currentState == LEADERBOARD) {
                        InitLeaderboardScreen();
                    }
                }
            } break;
            case BACKGROUND_SELECTION: {
                currentState = UpdateBackgroundSelectionScreen(backgroundSelector, currentState);
                 if (currentState == MENU && IsAudioDeviceReady() && !IsMenuMusicCurrentlyPlaying()) {
                    PlayMenuMusic();
                }
            } break;
            case LEADERBOARD: {
                currentState = UpdateLeaderboardScreen(currentState);
                 if (currentState == MENU && IsAudioDeviceReady() && !IsMenuMusicCurrentlyPlaying()) {
                    PlayMenuMusic();
                }
            } break;
            case GAMEPLAY: {
                if (IsKeyPressed(KEY_P) && gameOverState != GAME_OVER) { tombolpause(&tmblpause); }

                if (!tmblpause.isPause) {
                    bgX -= KECEPATAN_BACKGROUND_SCROLL;
                    if (backgroundSelector->current && backgroundSelector->current->texture.id != 0) {
                         if (bgX <= -backgroundSelector->current->texture.width) bgX = 0;
                    } else if (bgX <= -SCREEN_WIDTH) {
                         bgX = 0;
                    }
                    updateAwan(awanList);

                    if (gameOverState == GAME_READY) {
                        myBird->position.y = SCREEN_HEIGHT / 2.0f - myBird->collisionHeight / 2.0f;
                        myBird->speed = 0;
                        if (IsKeyPressed(KEY_SPACE)) {
                            gameOverState = GAME_ACTIVE;
                            myBird->speed = FLAP_STRENGTH;
                            if(IsAudioDeviceReady()) PlaySoundEffect(SOUND_FLAP);
                        }
                    } else if (gameOverState == GAME_ACTIVE) {
                        UpdateBird(myBird);
                         if (IsKeyPressed(KEY_SPACE)) {
                            myBird->speed = FLAP_STRENGTH;
                            if(IsAudioDeviceReady()) PlaySoundEffect(SOUND_FLAP);
                        }
                        Pergerakan_pipa();

                        GameOverState prevState = gameOverState;
                        gameOverState = UpdateGameCollision(*myBird, plist, tplist);

                        if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                            if(IsAudioDeviceReady()) PlaySoundEffect(SOUND_GAME_OVER);
                            printf(">> GAME OVER PADA SCORE: %d\n", score);
                            AddScoreToLeaderboard(score);
                            scoreSavedThisSession = true;
                        }
                        if (gameOverState == GAME_ACTIVE) {
                            address p_iter = plist->head;
                            while (p_iter != NULL) {
                                float birdRightX = myBird->position.x + myBird->collisionWidth;
                                float pipeBackX = p_iter->korx + LEBAR_PIPA;

                                if (!p_iter->status && myBird->position.x > pipeBackX) {
                                    TambahSkor();
                                    if(IsAudioDeviceReady()) PlaySoundEffect(SOUND_SCORE);
                                    p_iter->status = 1;
                                }
                                p_iter = p_iter->next;
                            }
                        }
                    } else { // GAME_OVER
                        if (!scoreSavedThisSession) {
                            SimpanHighscore(); // Fungsi lama untuk highscore tunggal
                        }
                        Pipa_berhenti(false);

                        if (IsKeyPressed(KEY_ENTER)) {
                            myBird->position.y = SCREEN_HEIGHT / 2.0f - myBird->collisionHeight / 2.0f;
                            myBird->speed = 0;
                            freeListPasangan();
                            Buat_pipa_linkedlist();
                            ResetSkor();
                            Pipa_berhenti(true);
                            scoreSavedThisSession = false; // Reset untuk sesi game baru
                            gameOverState = GAME_READY;
                            jedapause(&tmblpause);
                        } else if (IsKeyPressed(KEY_BACKSPACE)) {
                            currentState = MENU;
                            // menuMusicStarted akan dihandle oleh logika di case MENU
                            Pipa_berhenti(true);
                            gameOverState = GAME_READY;
                        }
                    }
                }
            } break;
            default: break;
        }

        // --- DRAWING ---
        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (currentState != BACKGROUND_SELECTION && currentState != LEADERBOARD) {
            LoopDrawSelectedBackground(backgroundSelector, &bgX);
        } else if (currentState == LEADERBOARD) {
            // Untuk leaderboard, kita gambar background yang sedang aktif (current) secara statis
            if (backgroundSelector && backgroundSelector->current && backgroundSelector->current->texture.id != 0) {
                 DrawTexture(backgroundSelector->current->texture, 0, 0, WHITE); // Menggunakan current background
            } else if (backgroundSelector && backgroundSelector->head && backgroundSelector->head->texture.id != 0) {
                // Fallback ke background pertama jika current tidak valid (seharusnya tidak terjadi jika selector diinisialisasi dengan benar)
                 DrawTexture(backgroundSelector->head->texture, 0, 0, WHITE);
            } else {
                // Fallback jika tidak ada background sama sekali
                // Anda bisa membiarkan ClearBackground(SKYBLUE) saja atau gambar warna solid lain
            }
        }
        gambarAwan(awanList);

        switch (currentState) {
            case MENU: {
                setupMenu();
            } break;
            case BACKGROUND_SELECTION: {
                DrawBackgroundSelectionScreen(backgroundSelector);
            } break;
            case LEADERBOARD: {
                DrawLeaderboardScreen(gameFont);
            } break;
            case GAMEPLAY: {
                Gambar_pipa(score);
                DrawBird(myBird);
                TampilkanSkor(gameFont);

                if (gameOverState == GAME_READY && !tmblpause.isPause) {
                    const char* getReadyMsg = "GET READY!";
                    const char* pressSpaceMsg = "Press SPACE to Start";
                    DrawText(getReadyMsg, SCREEN_WIDTH / 2 - MeasureText(getReadyMsg, 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, LIME);
                    DrawText(pressSpaceMsg, SCREEN_WIDTH / 2 - MeasureText(pressSpaceMsg, 25) / 2, SCREEN_HEIGHT / 2 - 10, 25, LIME);
                } else if (gameOverState == GAME_OVER && !tmblpause.isPause) {
                    DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score);
                }
                if (tmblpause.isPause) { DrawPauseScreen(&tmblpause); }
            } break;
            default: break;
        }
        EndDrawing();
    }

    SimpanHighscore();
    SaveLeaderboard();

    UnloadBird(myBird);
    UnloadBackgroundSelector(backgroundSelector);
    if(IsAudioDeviceReady()) {
        StopMenuMusic(); // <--- GANTI DARI StopMusicStream(menuMusic)
        UnloadSounds();
    }
    if (IsAudioDeviceReady()) CloseAudioDevice();
    freeAwan(&awanList);
    freeListPasangan();
    if (plist) free(plist);
    if (tplist) free(tplist);
    plist = NULL;
    tplist = NULL;

    CloseWindow();
    return 0;
}