// game_state.c
#include "game_state.h"
#include "dava.h"
#include "zakky.h"
#include <stdio.h>

void HandleMenuState(GameState *currentState, GameOverState *gameOverState, Bird *bird, int Pipa[3][3], int TutupPipa[3][3]) {
    *gameOverState = GAME_READY;
    *currentState = DrawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (*currentState == GAMEPLAY) {
        bird->position.y = SCREEN_HEIGHT / 2;
        bird->speed = 0;
        Buat_pipa(Pipa, TutupPipa);
    }
}

void HandleGameplayState(GameState *currentState, GameOverState *gameOverState, Bird *bird, int Pipa[3][3], int TutupPipa[3][3]) {
    // State GAME_READY - Menunggu input pemain untuk memulai
    if (*gameOverState == GAME_READY) {
        bird->position.y = SCREEN_HEIGHT / 2;
        bird->speed = 0;
        
        if (IsKeyPressed(KEY_SPACE)) {
            *gameOverState = GAME_ACTIVE;
            bird->speed = FLAP_STRENGTH;
        }
    }
    // State GAME_ACTIVE - Game berjalan normal
    else if (*gameOverState == GAME_ACTIVE) {
        UpdateBirds(bird, 1); // Update hanya satu burung
        Pergerakan_pipa(Pipa, TutupPipa);
        
        *gameOverState = UpdateGameCollision(*bird, Pipa, TutupPipa);
    } 
    // State GAME_OVER - Menunggu input untuk restart
    else {
        if (IsKeyPressed(KEY_ENTER)) {
            *gameOverState = GAME_READY;
            ResetGame(bird, Pipa, TutupPipa);
        } 
        else if (IsKeyPressed(KEY_BACKSPACE)) 
        {
            printf("BACKSPACE DITEKAN - Kembali ke Menu\n");
            *currentState = MENU;
            ResetGame(bird, Pipa, TutupPipa);
        }
    }
}

void RenderGameState(GameState currentState, GameOverState gameOverState, Bird *birds, int Pipa[3][3], int TutupPipa[3][3]) {
    // Gambar objek game
    DrawBirds(birds, MAX_BIRDS);
    Gambar_pipa(Pipa, TutupPipa);
    
    // Pesan bantuan sesuai dengan status game
    if (gameOverState == GAME_READY) {
        DrawText("GET READY!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, 40, DARKGRAY);
        DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, DARKGRAY);
    }
    else if (gameOverState == GAME_ACTIVE) {
        DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
    }
    
    // Jika game over, tampilkan layar game over
    if (gameOverState == GAME_OVER) {
        DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    }
}