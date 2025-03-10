#include "zakky.h"
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
