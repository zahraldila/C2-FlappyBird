// File: main.c
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h> // Untuk strlen, strcpy

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

// Variabel untuk input nama leaderboard
char playerNameInput[MAX_PLAYER_NAME_LENGTH + 1] = { 0 };
int letterCount = 0;
bool enteringNameMode = false;

// plist dan tplist global dari PipaLinkedList.c

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
    if (myBird == NULL) 
    {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi burung!");
        UnloadBackgroundSelector(backgroundSelector);
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
        if (IsAudioDeviceReady()) CloseAudioDevice();
        CloseWindow();
        return 1;
    }
    initPipaList();
    
    // Buat_pipa_linkedlist() akan dipanggil saat transisi ke GAMEPLAY

    Font gameFont = GetFontDefault();

    bool shouldCloseGame = false;

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
                    } else if (currentState == BACKGROUND_SELECTION) {
                        InitBackgroundSelectionScreen();
                    } else if (currentState == LEADERBOARD) {
                        InitLeaderboardScreen();
                    }
                }
            } break;
            case BACKGROUND_SELECTION: 
            {
                currentState = UpdateBackgroundSelectionScreen(backgroundSelector, currentState);
                 if (currentState == MENU && IsAudioDeviceReady() && !IsMenuMusicCurrentlyPlaying()) {
                    PlayMenuMusic();
                }
            } break;
            case LEADERBOARD: 
            {
                currentState = UpdateLeaderboardScreen(currentState);
                 if (currentState == MENU && IsAudioDeviceReady() && !IsMenuMusicCurrentlyPlaying()) {
                    PlayMenuMusic();
                }
            } break;
            case GAMEPLAY: 
            {
                if (IsKeyPressed(KEY_P) && gameOverState != GAME_OVER) { tombolpause(&tmblpause); }

                if (!tmblpause.isPause) {
                    if (!enteringNameMode) { // Hanya update game jika tidak sedang input nama
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
                                if (IsScoreHighEnough(score) && !scoreAddedToLeaderboardThisSession) {
                                    enteringNameMode = true;
                                    letterCount = 0;
                                    playerNameInput[0] = '\0';
                                } else {
                                    // SimpanHighscore(); // Opsional jika masih mau highscore tunggal
                                }
                            }
                            if (gameOverState == GAME_ACTIVE) { // Update skor jika masih aktif
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
                        } else { // GAME_OVER (dan tidak sedang enteringNameMode awalnya)
                            Pipa_berhenti(false);
                            // Logika untuk restart atau kembali ke menu setelah GAME_OVER (bukan saat input nama)
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
                            } else if (IsKeyPressed(KEY_BACKSPACE)) {
                                currentState = MENU;
                                Pipa_berhenti(true);
                                gameOverState = GAME_READY;
                            }
                        }
                    } else { // Ini adalah blok else untuk if (!enteringNameMode), jadi ini adalah saat enteringNameMode == true
                        // --- LOGIKA INPUT NAMA ---
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
                            AddScoreToLeaderboard(playerNameInput, score);
                            scoreAddedToLeaderboardThisSession = true;
                            enteringNameMode = false;
                            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                            // Setelah input nama, game tetap dalam state GAME_OVER,
                            // pemain kemudian bisa memilih restart atau kembali ke menu
                            // yang akan ditangani oleh blok 'else { // GAME_OVER ... }' di atas pada iterasi berikutnya.
                        }
                    } // Akhir dari if (enteringNameMode)
                } // Akhir dari if (!tmblpause.isPause)
            } break; // Akhir dari case GAMEPLAY
            default: break;
        } // Akhir dari switch (currentState) untuk UPDATE LOGIC

        // --- BAGIAN DRAWING ---
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Gambar background
        if (currentState == MENU || (currentState == GAMEPLAY && !tmblpause.isPause && !enteringNameMode) ) {
            LoopDrawSelectedBackground(backgroundSelector, &bgX);
        } 
        else if (currentState == LEADERBOARD) 
        {
            // Untuk leaderboard, kita gambar background yang sedang aktif (current) secara statis
            if (backgroundSelector && backgroundSelector->current && backgroundSelector->current->texture.id != 0) {
                 DrawTexture(backgroundSelector->current->texture, 0, 0, WHITE); // Menggunakan current background
            }
             else if (backgroundSelector && backgroundSelector->head && backgroundSelector->head->texture.id != 0) 
            {
                // Fallback ke background pertama jika current tidak valid (seharusnya tidak terjadi jika selector diinisialisasi dengan benar)
                 DrawTexture(backgroundSelector->head->texture, 0, 0, WHITE);
            } 
            else 
            {
                // Fallback jika tidak ada background sama sekali
                // Anda bisa membiarkan ClearBackground(SKYBLUE) saja atau gambar warna solid lain
            }
        }
        // Awan
        if (currentState == MENU || (currentState == GAMEPLAY && !tmblpause.isPause && !enteringNameMode) || currentState == BACKGROUND_SELECTION || currentState == LEADERBOARD) {
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
                if (myBird == NULL || myBird->texture.id == 0) {
                    DrawText("ERROR: BURUNG TIDAK DAPAT DIMUAT!", GetScreenWidth()/2 - MeasureText("ERROR: BURUNG TIDAK DAPAT DIMUAT!", 20)/2, GetScreenHeight()/2 -10, 20, RED);
                } else {
                    // Hanya gambar pipa dan burung jika tidak sedang input nama
                    if (!enteringNameMode) {
                        Gambar_pipa(score);
                        DrawBird(myBird);
                    }
                }
                TampilkanSkor(gameFont); // Skor selalu tampil

                if (enteringNameMode) {
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.8f));
                    const char* congratsText = "NEW HIGHSCORE!";
                    DrawText(congratsText, SCREEN_WIDTH / 2 - MeasureText(congratsText, 40) / 2, SCREEN_HEIGHT / 2 - 100, 40, YELLOW);
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
                } else if (gameOverState == GAME_READY && !tmblpause.isPause) {
                    const char* getReadyMsg = "GET READY!";
                    const char* pressSpaceMsg = "Press SPACE to Start";
                    DrawText(getReadyMsg, SCREEN_WIDTH / 2 - MeasureText(getReadyMsg, 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, LIME);
                    DrawText(pressSpaceMsg, SCREEN_WIDTH / 2 - MeasureText(pressSpaceMsg, 25) / 2, SCREEN_HEIGHT / 2 - 10, 25, LIME);
                } else if (gameOverState == GAME_OVER && !tmblpause.isPause) {
                    DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score);
                }

                if (tmblpause.isPause && !enteringNameMode) { DrawPauseScreen(&tmblpause); }
            } break;
            default: break;
        }
        EndDrawing();
    } // Akhir dari while (!WindowShouldClose())

    // --- CLEANUP ---
    SimpanHighscore();
    SaveLeaderboard();
    if (myBird) UnloadBird(myBird);
    UnloadBackgroundSelector(backgroundSelector);
    if(IsAudioDeviceReady()) {
        StopMenuMusic();
        UnloadSounds();
    }
    if (IsAudioDeviceReady()) CloseAudioDevice();
    freeAwan(&awanList);
    Hapus_semua_pipa();

    CloseWindow();
    return 0;
}