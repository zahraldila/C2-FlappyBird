#include "raylib.h"
#include "dava.h"
#include <stdlib.h>
#include <stdio.h>

Bird* InitBird() {
    Bird* newBird = (Bird*)malloc(sizeof(Bird));
    if (newBird == NULL) {
        TraceLog(LOG_FATAL, "BIRD: Gagal mengalokasikan memori untuk burung!");
        return NULL;
    }

    Image img = LoadImage("assets/Flappy.png");
    if (img.data == NULL) {
        TraceLog(LOG_WARNING, "BIRD: Gagal memuat gambar 'assets/Flappy.png'. Pastikan file ada.");
        newBird->texture = (Texture2D){0};
        // Set default collision size jika gambar gagal dimuat agar tidak crash
        newBird->collisionWidth = 10; // Contoh
        newBird->collisionHeight = 10; // Contoh
    } else {
        // Ukuran asli sebelum resize, bisa digunakan untuk referensi collision box
        // float originalWidth = img.width;
        // float originalHeight = img.height;

        ImageResize(&img, img.width / 3, img.height / 3); // Gambar dikecilkan menjadi 1/3
        newBird->texture = LoadTextureFromImage(img);
        UnloadImage(img);

        // PENYESUAIAN PENTING DI SINI:
        // Tentukan collisionWidth dan collisionHeight berdasarkan visual burung
        // setelah di-resize menjadi 1/3.
        // Ini adalah nilai perkiraan, Anda HARUS menyesuaikannya agar pas.
        // Anda bisa mulai dengan persentase dari newBird->texture.width/height
        // atau nilai piksel absolut jika Anda tahu ukuran visual burungnya.

        // Contoh: jika visual burung sekitar 80% dari lebar dan 70% dari tinggi tekstur yang sudah di-resize:
        newBird->collisionWidth = newBird->texture.width * 1.0f;
        newBird->collisionHeight = newBird->texture.height * 1.0f;
        // Atau jika Anda tahu ukuran piksel spesifik yang diinginkan untuk bounding box:
        // newBird->collisionWidth = 30; // Misalnya
        // newBird->collisionHeight = 20; // Misalnya
    }

    newBird->position = (Vector2){100, SCREEN_HEIGHT / 2.0f};
    newBird->speed = 0;
    newBird->scale = 1.0f; // Scale visual tambahan, jika 1.0 maka ukuran tekstur yang dipakai

    if (newBird->texture.id == 0) { // Jika tekstur gagal dimuat, pastikan collision size tidak nol
        if (newBird->collisionWidth == 0) newBird->collisionWidth = 10;
        if (newBird->collisionHeight == 0) newBird->collisionHeight = 10;
    }


    return newBird;
}

void UpdateBird(Bird* bird) {
    if (bird == NULL) return;

    // Terapkan gravitasi
    bird->speed += GRAVITY;
    bird->position.y += bird->speed;

    // Batas atas layar: jika burung menyentuh bagian atas layar (y=0)
    if (bird->position.y < 0) {
        bird->position.y = 0; // Tahan burung di batas atas layar
        bird->speed = 0;      // Hentikan kecepatan vertikalnya (tidak memantul, tapi berhenti naik)
    }

    // Batas bawah layar akan ditangani oleh fungsi UpdateGameCollision di collision.c
    // Tidak perlu penanganan batas bawah eksplisit di sini jika collision.c sudah benar.
}

void DrawBird(Bird* bird) {
    if (bird == NULL || bird->texture.id == 0) return;
    // Menggambar tekstur utama
    DrawTextureEx(bird->texture, bird->position, 0.0f, bird->scale, WHITE);

    // Jika Anda ingin visual bounding box collision yang lebih kecil, gambar juga di sini untuk debug
    // Rectangle collisionRectDebug = {
    //     bird->position.x, // Jika collision box dimulai dari posisi x,y burung
    //     bird->position.y,
    //     bird->collisionWidth,
    //     bird->collisionHeight
    // };
    // DrawRectangleLinesEx(collisionRectDebug, 1, BLUE); // Warna lain untuk membedakan
}

void UnloadBird(Bird* bird) {
    if (bird == NULL) return;
    if (bird->texture.id != 0) {
        UnloadTexture(bird->texture);
    }
    free(bird);
}