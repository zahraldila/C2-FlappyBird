#include "Alexandrio.h"
#include "zakky.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>


int Awan[5][2]; // [x, y]
void Buat_awan(int Awan[5][2]) 
{
    int i = 0;
    while (i < 5)
    {
        Awan[i][0] = LEBAR_LAYAR + (i * 200); // Awalnya di luar layar
        Awan[i][1] = rand() % 150; // Posisi y acak di bagian atas layar
        i++;
    }
}

void Pergerakan_awan(int Awan[5][2]) 
{
    int i = 0;
    while (i < 5)
    {
        Awan[i][0] -= KECEPATAN_AWAN; // Geser awan ke kiri

        // Jika awan sudah cukup keluar dari layar
        if (Awan[i][0] < 0)  
        {  
            // Reset posisi x ke sisi kanan layar dengan jarak acak
            Awan[i][0] = LEBAR_LAYAR + 20;
            // Reset posisi y dengan variasi acak
            Awan[i][1] = rand() % (TINGGI_LAYAR / 2);

            printf("Awan %d direset ke posisi (%d, %d)\n", i, Awan[i][0], Awan[i][1]);
        }
    }
}

void Gambar_awan(int Awan[5][2]) 
{
    for (int i = 0; i < 5; i++) {
        DrawCircle(Awan[i][0], Awan[i][1], 20, WHITE);
        DrawCircle(Awan[i][0] + 15, Awan[i][1] + 5, 15, WHITE);
        DrawCircle(Awan[i][0] - 15, Awan[i][1] + 5, 15, WHITE);
    }
}





