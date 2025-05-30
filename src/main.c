#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h> // Untuk strlen, strcpy

#include "raylib.h"
#include "bird_struct.h"
#include "pipa_ll.h"
#include "dava.h"
#include "zakky.h" // Untuk GameState, MENU, GAMEPLAY, PauseState, jedapause, tombolpause, setupMenu, pilihMenu, UpdateBackgroundSelectionScreen, DrawBackgroundSelectionScreen, InitLeaderboardScreen, UpdateLeaderboardScreen, DrawLeaderboardScreen, awanList, JUMLAH_AWAN, SCREEN_WIDTH, KECEPATAN_BACKGROUND_SCROLL, updateAwan, gambarAwan, freeAwan, bgX
#include "alexandrio.h" // Untuk Hapus_semua_pipa, Buat_pipa, Pipa_berhenti, Pergerakan_pipa, Gambar_pipa, LEBAR_PIPA
#include "zahra.h" // Untuk GameOverState, UpdateGameCollision, DrawGameOver, HandleHelpState, DrawHelpScreen
#include "qlio.h" // Untuk InitSkor, TambahSkor, ResetSkor, TampilkanSkor, SimpanHighscore, IsMenuMusicCurrentlyPlaying, PlayMenuMusic, StopMenuMusic, InitSounds, PlaySoundEffect, UnloadSounds, UpdateMusic, score
#include "sound.h" // Sudah ter-include dari qlio.h
#include "leaderboard.h" // Untuk fungsi leaderboard

