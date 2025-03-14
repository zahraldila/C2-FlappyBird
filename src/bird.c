// File: bird.c
#include "raylib.h"
#include "dava.h"
#include "zakky.h"
#include "bird_struct.h"
#include <stdlib.h>
#include <time.h>


float bgX;

//  Implementasi dari bird.c
void InitBirds(Bird bird[], int count) {
    Image img = LoadImage("Flappy.png");
    ImageResize(&img, img.width / 2, img.height / 2);
    
    for (int i = 0; i < count; i++) {
        bird[i].texture = LoadTextureFromImage(img);
        bird[i].position = (Vector2){0, 200};
        bird[i].speed = 0;
    }
    
    UnloadImage(img);
}

void UpdateBirds(Bird bird[], int count) {
    for (int i = 0; i < count; i++) {
        bird[i].speed += GRAVITY;  // Tambahkan gravitasi

        if (IsKeyPressed(KEY_SPACE)) {
            bird[i].speed = FLAP_STRENGTH;
        }        

        bird[i].position.y += bird[i].speed;  // Perbarui posisi burung

        // Cegah burung jatuh ke tanah
        if (bird[i].position.y > 385) {
            bird[i].position.y = 385;
            bird[i].speed = 0;
        }

        // Cegah burung terbang di luar layar atas
        if (bird[i].position.y < -15) {
            bird[i].position.y = -15;
            bird[i].speed = 0;
        }
    }
}

void DrawBirds(Bird bird[], int count) {
    for (int i = 0; i < count; i++) {
        DrawTexture(bird[i].texture, (int)bird[i].position.x, (int)bird[i].position.y, WHITE);
    }
}

void UnloadBirds(Bird bird[], int count) {
    for (int i = 0; i < count; i++) {
        UnloadTexture(bird[i].texture);
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


void InitBackground(Texture2D *bg) {
    *bg = LoadTexture("city.png"); // Load gambar kota
}

void UpdateBackground(float *bgX) {
    *bgX -= 0.5f; // Background bergerak lambat
    if (*bgX <= -LEBAR_LAYAR) {
        *bgX = 0; // Reset posisi jika keluar layar
    }
}

void DrawBackground(Texture2D bg, float bgX) {
    DrawTextureEx(bg, (Vector2){bgX, 0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(bg, (Vector2){bgX + LEBAR_LAYAR, 0}, 0.0f, 1.0f, WHITE);
}