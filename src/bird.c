#include "raylib.h"
#include "dava.h"
#include <stdlib.h>

float bgX = 0;

// Inisialisasi burung dan linked list yTrack
BirdGame* InitBird() {
    BirdGame* bg = (BirdGame*)malloc(sizeof(BirdGame));

    Image img = LoadImage("Flappy.png");
    ImageResize(&img, img.width / 3, img.height / 3);
    bg->bird.texture = LoadTextureFromImage(img);
    UnloadImage(img);

    bg->bird.position = (Vector2){100, SCREEN_HEIGHT / 2};
    bg->bird.speed = 0;

    BirdNode* first = (BirdNode*)malloc(sizeof(BirdNode));
    first->y = bg->bird.position.y;
    first->speed = bg->bird.speed;
    first->prev = NULL;
    first->next = NULL;

    bg->yTrackHead = first;
    bg->yTrackNow = first;

    return bg;
}

// Tambah posisi baru ke DLL
void AddBirdPosition(BirdGame* bg) {
    BirdNode* newNode = (BirdNode*)malloc(sizeof(BirdNode));
    newNode->y = bg->bird.position.y;
    newNode->speed = bg->bird.speed;
    newNode->prev = bg->yTrackNow;
    newNode->next = NULL;

    bg->yTrackNow->next = newNode;
    bg->yTrackNow = newNode;
}

// Update posisi burung dan simpan ke DLL
void UpdateBird(BirdGame* bg) {
    bg->bird.speed += GRAVITY;

    if (IsKeyPressed(KEY_SPACE)) {
        bg->bird.speed = FLAP_STRENGTH;
    }

    bg->bird.position.y += bg->bird.speed;

    if (bg->bird.position.y > SCREEN_HEIGHT - 65) {
        bg->bird.position.y = SCREEN_HEIGHT - 65;
        bg->bird.speed = 0;
    }

    if (bg->bird.position.y < -15) {
        bg->bird.position.y = -15;
        bg->bird.speed = 0;
    }

    AddBirdPosition(bg);
}

// Gambar burung
void DrawBird(BirdGame* bg) {
    DrawTexture(bg->bird.texture, (int)bg->bird.position.x, (int)bg->bird.position.y, WHITE);
}

// Bebaskan memori
void UnloadBird(BirdGame* bg) {
    UnloadTexture(bg->bird.texture);

    // Hapus DLL posisi
    BirdNode* current = bg->yTrackNow;
    while (current && current->prev) {
        current = current->prev;
    }

    while (current) {
        BirdNode* temp = current;
        current = current->next;
        free(temp);
    }

    free(bg);
}

// Background tetap pakai float
void InitBackground(Texture2D* bg) {
    *bg = LoadTexture("city.png");
}

void UpdateBackground(float* bgX) {
    *bgX -= 0.5f;
    if (*bgX <= -LEBAR_LAYAR) {
        *bgX = 0;
    }
}

void DrawBackground(Texture2D bg, float bgX) {
    DrawTextureEx(bg, (Vector2){bgX, 0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(bg, (Vector2){bgX + LEBAR_LAYAR, 0}, 0.0f, 1.0f, WHITE);
}
