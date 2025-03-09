#include "bird.h"

void InitBirds(Bird birds[]) {
    for (int i = 0; i < MAX_BIRDS; i++) {
        birds[i].position = (Vector2){100, 300};  // Posisi awal burung
        birds[i].velocity = 0;
        birds[i].rotation = 0;
        birds[i].texture = LoadTexture("bird.png");  // Pastikan ada gambar burung
    }
}

void UpdateBirds(Bird birds[]) {
    for (int i = 0; i < MAX_BIRDS; i++) {
        birds[i].velocity += GRAVITY;
        birds[i].position.y += birds[i].velocity;

        if (birds[i].velocity < 0) birds[i].rotation = -20;
        else birds[i].rotation = 10;
    }
}

void DrawBirds(Bird birds[]) {
    for (int i = 0; i < MAX_BIRDS; i++) {
        DrawTexturePro(
            birds[i].texture,
            (Rectangle){0, 0, birds[i].texture.width, birds[i].texture.height},
            (Rectangle){birds[i].position.x, birds[i].position.y, birds[i].texture.width, birds[i].texture.height},
            (Vector2){birds[i].texture.width / 2, birds[i].texture.height / 2},
            birds[i].rotation,
            WHITE
        );
    }
}
