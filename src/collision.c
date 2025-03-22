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
    // Koordinat penting burung - bagian tengah burung
    float birdCenterX = bird.position.x + bird.texture.width/2;
    float birdCenterY = bird.position.y + bird.texture.height/2;
    float birdRadius = 5;
    
    for (int i = 0; i < 3; i++) {
        // Koordinat pipa
        float pipeLeft = Pipa[i][0];
        float pipeRight = Pipa[i][0] + LEBAR_PIPA;
        
        // Jika burung berada di area pipa secara horizontal
        if (birdCenterX + birdRadius > pipeLeft && birdCenterX - birdRadius < pipeRight) {
            // Longgarkan margin untuk pipa bawah
            float topPipeBottom = Pipa[i][1] + 30; // Batas bawah pipa atas
            float bottomPipeTop = Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH; // Tambah offset 10px
            
            // Cek collision dengan pipa atas
            if (birdCenterY - birdRadius < topPipeBottom) {
                return true;
            }
            
            // Cek collision dengan pipa bawah (dengan offset)
            if (birdCenterY + birdRadius > bottomPipeTop) {
                return true;
            }
        }
    }
    
    return false;
}

// Fungsi untuk memeriksa collision antara burung dan tanah
bool CheckBirdGroundCollision(Bird bird) {
    // Koordinat penting burung - bagian tengah burung dengan area kecil
    float birdCenterX = bird.position.x + bird.texture.width/2;
    float birdCenterY = bird.position.y + bird.texture.height/2;
    float birdRadius = 10; // Radius sangat kecil, hanya 10 pixel
    
    // Posisi Y tanah
    const int GROUND_Y_POSITION = 420;
    
    // Cek apakah bagian bawah area collision burung menyentuh tanah
    if (birdCenterY + birdRadius >= GROUND_Y_POSITION) {
        return true;
    }
    
    return false;
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
    
    // Tampilkan skor akhir
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    DrawText(scoreText, screenWidth / 2 - 80, screenHeight / 2 - 20, 30, WHITE);
    
    // Tampilkan petunjuk untuk restart
    DrawText("Press ENTER to Restart", screenWidth / 2 - 150, screenHeight / 2 + 30, 20, WHITE);
    DrawText("Press Backspace to Menu", screenWidth / 2 - 120, screenHeight / 2 + 60, 20, WHITE);
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

// Fungsi untuk menampilkan hitbox (untuk debugging)
void DrawHitboxes(Bird bird, int Pipa[3][3], int TutupPipa[3][3]) {
    // Gambar area collision burung
    float birdCenterX = bird.position.x + bird.texture.width/2;
    float birdCenterY = bird.position.y + bird.texture.height/2;
    DrawCircleLines(birdCenterX, birdCenterY, 10, RED);
    
    // Gambar area collision pipa
    for (int i = 0; i < 3; i++) {
        // Pipa atas
        DrawRectangleLines(Pipa[i][0], 0, LEBAR_PIPA, Pipa[i][1], GREEN);
        
        // Pipa bawah
        DrawRectangleLines(Pipa[i][0], 
                         Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, 
                         LEBAR_PIPA, 
                         TINGGI_LAYAR - (Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH), 
                         BLUE);
    }
}