#include "raylib.h"
#include "dava.h"     // Menggunakan deklarasi dan konstanta dari dava.h yang sudah diperbaiki
#include <stdlib.h>   // Untuk malloc dan free
#include <stdio.h>    // Untuk TraceLog jika diperlukan

// Variabel global ini (`bgxX`) digunakan oleh fungsi background di file ini.
// Jika `bgX` di `main.c` adalah variabel yang sama, perlu ada konsistensi
// nama dan deklarasi `extern` yang tepat.
float bgxX = 0; 

// Implementasi InitBird() diubah agar sesuai dengan deklarasi Bird* InitBird(void)
Bird* InitBird() {
    Bird* newBird = (Bird*)malloc(sizeof(Bird));
    if (newBird == NULL) {
        TraceLog(LOG_FATAL, "BIRD: Gagal mengalokasikan memori untuk burung!");
        return NULL; // Kembalikan NULL jika alokasi gagal
    }

    // Pastikan path ke "assets/Flappy.png" benar dari direktori eksekusi game Anda.
    // Jika file game.exe ada di root, dan gambar ada di assets/Flappy.png, maka path ini benar.
    Image img = LoadImage("assets/Flappy.png");
    if (img.data == NULL) {
        TraceLog(LOG_WARNING, "BIRD: Gagal memuat gambar 'assets/Flappy.png'. Pastikan file ada.");
        // Set tekstur ke keadaan tidak valid jika gambar gagal dimuat
        newBird->texture = (Texture2D){0}; 
    } else {
        ImageResize(&img, img.width / 3, img.height / 3);
        newBird->texture = LoadTextureFromImage(img);
        UnloadImage(img); // Selalu unload image setelah membuat tekstur
    }

    // Menggunakan SCREEN_HEIGHT dari dava.h (yang bernilai 600 berdasarkan file terakhir Anda)
    newBird->position = (Vector2){100, SCREEN_HEIGHT / 2.0f}; 
    newBird->speed = 0;

    return newBird; // Mengembalikan pointer ke Bird yang baru dibuat
}

// Update posisi burung
void UpdateBird(Bird* bird) { // Parameter 'bird' adalah pointer ke Bird
    if (bird == NULL) return; // Pemeriksaan keamanan

    bird->speed += GRAVITY; // Menggunakan GRAVITY dari dava.h

    if (IsKeyPressed(KEY_SPACE)) {
        bird->speed = FLAP_STRENGTH; // Menggunakan FLAP_STRENGTH dari dava.h
    }

    bird->position.y += bird->speed;

    // Batas bawah layar, menggunakan tinggi aktual tekstur jika tersedia
    float birdTextureHeight = (bird->texture.id != 0) ? bird->texture.height : 30.0f; // Fallback jika tekstur tidak ada
    if (bird->position.y > (SCREEN_HEIGHT - birdTextureHeight)) {
        bird->position.y = (SCREEN_HEIGHT - birdTextureHeight);
        bird->speed = 0;
    }

    // Batas atas layar
    if (bird->position.y < 0) { 
        bird->position.y = 0;
        bird->speed = 0;
    }
}

// Gambar burung
void DrawBird(Bird* bird) { // Parameter 'bird' adalah pointer ke Bird
    if (bird == NULL || bird->texture.id == 0) return; // Pemeriksaan keamanan dan validitas tekstur
    DrawTexture(bird->texture, (int)bird->position.x, (int)bird->position.y, WHITE);
}

// Bebaskan memori
void UnloadBird(Bird* bird) { // Parameter 'bird' adalah pointer ke Bird
    if (bird == NULL) return;

    if (bird->texture.id != 0) { // Hanya unload jika tekstur valid
        UnloadTexture(bird->texture);
    }
    free(bird); // Bebaskan memori yang dialokasikan untuk struct Bird
}