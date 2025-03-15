#include "raylib.h"
#include "dava.h"
#include "bird_struct.h"
#include "alexandrio.h"
#include "zakky.h"
#include "zahra.h"
#include <stdlib.h>
#include <stdio.h>

// Status permainan global
GameOverState gameOverState = GAME_ACTIVE;

// Fungsi untuk memeriksa collision antara burung dan pipa
bool CheckBirdPipeCollision(Bird bird, int Pipa[3][3], int TutupPipa[3][3]) {
    Rectangle birdRect = {
        bird.position.x + 10,        // Koordinat X dengan offset lebih besar
        bird.position.y + 10,        // Koordinat Y dengan offset lebih besar
        bird.texture.width * 0.6f,   // Lebar lebih kecil (60% dari texture)
        bird.texture.height * 0.6f   // Tinggi lebih kecil (60% dari texture)
    };
    
    // Untuk debugging visual (opsional)
    // DrawRectangleRec(birdRect, (Color){255, 0, 0, 128}); // Tampilkan hitbox merah transparan
    
    // Periksa collision dengan setiap pipa
    for (int i = 0; i < 3; i++) {
        // Buat rectangle untuk pipa atas dengan margin 5 pixel
        Rectangle upperPipeRect = {
            (float)Pipa[i][0] + 5,            // Offset X sedikit
            0,
            LEBAR_PIPA - 10,                  // Lebar dengan margin
            (float)Pipa[i][1] - 5            // Tinggi dengan margin
        };
        
        // Buat rectangle untuk tutup pipa atas dengan margin 5 pixel
        Rectangle upperPipeCapRect = {
            (float)TutupPipa[i][0] + 5,
            (float)Pipa[i][1] - 30 + 5,
            LEBAR_PIPA + 10,                 // Kurangi margin
            20                              // Kurangi tinggi dari 30 ke 20
        };
        
        // Buat rectangle untuk pipa bawah dengan margin 5 pixel
        Rectangle lowerPipeRect = {
            (float)Pipa[i][0] + 5,
            (float)(Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH + 5),
            LEBAR_PIPA - 10,
            (float)(TINGGI_LAYAR - Pipa[i][1] - JARAK_PIPA_ATAS_BAWAH) - 5
        };
        
        // Buat rectangle untuk tutup pipa bawah dengan margin 5 pixel
        Rectangle lowerPipeCapRect = {
            (float)TutupPipa[i][0] + 5,
            (float)(Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH),
            LEBAR_PIPA + 10,
            20                              // Kurangi tinggi dari 30 ke 20
        };
        
        // Periksa apakah terjadi collision dengan pipa atau tutupnya
        if (CheckCollisionRecs(birdRect, upperPipeRect) ||
            CheckCollisionRecs(birdRect, upperPipeCapRect) ||
            CheckCollisionRecs(birdRect, lowerPipeRect) ||
            CheckCollisionRecs(birdRect, lowerPipeCapRect)) {
            return true; // Collision terdeteksi
        }
    }
    
    return false; // Tidak ada collision
}

// Fungsi untuk memeriksa collision antara burung dan tanah
bool CheckBirdGroundCollision(Bird bird) {
    // Posisi Y tanah sesuai dengan yang ada di file Pipa.c - 400px
    const int GROUND_Y_POSITION = 400;
    
    // Gunakan offset untuk hitbox burung yang lebih akurat
    float birdBottomY = bird.position.y + bird.texture.height * 0.7f;
    
    // Cek apakah bagian bawah burung menyentuh atau melewati tanah
    if (birdBottomY >= GROUND_Y_POSITION - 5) { // Berikan sedikit toleransi (5 pixel)
        return true; // Collision dengan tanah terdeteksi
    }
    
    return false; // Tidak ada collision
}

// Fungsi untuk memperbarui status permainan berdasarkan collision
GameOverState UpdateGameCollision(Bird bird, int Pipa[3][3], int TutupPipa[3][3]) {
    // Jika terjadi collision dengan pipa atau tanah, game over
    if (CheckBirdPipeCollision(bird, Pipa, TutupPipa) || CheckBirdGroundCollision(bird)) {
        return GAME_OVER;
    }
    
    return GAME_ACTIVE; // Permainan tetap aktif jika tidak ada collision
}

// Fungsi untuk menampilkan layar game over
void DrawGameOver(int screenWidth, int screenHeight, int score) {
    // Gambar kotak transparan sebagai background
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});
    
    // Tampilkan teks "GAME OVER"
    DrawText("GAME OVER", screenWidth / 2 - 120, screenHeight / 3, 40, RED);
    
    // Tampilkan petunjuk untuk restart
    DrawText("Press ENTER to Restart", screenWidth / 2 - 150, screenHeight / 2 + 20, 20, WHITE);
    DrawText("Press ESC to Menu", screenWidth / 2 - 120, screenHeight / 2 + 50, 20, WHITE);
}

// Fungsi untuk me-reset game
void ResetGame(Bird *bird, int Pipa[3][3], int TutupPipa[3][3]) {
    // Reset posisi burung
    bird->position.y = TINGGI_LAYAR / 2;
    bird->speed = 0;
    
    // Re-inisialisasi pipa
    Buat_pipa(Pipa, TutupPipa);
    
    // Reset status game (ke status menunggu)
    gameOverState = GAME_READY;
}