#include "dava.h"
#include "zakky.h"

// Fungsi dari bird.c
void InitBirds(Bird birds[], int count) {
    Image img = LoadImage("Flappy.png"); // Load gambar burung
    ImageResize(&img, img.width / 2, img.height / 2); // Perkecil ukuran jadi setengahnya
    
    for (int i = 0; i < count; i++) {
        birds[i].texture = LoadTextureFromImage(img);
        birds[i].position = (Vector2){0, 200}; // Atur posisi awal
        birds[i].speed = 0;
    }
    
    UnloadImage(img); // Hapus image setelah dipakai
}

void UpdateBirds(Bird birds[], int count) {
    for (int i = 0; i < count; i++) {
        birds[i].speed += GRAVITY;  // Tambahkan gravitasi

        if (IsKeyPressed(KEY_SPACE)) {
            birds[i].speed = FLAP_STRENGTH;  // Beri kecepatan ke atas
        }

        birds[i].position.y += birds[i].speed;  // Perbarui posisi burung

        // Cegah burung jatuh ke tanah
        if (birds[i].position.y > GetScreenHeight() - birds[i].texture.height) {
            birds[i].position.y = GetScreenHeight() - birds[i].texture.height;
            birds[i].speed = 0;
        }

        // Cegah burung terbang di luar layar atas
        if (birds[i].position.y < -10) {
            birds[i].position.y = 0;
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

// Fungsi dari birdd.c
Bird CreateBird(float x, float y, const char *texturePath, float scale) {
    Bird bird;
    bird.position = (Vector2){x, y};
    bird.texture = LoadTexture(texturePath);
    
    // Simpan skala agar bisa digunakan saat menggambar
    bird.scale = scale;
    
    return bird;
}

void DrawBird(Bird bird) {
    // Hitung ukuran gambar setelah diskalakan
    float width = bird.texture.width * bird.scale;
    float height = bird.texture.height * bird.scale;

    // Posisi agar burung tetap berada di tengah berdasarkan ukuran yang telah diskalakan
    float x = bird.position.x - width / 2;
    float y = bird.position.y - height / 2;

    DrawTextureEx(bird.texture, (Vector2){x, y}, 0.0f, bird.scale, WHITE);
}

void UnloadBird(Bird *bird) {
    UnloadTexture(bird->texture);
}
