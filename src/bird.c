#include "raylib.h"
#include "dava.h"
#include "bird_struct.h"
#include <stdlib.h>
#include <stdio.h>

// Membuat dan menginisialisasi objek burung.
Bird* InitBird() {
    Bird* newBird = (Bird*)malloc(sizeof(Bird));
    if (newBird == NULL) {
        TraceLog(LOG_FATAL, "BIRD: Gagal mengalokasikan memori untuk burung!");
        return NULL;
    }

    const char* pathToLoad = "assets/Flappy.png";

    // Muat gambar burung dari path default
    Image birdImage = LoadImage(pathToLoad);

    // Ubah ukuran gambar
    ImageResize(&birdImage, birdImage.width / 3, birdImage.height / 3);
    // Buat tekstur GPU dari gambar yang sudah dimuat dan di-resize
    newBird->texture = LoadTextureFromImage(birdImage);
    // Hapus data gambar dari memori CPU (RAM) karena sudah tidak diperlukan setelah tekstur dibuat.
    UnloadImage(birdImage);

    // Atur ukuran tabrakan (collision box) sama dengan ukuran tekstur.
    newBird->collisionWidth = (float)newBird->texture.width;
    newBird->collisionHeight = (float)newBird->texture.height;

    // Inisialisasi posisi, kecepatan, dan skala burung.
    newBird->position = (Vector2){100.0f, SCREEN_HEIGHT / 2.0f - newBird->collisionHeight / 2.0f};
    newBird->speed = 0.0f;
    newBird->scale = 1.0f;

    return newBird;
}

// Memperbarui logika burung setiap frame.
void UpdateBird(Bird* bird) {

    bird->speed += GRAVITY;
    bird->position.y += bird->speed;

    // Jika burung menyentuh batas atas layar, tahan posisi di batas atas, kecepatan menjadi 0.
    if (bird->position.y < 0) {
        bird->position.y = 0;
        bird->speed = 0;
    }
}

// Menggambar burung.
void DrawBird(Bird* bird) {
    DrawTextureEx(bird->texture, bird->position, 0.0f, bird->scale, WHITE);
}

// Membersihkan sumber daya burung.
void UnloadBird(Bird* bird) {
    UnloadTexture(bird->texture);
    free(bird);
}
