#include "raylib.h"
#include "dava.h"
#include "bird_struct.h"
#include <stdlib.h>
#include <stdio.h>

// Membuat dan menginisialisasi objek burung.
Bird* InitBird() {
    Bird* newBird = (Bird*)malloc(sizeof(Bird));                                // Alokasi memori untuk Bird.
    if (newBird == NULL) {                                                      // Cek kegagalan alokasi memori.
        TraceLog(LOG_FATAL, "BIRD: Gagal mengalokasikan memori untuk burung!"); // Log error fatal.
        return NULL;                                                            // Kembalikan NULL jika alokasi gagal.
    }

    const char* pathToLoad = "assets/Flappy.png"; // Path skin burung default, selalu digunakan.

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
    newBird->position = (Vector2){100.0f, SCREEN_HEIGHT / 2.0f - newBird->collisionHeight / 2.0f}; // Posisi awal di tengah layar vertikal.
    newBird->speed = 0.0f;                                                                         // Kecepatan awal vertikal.
    newBird->scale = 1.0f;                                                                         // Skala visual normal (tidak diperbesar/diperkecil).

    return newBird; // Kembalikan objek burung yang sudah diinisialisasi.
}

// Memperbarui logika burung setiap frame.
void UpdateBird(Bird* bird) {

    bird->speed += GRAVITY; // Terapkan gravitasi ke kecepatan.
    bird->position.y += bird->speed; // Update posisi Y berdasarkan kecepatan.

    // Jika burung menyentuh batas atas layar.
    if (bird->position.y < 0) {
        bird->position.y = 0; // Tahan posisi di batas atas.
        bird->speed = 0;      // Hentikan kecepatan naik.
    }
    // Batas bawah (tabrakan dengan tanah) ditangani di file collision.c.
}

// Menggambar burung.
void DrawBird(Bird* bird) {
    DrawTextureEx(bird->texture, bird->position, 0.0f, bird->scale, WHITE); // Gambar tekstur burung.
}

// Membersihkan sumber daya burung.
void UnloadBird(Bird* bird) {
    UnloadTexture(bird->texture); // Hapus tekstur dari VRAM.
    free(bird);                   // Bebaskan memori struct Bird.
}
