// File: main.c
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

// Asumsi include header ini sudah benar dan menyediakan definisi dasar
#include "raylib.h"
#include "bird_struct.h"  // Untuk Bird
#include "pipa_ll.h"      // Untuk Singlelinkedlist, initList, freeList
#include "dava.h"         // Untuk InitBird (Bird*), UpdateBird (Bird*), DrawBird (Bird*), UnloadBird (Bird*),
                          // SCREEN_WIDTH, SCREEN_HEIGHT, FLAP_STRENGTH, GRAVITY, LEBAR_LAYAR,
                          // BackgroundSelector, InitBackgroundSelector, LoopDrawSelectedBackground, UnloadBackgroundSelector, NextBackground
#include "zakky.h"        // Untuk GameState (enum), DrawMenu (GameState DrawMenu()), PauseState,
                          // AwanNode, JUMLAH_AWAN, insertAwan, updateAwan, gambarAwan, freeAwan, bgX (extern)
#include "alexandrio.h"   // Untuk plist, tplist, Pergerakan_pipa, Gambar_pipa (void Gambar_pipa()),
                          // Pipa_berhenti, LEBAR_PIPA.
#include "zahra.h"        // Untuk GameOverState (enum), UpdateGameCollision
#include "qlio.h"         // Untuk InitSkor, TambahSkor, SimpanHighscore, score, highscore, ResetSkor
#include "sound.h"        // Untuk fungsi-fungsi suara
#include "game_state.h"   // Untuk ResetGame (void ResetGame(Bird*, ...)), DrawGameOver

// Deklarasi fungsi yang hilang (implicit declaration) secara lokal di main.c
// Ini adalah solusi sementara, idealnya deklarasi ada di header yang sesuai.
void Buat_pipa_linkedlist(void); // Diasumsikan tidak ada parameter dan return void
//void ResetList(Singlelinkedlist *L_plist, Singlelinkedlist *L_tplist); // Diasumsikan ini parameternya
// void DrawBackground(Texture2D bg, float offset); // DrawBackground ada di dava.h



// Deklarasi global list pipa (sudah extern di Alexandrio.h)
Singlelinkedlist *plist;
Singlelinkedlist *tplist;

// Stub untuk DrawGameOver jika tidak didefinisikan di game_state.c
#ifndef DRAW_GAME_OVER_DEFINED_IN_GAME_STATE_C
#define DRAW_GAME_OVER_DEFINED_IN_MAIN_C
void DrawGameOver(int screenWidth, int screenHeight, int currentScore) {
    const char* gameOverText = "GAME OVER";
    const char* scoreTextMsg = TextFormat("SCORE: %d", currentScore);
    const char* restartText = "Press ENTER to Restart";
    const char* menuText = "Press BACKSPACE for Menu";

    DrawText(gameOverText, screenWidth/2 - MeasureText(gameOverText, 40)/2, screenHeight/2 - 60, 40, RED);
    DrawText(scoreTextMsg, screenWidth/2 - MeasureText(scoreTextMsg, 20)/2, screenHeight/2 - 10, 20, BLACK);
    DrawText(restartText, screenWidth/2 - MeasureText(restartText, 20)/2, screenHeight/2 + 20, 20, DARKGRAY);
    DrawText(menuText, screenWidth/2 - MeasureText(menuText, 20)/2, screenHeight/2 + 50, 20, DARKGRAY);
}
#endif


