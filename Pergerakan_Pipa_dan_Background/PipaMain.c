#include "raylib.h"
#include "Alexandrio.h"
#include <stdlib.h>
#include <time.h>


int main() {
    InitWindow(LEBAR_LAYAR, TINGGI_LAYAR, "Flappy Bird Pipes");
    SetTargetFPS(60);
    srand(time(NULL));
    int Pipa[3][3], TutupPipa[3][3];      
    Buat_pipa(Pipa, TutupPipa);
    // cukup Pipa tidak perlu Pipa[3][3] karena sudah mengirim alamat array
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        Pergerakan_pipa(Pipa, TutupPipa);   
        Gambar_pipa(Pipa, TutupPipa);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}