#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LEBAR_LAYAR 800
#define TINGGI_LAYAR 450
#define LEBAR_PIPA 80
#define JARAK_PIPA_ATAS_BAWAH 150
#define KECEPATAN_PIPA 2



void Buat_pipa(int Pipa[3][3]){
    for(int i = 0; i < 3; i++){
        Pipa[i][0] = (LEBAR_LAYAR + i * 300) + 10;
        Pipa[i][1] = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50;
        Pipa[i][2] = 0;
    }   
}


void Pergerakan_pipa(int Pipa[3][3]){
    for(int i = 0; i < 3; i++){
        Pipa[i][0] -= KECEPATAN_PIPA;
        if(Pipa[i][0] + LEBAR_PIPA < 0){
            Pipa[i][0] = LEBAR_LAYAR + 20;
            Pipa[i][1] = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 100) + 50;
            Pipa[i][2] = 0;   
        }
    }
}


void Gambar_pipa(int Pipa[3][3]){
    for(int i = 0; i < 3; i++){
        DrawRectangle(Pipa[i][0], 0, LEBAR_PIPA, Pipa[i][1], GREEN);
        DrawRectangle(Pipa[i][0], Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA, TINGGI_LAYAR - Pipa[i][1] - JARAK_PIPA_ATAS_BAWAH, GREEN);
    }
}


int main() {
    InitWindow(LEBAR_LAYAR, TINGGI_LAYAR, "Flappy Bird Pipes");
    SetTargetFPS(60);
    srand(time(NULL));
    int Pipa[3][3];      
    Buat_pipa(Pipa);
    // cukup Pipa tidak perlu Pipa[3][3] karena sudah mengirim alamat array
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        Pergerakan_pipa(Pipa);   
        Gambar_pipa(Pipa);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