int main() {
    // Menggunakan SCREEN_WIDTH dan SCREEN_HEIGHT dari dava.h (800x600 berdasarkan log terakhir Anda)
    // Jika Alexandrio.h (450) di-include setelah dava.h, maka 450 yang akan dipakai.
    // Pastikan nilai yang diinginkan (800x450) konsisten. Untuk ini, kita anggap dava.h sudah 800x450.
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
    // bgX dideklarasikan extern di zakky.h, definisinya harus ada di satu file .c (misal background.c)
    // Jika tidak ada, maka harus didefinisikan di sini atau file .c lain yang dikompilasi.
    // float bgX = 0; // Aktifkan jika bgX tidak didefinisikan di tempat lain.

    GameState currentState = MENU;
    GameOverState gameOverState = GAME_READY;
    PauseState tmblpause;
    jedapause(&tmblpause);

    Bird *myBird = InitBird(); // Diubah: myBird sekarang Bird*, InitBird() mengembalikan Bird*
    if (myBird == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal menginisialisasi burung!");
        UnloadBackgroundSelector(backgroundSelector);
        CloseWindow();
        return 1;
    }

    bool scoreSaved = false;
    // bool passedPipe[3] = { false }; // Komentari jika tidak digunakan
    InitSkor();

    InitAudioDevice();
    InitSounds();
    bool menuMusicStarted = false;

    AwanNode *awanList = NULL;
    for (int j = 0; j < JUMLAH_AWAN; j++) { 
        float x = (float)SCREEN_WIDTH + j * 200.0f;
        float y = (float)(rand() % 150);
        insertAwan(&awanList, x, y); 
    }

    plist = (Singlelinkedlist*)malloc(sizeof(Singlelinkedlist));
    tplist = (Singlelinkedlist*)malloc(sizeof(Singlelinkedlist));
    if (plist == NULL || tplist == NULL) {
        TraceLog(LOG_FATAL, "MAIN: Gagal alokasi memori untuk list pipa!");
        if (myBird) UnloadBird(myBird);
        UnloadBackgroundSelector(backgroundSelector);
        CloseWindow();
        return 1;
    }
    initList(plist);  
    initList(tplist);
    Buat_pipa_linkedlist(); // Error: implicit declaration - deklarasi ditambahkan di atas

    bool shouldCloseGame = false; 

    while (!WindowShouldClose() && !shouldCloseGame) {
        if (currentState == GAMEPLAY && gameOverState != GAME_OVER && IsKeyPressed(KEY_P)) {
            tombolpause(&tmblpause);
        }

        UpdateMusic();

        if (currentState == MENU || (currentState == GAMEPLAY && !tmblpause.isPause)) {
            bgX -= 0.5f; 
            if (backgroundSelector->current && backgroundSelector->current->texture.id != 0) {
                 if (bgX <= -backgroundSelector->current->texture.width) bgX = 0;
            } else if (bgX <= -SCREEN_WIDTH) { 
                 bgX = 0;
            }
            updateAwan(awanList);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        LoopDrawSelectedBackground(backgroundSelector, &bgX); 
        gambarAwan(awanList);     

        if (currentState == MENU) {
            gameOverState = GAME_READY;
            if (!menuMusicStarted) {
                PlayMenuMusic();
                menuMusicStarted = true;
            }
            
            // Error: too many arguments to function 'DrawMenu'; expected 0, have 1
            // ./include/zakky.h:32:11: note: declared here GameState DrawMenu();
            // Mengubah pemanggilan agar sesuai deklarasi di zakky.h versi Anda
            currentState = DrawMenu(); 
            // Jika ingin menggunakan shouldCloseGame, deklarasi di zakky.h HARUS diubah menjadi:
            // GameState DrawMenu(bool *shouldCloseGame);
            // Dan implementasi di main_menu.c juga harus sesuai.
            // Untuk sementara, kita ikuti deklarasi yang ada di error log.
            // Maka, logika shouldCloseGame dari menu tidak bisa dipakai seperti ini.
            // Untuk keluar dari menu, pemain harus menekan ESC atau tombol close window.


            if (currentState == GAMEPLAY) {
                StopMenuMusic();
                menuMusicStarted = false;
                myBird->position.y = SCREEN_HEIGHT / 2.0f; 
                myBird->speed = 0;                        
                //ResetList(plist, tplist); // Error: implicit declaration - deklarasi ditambahkan di atas     
                Buat_pipa_linkedlist();         
                ResetSkor();                    
                scoreSaved = false;
                // for (int i = 0; i < 3; i++) passedPipe[i] = false;
                jedapause(&tmblpause); 
            } 
            else if (currentState == BACKGROUND) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
                     NextBackground(backgroundSelector); 
                }
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) { 
                    currentState = MENU; 
                }
                DrawText("Pilih Background (Klik Kiri Ganti)", 20, 20, 20, DARKGRAY);
                DrawText("ESC/Backspace ke Menu", 20, 50, 20, DARKGRAY);
            }
        }
        else if (currentState == GAMEPLAY) {
            if (!tmblpause.isPause) {
                if (gameOverState == GAME_READY) {
                    myBird->position.y = SCREEN_HEIGHT / 2.0f;
                    myBird->speed = 0;
                    if (IsKeyPressed(KEY_SPACE)) {
                        gameOverState = GAME_ACTIVE;
                        myBird->speed = FLAP_STRENGTH; // FLAP_STRENGTH dari dava.h
                        PlaySoundEffect(SOUND_FLAP);
                    }
                } else if (gameOverState == GAME_ACTIVE) {
                    UpdateBird(myBird); 
                    Pergerakan_pipa();  

                    GameOverState prevState = gameOverState;
                    // UpdateGameCollision mengharapkan Bird (by value), myBird adalah Bird*
                    gameOverState = UpdateGameCollision(*myBird, plist, tplist); 

                    if (prevState == GAME_ACTIVE && gameOverState == GAME_OVER) {
                        PlaySoundEffect(SOUND_COLLIDE);
                        PlaySoundEffect(SOUND_GAME_OVER);
                        printf(">> GAME OVER PADA SCORE: %d\n", score);
                    }

                    if (gameOverState == GAME_ACTIVE) {
                        address p_iter = plist->head; // Menggunakan nama variabel yang berbeda
                        while (p_iter != NULL) {
                            // Logika skor disesuaikan
                            float birdFrontX = myBird->position.x;
                            float birdRightX = myBird->position.x + myBird->texture.width;
                            float pipeFrontX = p_iter->korx;
                            float pipeBackX = p_iter->korx + LEBAR_PIPA;

                            // Cek jika burung baru saja melewati sisi belakang pipa
                            if (!p_iter->status && birdFrontX > pipeBackX) {
                                TambahSkor();
                                PlaySoundEffect(SOUND_SCORE);
                                p_iter->status = 1; 
                            }
                            p_iter = p_iter->next;
                        }

                        if (IsKeyPressed(KEY_SPACE)) {
                            myBird->speed = FLAP_STRENGTH;
                            PlaySoundEffect(SOUND_FLAP);
                        }
                    }
                } else { // GAME_OVER
                    if (!scoreSaved) {
                        SimpanHighscore();
                        Pipa_berhenti(false); 
                        scoreSaved = true;
                    }

                    if (IsKeyPressed(KEY_ENTER)) { 
                        gameOverState = GAME_READY;
                        Pipa_berhenti(true);
                        // ResetGame menerima Bird*
                        //ResetGame(myBird, plist, tplist); 
                        scoreSaved = false;
                    } else if (IsKeyPressed(KEY_BACKSPACE)) { 
                        currentState = MENU;
                        menuMusicStarted = false;
                        gameOverState = GAME_READY; 
                        Pipa_berhenti(true); 
                    }
                }
            }

            // Error: too many arguments to function 'Gambar_pipa'; expected 0, have 1
            // ./include/alexandrio.h:26:6: note: declared here void Gambar_pipa();
            // Mengubah pemanggilan agar sesuai deklarasi di alexandrio.h versi Anda
            Gambar_pipa(score); 
            // Jika ingin Gambar_pipa(score), deklarasi di alexandrio.h HARUS diubah menjadi:
            // void Gambar_pipa(int s);
            // Dan definisi di Pipa.c harus void Gambar_pipa(int s) { ... } (yang sudah ada di log error).
            // Ini menunjukkan ketidaksinkronan antara Alexandrio.h dan Pipa.c

            DrawBird(myBird);           
            
            char scoreTextVal[20]; 
            sprintf(scoreTextVal, "Score: %d", score);
            DrawText(scoreTextVal, SCREEN_WIDTH / 2 - MeasureText(scoreTextVal, 30) / 2, 10, 30, BLACK);
            
            char highscoreTextVal[20]; 
            sprintf(highscoreTextVal, "Best: %d", highscore);
            DrawText(highscoreTextVal, SCREEN_WIDTH / 2 - MeasureText(highscoreTextVal, 25) / 2, 45, 25, DARKGRAY);

            if (gameOverState == GAME_READY && currentState == GAMEPLAY) { 
                const char* getReadyMsg = "GET READY!";
                const char* pressSpaceMsg = "Press SPACE to Start";
                DrawText(getReadyMsg, SCREEN_WIDTH / 2 - MeasureText(getReadyMsg, 40) / 2, SCREEN_HEIGHT / 2 - 30, 40, LIME);
                DrawText(pressSpaceMsg, SCREEN_WIDTH / 2 - MeasureText(pressSpaceMsg, 25) / 2, SCREEN_HEIGHT / 2 + 20, 25, LIME);
            } 

            if (gameOverState == GAME_OVER && !tmblpause.isPause) {
                DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score); // Error: implicit declaration - deklarasi dari game_state.h
            }
        }

        if (tmblpause.isPause && currentState == GAMEPLAY) {
            DrawPauseScreen(&tmblpause); 
        }

        EndDrawing();
    }

    SimpanHighscore();
    UnloadBird(myBird); 
    UnloadBackgroundSelector(backgroundSelector); 
    UnloadSounds();
    // UnloadTexture(cityBg); // cityBg tidak lagi digunakan
    CloseAudioDevice();
    freeAwan(&awanList);
    
    // freeList dari pipa_ll.h dideklarasikan sebagai void freeList(void);
    // Pemanggilan sudah benar jika freeList bekerja pada global plist dan tplist
    freeList(); 

    if (plist) free(plist); 
    if (tplist) free(tplist);
    
    CloseWindow();

    return 0;
}