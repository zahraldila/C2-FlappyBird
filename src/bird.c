#include "raylib.h"
#include "dava.h"
#include "bird_struct.h"
#include <stdlib.h>

float bgX;

BirdNode* InitBird() {
    Image img = LoadImage("Flappy.png");
    ImageResize(&img, img.width / 3, img.height / 3);

    BirdNode *node = (BirdNode *)malloc(sizeof(BirdNode));
    node->bird.texture = LoadTextureFromImage(img);
    node->bird.position = (Vector2){100, 200}; // posisi awal burung
    node->bird.speed = 0;
    node->next = NULL;
    node->prev = NULL;

    UnloadImage(img);
    return node;
}

void UpdateBird(BirdNode *node) {
    if (node == NULL) return;

    node->bird.speed += GRAVITY;

    if (IsKeyPressed(KEY_SPACE)) {
        node->bird.speed = FLAP_STRENGTH;
    }

    node->bird.position.y += node->bird.speed;

    if (node->bird.position.y > TINGGI_LAYAR - 65) {
        node->bird.position.y = TINGGI_LAYAR - 65;
        node->bird.speed = 0;
    }

    if (node->bird.position.y < 0) {
        node->bird.position.y = 0;
        node->bird.speed = 0;
    }
}

void DrawBird(BirdNode *node) {
    if (node == NULL) return;
    DrawTexture(node->bird.texture, (int)node->bird.position.x, (int)node->bird.position.y, WHITE);
}

void UnloadBird(BirdNode *node) {
    if (node != NULL) {
        UnloadTexture(node->bird.texture);
        free(node);
    }
}

// Background
void InitBackground(Texture2D *bg) {
    *bg = LoadTexture("city.png");
}

void UpdateBackground(float *bgX) {
    *bgX -= 0.5f;
    if (*bgX <= -LEBAR_LAYAR) {
        *bgX = 0;
    }
}

void DrawBackground(Texture2D bg, float bgX) {
    DrawTextureEx(bg, (Vector2){bgX, 0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(bg, (Vector2){bgX + LEBAR_LAYAR, 0}, 0.0f, 1.0f, WHITE);
}
