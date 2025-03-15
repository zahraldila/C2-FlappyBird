// game_init.c
#include "game_init.h"
#include "dava.h"
#include "zakky.h"
#include "alexandrio.h"
#include <time.h>

void InitGame(Bird *birds, Bird *bird, int Pipa[3][3], int TutupPipa[3][3], Texture2D *cityBg) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird - Combined Version");
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));
    
    // Inisialisasi background
    *cityBg = LoadTexture("city.png");
    
    // Inisialisasi burung
    InitBirds(birds, MAX_BIRDS);
    *bird = CreateBird(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2, "Flappy.png", 0.8f);
    
    // Inisialisasi pipa
    Buat_pipa(Pipa, TutupPipa);
}

void CleanupGame(Bird *birds, Bird *bird, Texture2D cityBg) {
    UnloadBirds(birds, MAX_BIRDS);
    UnloadBird(bird);
    UnloadTexture(cityBg);
    CloseWindow();
}