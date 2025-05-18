// game_state.c
#include "game_state.h"
#include <stdio.h>

void HandleMenuState(GameState *currentState, GameOverState *gameOverState, Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist) {
    *gameOverState = GAME_READY;
    *currentState = DrawMenu();

    if (*currentState == GAMEPLAY) {
        bird->position.y = SCREEN_HEIGHT / 2;
        bird->speed = 0;
        ResetList(plist, tplist);         // Kosongkan list lama
        Buat_pipa_linkedlist();           // Isi dengan pipa baru
    }
}

void HandleGameplayState(GameState *currentState, GameOverState *gameOverState, Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist) {
    if (*gameOverState == GAME_READY) {
        bird->position.y = SCREEN_HEIGHT / 2;
        bird->speed = 0;

        if (IsKeyPressed(KEY_SPACE)) {
            *gameOverState = GAME_ACTIVE;
            bird->speed = FLAP_STRENGTH;
        }
    }
    else if (*gameOverState == GAME_ACTIVE) {
        UpdateBird(bird);
        Pergerakan_pipa();

        *gameOverState = UpdateGameCollision(*bird, plist, tplist);
    }
    else {
        if (IsKeyPressed(KEY_ENTER)) {
            *gameOverState = GAME_READY;
            ResetGame(bird, plist, tplist);
        }
        else if (IsKeyPressed(KEY_BACKSPACE)) {
            printf("BACKSPACE DITEKAN - Kembali ke Menu\n");
            *currentState = MENU;
            ResetGame(bird, plist, tplist);
        }
    }
}

void RenderGameState(GameState currentState, GameOverState gameOverState, Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist) {
    DrawBird(bird);
    Gambar_pipa(score);  // Gambar_pipa() kemungkinan tidak perlu parameter plist/tplist karena pakai global

    if (gameOverState == GAME_READY) {
        DrawText("Press SPACE to Start", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 20, 25, DARKGRAY);
    }
    else if (gameOverState == GAME_ACTIVE) {
        DrawText("Press SPACE to Flap!", 10, 10, 20, DARKGRAY);
    }

    if (gameOverState == GAME_OVER) {
        DrawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT, score);
    }
}

void ResetGame(Bird *bird, Singlelinkedlist *plist, Singlelinkedlist *tplist) {
    bird->position.y = SCREEN_HEIGHT / 2;
    bird->speed = 0;
    ResetSkor();
    ResetList(plist, tplist);
    Buat_pipa_linkedlist();
}