// Variabel untuk input nama leaderboard
char playerNameInput[MAX_PLAYER_NAME_LENGTH + 1] = { 0 };
int letterCount = 0;
bool enteringNameMode = false;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird C2");
    Image icon = LoadImage("assets/icon.png");
    if (icon.data != NULL)
    {
        SetWindowIcon(icon);
        UnloadImage(icon);
    }
    else
    {
        TraceLog(LOG_WARNING, "MAIN: Gagal memuat ikon 'assets/icon.png'");
    }
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    // Inisialisasi leaderboard
    InitLeaderboard();
    TraceLog(LOG_INFO, "MAIN: Leaderboard diinisialisasi.");

    BackgroundSelector *backgroundSelector = InitBackgroundSelector();
    if (backgroundSelector == NULL || backgroundSelector->current == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi BackgroundSelector.");
        FreeLeaderboard();
        CloseWindow();
        return 1;
    }

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY;
    PauseState tmblpause;
    jedapause(&tmblpause);

    Bird *myBird = InitBird();
    if (myBird == NULL)
    {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi burung!");
        UnloadBackgroundSelector(backgroundSelector);
        FreeLeaderboard();
        CloseWindow();
        return 1;
    }
    if (myBird->texture.id == 0)
    {
        TraceLog(LOG_WARNING, "MAIN: Tekstur burung tidak valid setelah InitBird(). Burung mungkin tidak terlihat.");
    }

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "MAIN: Gagal menginisialisasi audio device. Tidak akan ada suara.");
    } else {
        InitSounds();
    }

    InitSkor();
    bool scoreAddedToLeaderboardThisSession = false;

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
        FreeLeaderboard();
        if (IsAudioDeviceReady()) CloseAudioDevice();
        CloseWindow();
        return 1;
    }
    initPipaList();

    Font gameFont = GetFontDefault();

    bool shouldCloseGame = false;
    bool needNameInput = false; // Variabel baru untuk menandakan perlu input nama sebelum GAMEPLAY

    while (!WindowShouldClose() && !shouldCloseGame) {
        if (IsAudioDeviceReady()) UpdateMusic();

        // --- UPDATE LOGIC PER STATE ---
        switch (currentState) {
            case MENU: {
                gameOverState = GAME_READY;
                scoreAddedToLeaderboardThisSession = false;
                enteringNameMode = false;
                letterCount = 0;
                playerNameInput[0] = '\0';
                needNameInput = false;

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
                        // Jangan langsung ke GAMEPLAY, minta input nama dulu
                        needNameInput = true;
                        enteringNameMode = true;
                        letterCount = 0;
                        playerNameInput[0] = '\0';
                        if (IsAudioDeviceReady()) StopMenuMusic();
                    } else if (currentState == BACKGROUND_SELECTION) {
                        InitBackgroundSelectionScreen();
                    } else if (currentState == LEADERBOARD) {
                        InitLeaderboardScreen();
                    } else if (currentState == STATE_HELP) { // <-- BARIS INI DITAMBAHKAN
                        // Tidak ada inisialisasi khusus untuk layar bantuan
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
                if (needNameInput && enteringNameMode) {
                    // --- LOGIKA INPUT NAMA SEBELUM GAMEPLAY ---
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);

                    int key = GetCharPressed();
                    while (key > 0) {
                        if (((key >= 32) && (key <= 126)) && (letterCount < MAX_PLAYER_NAME_LENGTH)) {
                            char charToAdd = (char)key;
                            if (charToAdd >= 'a' && charToAdd <= 'z') {
                                charToAdd = (char)(charToAdd - 'a' + 'A');
                            }
                            if ((charToAdd >= 'A' && charToAdd <= 'Z') || (charToAdd >= '0' && charToAdd <= '9')) {
                                playerNameInput[letterCount] = charToAdd;
                                playerNameInput[letterCount + 1] = '\0';
                                letterCount++;
                            }
                        }
                        key = GetCharPressed();
                    }

                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (letterCount > 0) {
                            letterCount--;
                            playerNameInput[letterCount] = '\0';
                        }
                    }

                    if (IsKeyPressed(KEY_ENTER)) {
                        if (letterCount == 0) {
                            strcpy(playerNameInput, "AAA");
                        }
                        TraceLog(LOG_INFO, "MAIN: Nama pemain disimpan: %s", playerNameInput);
                        enteringNameMode = false;
                        needNameInput = false;
                        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                        // Inisialisasi game setelah input nama
                        if (myBird) UnloadBird(myBird);
                        myBird = InitBird();
                        if (myBird == NULL || myBird->texture.id == 0) {
                            TraceLog(LOG_ERROR, "MAIN: Gagal InitBird saat GAMEPLAY! Kembali ke MENU.");
                            currentState = MENU;
                            break;
                        }
                        myBird->position.y = SCREEN_HEIGHT / 2.0f - myBird->collisionHeight / 2.0f;
                        myBird->speed = 0;
                        Hapus_semua_pipa();
                        Buat_pipa();
                        ResetSkor();
                        Pipa_berhenti(true);
                        jedapause(&tmblpause);
                    }
                } else {
                    // --- LOGIKA GAMEPLAY NORMAL ---
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
                                TraceLog(LOG_INFO, "MAIN: Game over pada skor: %d", score);
                                if (IsScoreHighEnough(score) && !scoreAddedToLeaderboardThisSession) {
                                    TraceLog(LOG_INFO, "MAIN: Menambahkan skor ke leaderboard: %s %d", playerNameInput, score);
                                    AddScoreToLeaderboard(playerNameInput, score);
                                    SaveLeaderboard();
                                    TraceLog(LOG_INFO, "MAIN: Leaderboard disimpan setelah menambahkan skor.");
                                    scoreAddedToLeaderboardThisSession = true;
                                }
                            }
                            if (gameOverState == GAME_ACTIVE) {
                                address p_iter = plist->head;
                                while (p_iter != NULL) {
                                    if (!p_iter->status && (myBird->position.x > p_iter->korx + LEBAR_PIPA)) {
                                        TambahSkor();
                                        if(IsAudioDeviceReady()) PlaySoundEffect(SOUND_SCORE);
                                        p_iter->status = 1;
                                    }
                                    p_iter = p_iter->next;
                                }
                            }
                        } else { // GAME_OVER
                            Pipa_berhenti(false);
                            if (IsKeyPressed(KEY_ENTER)) {
                                if (myBird) UnloadBird(myBird);
                                myBird = InitBird();
                                if (myBird == NULL || myBird->texture.id == 0) { currentState = MENU; break; }
                                myBird->position.y = SCREEN_HEIGHT / 2.0f - myBird->collisionHeight / 2.0f;
                                myBird->speed = 0;
                                Hapus_semua_pipa();
                                Buat_pipa();
                                ResetSkor();
                                Pipa_berhenti(true);
                                scoreAddedToLeaderboardThisSession = false;
                                gameOverState = GAME_READY;
                                jedapause(&tmblpause);
                                needNameInput = true;
                                enteringNameMode = true;
                                letterCount = 0;
                                playerNameInput[0] = '\0';
                            } else if (IsKeyPressed(KEY_BACKSPACE)) {
                                currentState = MENU;
                                Pipa_berhenti(true);
                                gameOverState = GAME_READY;
                            }
                        }
                    }
                }
            } break;
            case STATE_HELP: { // <-- KASUS BARU INI (UPDATE LOGIC)
                HandleHelpState(&currentState); // Panggil fungsi update untuk state bantuan
            } break;
            default: break;
        }

        // --- BAGIAN DRAWING ---
        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (currentState == MENU || (currentState == GAMEPLAY && !tmblpause.isPause && !enteringNameMode)) {
            LoopDrawSelectedBackground(backgroundSelector, &bgX);
        } else if (currentState == LEADERBOARD || (currentState == GAMEPLAY && enteringNameMode) ||
                   currentState == STATE_HELP) {
            if (backgroundSelector && backgroundSelector->current && backgroundSelector->current->texture.id != 0) {
                DrawTexture(backgroundSelector->current->texture, 0, 0, WHITE);
            } else if (backgroundSelector && backgroundSelector->head && backgroundSelector->head->texture.id != 0) {
                DrawTexture(backgroundSelector->head->texture, 0, 0, WHITE);
            }
        }

        // Awan
        if (currentState == MENU || (currentState == GAMEPLAY && !tmblpause.isPause && !enteringNameMode) ||
            currentState == BACKGROUND_SELECTION || currentState == LEADERBOARD ||
            currentState == STATE_HELP) { 
            gambarAwan(awanList);
        }

        // Gambar elemen spesifik per state
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
                if (enteringNameMode) {
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.8f));
                    const char* enterNameText = TextFormat("ENTER NAME (%d Chars Max):", MAX_PLAYER_NAME_LENGTH);
                    DrawText(enterNameText, SCREEN_WIDTH / 2 - MeasureText(enterNameText, 20) / 2, SCREEN_HEIGHT / 2 - 40, 20, RAYWHITE);
                    Rectangle textBox = { SCREEN_WIDTH / 2.0f - 75, SCREEN_HEIGHT / 2.0f - 10, 150, 50 };
                    DrawRectangleRec(textBox, LIGHTGRAY);
                    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
                    DrawText(playerNameInput, (int)textBox.x + 10, (int)textBox.y + 15, 30, MAROON);
                    if (letterCount < MAX_PLAYER_NAME_LENGTH) {
                        if (((int)(GetTime()*2.5f))%2 == 0) {
                            DrawText("_", (int)textBox.x + 10 + MeasureText(playerNameInput, 30), (int)textBox.y + 12, 30, MAROON);
                        }
                    }
                    DrawText("Press ENTER to Confirm", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to Confirm", 20) / 2, SCREEN_HEIGHT / 2 + 60, 20, LIGHTGRAY);
                } else {
                    if (myBird == NULL || myBird->texture.id == 0) {
                        DrawText("ERROR: BURUNG TIDAK DAPAT DIMUAT!", GetScreenWidth()/2 - MeasureText("ERROR: BURUNG TIDAK DAPAT DIMUAT!", 20)/2, GetScreenHeight()/2 -10, 20, RED);
                    } else {
                        Gambar_pipa(score);
                        DrawBird(myBird);
                    }
                    TampilkanSkor(gameFont);

                    if (gameOverState == GAME_READY && !tmblpause.isPause) {
                        const char* getReadyMsg = "GET READY!";
                        const char* pressSpaceMsg = "Press SPACE to Start";
                        DrawText(getReadyMsg, SCREEN_WIDTH / 2 - MeasureText(getReadyMsg, 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, LIME);
                        DrawText(pressSpaceMsg, SCREEN_WIDTH / 2 - MeasureText(pressSpaceMsg, 25) / 2, SCREEN_HEIGHT / 2 - 10, 25, LIME);
                    } else if (gameOverState == GAME_OVER && !tmblpause.isPause) {
                        DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score);
                    }

                    if (tmblpause.isPause && !enteringNameMode) { DrawPauseScreen(&tmblpause); }
                }
            } break;
            case STATE_HELP: { 
                DrawHelpScreen();
            } break;
            case CREDIT:
            {
                DrawCreditScreen(gameFont);
            }
            break;
            default: break;
        }
        EndDrawing();
    }

    // --- CLEANUP ---
    SimpanHighscore();
    SaveLeaderboard();
    FreeLeaderboard();
    if (myBird) UnloadBird(myBird);
    UnloadBackgroundSelector(backgroundSelector);
    if(IsAudioDeviceReady()) {
        StopMenuMusic();
        UnloadSounds();
        CloseAudioDevice();
    }
    freeAwan(&awanList);
    Hapus_semua_pipa();
    CloseWindow();
    return 0;
}