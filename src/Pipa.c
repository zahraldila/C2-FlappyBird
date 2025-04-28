#include "raylib.h"
#include "Alexandrio.h"
#include "qlio.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

Color TANAH = {240, 164, 0 , 255};
Color RUMPUT = {0, 150, 0 , 255};
int Pipa[3][3], TutupPipa[3][3], Gerak[3];
bool Stop = true;

void Buat_pipa(int Pipa[3][3], int TutupPipa[3][3]){
    for(int i = 0; i < 3; i++){
        Pipa[i][0] = (LEBAR_LAYAR + i * 300) + 10;
        Pipa[i][1] = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50;
        Pipa[i][2] = 0;
        TutupPipa[i][0] = (LEBAR_LAYAR + i * 300);
        TutupPipa[i][1] = Pipa[i][2];
        TutupPipa[i][2] = 0;
        Gerak[i] = (rand()% 2) ? 1 : 0;
    }   
}

void Pergerakan_pipa(int Pipa[3][3], int TutupPipa[3][3]){
    for(int i = 0; i < 3; i++){
        Pipa[i][0] -= KECEPATAN_PIPA;
        TutupPipa[i][0] -= KECEPATAN_PIPA;
        if(Pipa[i][0] + LEBAR_PIPA < 0){
            Pipa[i][0] = LEBAR_LAYAR + 20;
            Pipa[i][1] = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50;
            Pipa[i][2] = 0;   
        }
        if(TutupPipa[i][0] + LEBAR_PIPA + 20 < 0){
            TutupPipa[i][0] = LEBAR_LAYAR;
            TutupPipa[i][1] = Pipa[i][2];
            TutupPipa[i][2] = 0;
        }
    }
}


void Gambar_pipa(int Pipa[3][3], int TutupPipa[3][3], int s){
    for(int i = 0; i < 3; i++){
            if(s > 14 && Stop){
                Pipa_naik_turun(i);
            }    
            Munculkan_Pipa(i);
    }
}

void Pipa_naik_turun(int i){    
    if (Gerak[i] == 0) {
        Pipa[i][1] += 1; 
        if (Pipa[i][1] >= TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 50) {
            Gerak[i] = 1; // Ubah arah jadi naik
        }
    } else { 
        Pipa[i][1] -= 1; 
        if (Pipa[i][1] <= 50) {
            Gerak[i] = 0; // Ubah arah jadi turun
        }
    }
}

void Munculkan_Pipa(int i){
    for(i ; i < 3; i ++){
        DrawRectangle(Pipa[i][0], 0, LEBAR_PIPA, Pipa[i][1], GREEN);
        DrawRectangle(TutupPipa[i][0], Pipa[i][1] - 30, LEBAR_PIPA + 20, 30, GREEN);
        DrawRectangle(Pipa[i][0], Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA, TINGGI_LAYAR - Pipa[i][1] - JARAK_PIPA_ATAS_BAWAH, GREEN);
        DrawRectangle(TutupPipa[i][0], Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA + 20, 30, GREEN);    
        DrawRectangle(0, 420, LEBAR_LAYAR, 100, TANAH);
        DrawRectangle(0, 420, LEBAR_LAYAR, 10, RUMPUT);
    }
}

void Pipa_berhenti(bool Cek){
    Stop = Cek;
}

