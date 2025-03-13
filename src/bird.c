// File: bird.c
#include "raylib.h"
#include "dava.h"
#include "zakky.h"
#include "alexandrio.h"
#include "bird_struct.h"
#include <stdlib.h>
#include <time.h>

// Implementasi dari bird.c
void InitBirds(Bird birds[], int count) {
    Image img = LoadImage("Flappy.png");
    ImageResize(&img, img.width / 2, img.height / 2);
    
    for (int i = 0; i < count; i++) {
        birds[i].texture = LoadTextureFromImage(img);
        birds[i].position = (Vector2){0, 200};
        birds[i].speed = 0;
    }
    
    UnloadImage(img);
}

void UpdateBirds(Bird birds[], int count) {
    for (int i = 0; i < count; i++) {
        birds[i].speed += GRAVITY;  // Tambahkan gravitasi

        if (IsKeyPressed(KEY_SPACE)) {
            birds[i].speed = FLAP_STRENGTH;
        }        

        birds[i].position.y += birds[i].speed;  // Perbarui posisi burung

        // Cegah burung jatuh ke tanah
        if (birds[i].position.y > 385) {
            birds[i].position.y = 385;
            birds[i].speed = 0;
        }

        // Cegah burung terbang di luar layar atas
        if (birds[i].position.y < -15) {
            birds[i].position.y = -15;
            birds[i].speed = 0;
        }
    }
}

void DrawBirds(Bird birds[], int count) {
    for (int i = 0; i < count; i++) {
        DrawTexture(birds[i].texture, (int)birds[i].position.x, (int)birds[i].position.y, WHITE);
    }
}

void UnloadBirds(Bird birds[], int count) {
    for (int i = 0; i < count; i++) {
        UnloadTexture(birds[i].texture);
    }
}

// Implementasi dari zakky.h
Bird CreateBird(float x, float y, const char *texturePath, float scale) {
    Bird bird;
    bird.position = (Vector2){x, y};
    bird.texture = LoadTexture(texturePath);
    bird.scale = scale;
    return bird;
}



void UnloadBird(Bird *bird) {
    UnloadTexture(bird->texture);
}

// Implementasi dari alexandrio.h (pipa)
void Buat_pipa(int Pipa[3][3]) {
    for (int i = 0; i < 3; i++) {
        Pipa[i][0] = (SCREEN_WIDTH + i * 300) + 10;
        Pipa[i][1] = rand() % (SCREEN_HEIGHT - JARAK_PIPA_ATAS_BAWAH - 150) + 50;
        Pipa[i][2] = 0;
    }
}

void Pergerakan_pipa(int Pipa[3][3]) {
    for (int i = 0; i < 3; i++) {
        Pipa[i][0] -= KECEPATAN_PIPA;
        if (Pipa[i][0] + LEBAR_PIPA < 0) {
            Pipa[i][0] = SCREEN_WIDTH + 20;
            Pipa[i][1] = rand() % (SCREEN_HEIGHT - JARAK_PIPA_ATAS_BAWAH - 100) + 50;
            Pipa[i][2] = 0;
        }
    }
}

void Gambar_pipa(int Pipa[3][3]) {
    for (int i = 0; i < 3; i++) {
        DrawRectangle(Pipa[i][0], 0, LEBAR_PIPA, Pipa[i][1], GREEN);
        DrawRectangle(Pipa[i][0], Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA, SCREEN_HEIGHT - Pipa[i][1] - JARAK_PIPA_ATAS_BAWAH, GREEN);
    }
}

GameState DrawMenu(int screenWidth, int screenHeight) {
    GameState state = MENU;

    // Gambar teks judul
    DrawText("Flappy Bird", screenWidth / 2 - 100, 100, 40, DARKBLUE);

    // Tombol Start
    Rectangle startButton = { screenWidth / 2 - 100, 250, 200, 50 };
    DrawRectangleRec(startButton, GREEN);
    DrawText("START", screenWidth / 2 - 40, 265, 20, WHITE);

    // Tombol Exit
    Rectangle exitButton = { screenWidth / 2 - 100, 320, 200, 50 };
    DrawRectangleRec(exitButton, RED);
    DrawText("EXIT", screenWidth / 2 - 30, 335, 20, WHITE);

    // Cek klik tombol
    Vector2 mousePoint = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, startButton)) {
            state = GAMEPLAY;  // Masuk ke game
        }
        if (CheckCollisionPointRec(mousePoint, exitButton)) {
            CloseWindow(); // Tutup jendela game
            exit(0);       // Keluar dari program sepenuhnya
        }
    }

    return state;
}

void InitBackground(Texture2D *bg) {
    *bg = LoadTexture("city.png"); // Load gambar kota
}

void UpdateBackground(float *bgX) {
    *bgX -= 0.5f; // Background bergerak lambat
    if (*bgX <= -SCREEN_WIDTH) {
        *bgX = 0; // Reset posisi jika keluar layar
    }
}

void DrawBackground(Texture2D bg, float bgX) {
    DrawTextureEx(bg, (Vector2){bgX, 0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(bg, (Vector2){bgX + SCREEN_WIDTH, 0}, 0.0f, 1.0f, WHITE);
}