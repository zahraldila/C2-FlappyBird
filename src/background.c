#include "Alexandrio.h"
#include "zakky.h"
#include <stdlib.h>

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
        if (Awan[i][0] + 50 < 0) 
        { // Jika awan keluar layar, pindahkan ke kanan
            Awan[i][0] = LEBAR_LAYAR;
            Awan[i][1] = rand() % 100;
        }
        i++;   
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





