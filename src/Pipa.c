#include "raylib.h"
#include "Alexandrio.h"
#include "qlio.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>



// Di luar fungsi (global scope)
Singlelinkedlist *plist = KOSONG;
Singlelinkedlist *tplist = KOSONG;
int Gerak[3];
Color TANAH = {240, 164, 0 , 255};
Color RUMPUT = {0, 150, 0 , 255};
bool Stop = true;

void Buat_pipa() {
    for(int PIPA_KE = 0; PIPA_KE < 3; PIPA_KE++) {
        address newNodePipa = buatNodePipa(PIPA_KE);
        address newNodeTPipa = buatNodeTPipa(PIPA_KE, newNodePipa->tinggi);
        // Simpan gerakan awal 
        Gerak[PIPA_KE] = (rand() % 2) ? 1 : 0;
        insertBelakang(newNodePipa, newNodeTPipa);
    }
}

void Pergerakan_pipa(){
    address pipaTemp = plist->head;
    address tpipaTemp = tplist->head;

    while (pipaTemp != KOSONG && tpipaTemp != KOSONG) {
        pipaTemp->korx -= KECEPATAN_PIPA;
        tpipaTemp->korx -= KECEPATAN_PIPA;

        if (pipaTemp->korx + LEBAR_PIPA < 0) {
            deleteFirst();
            address newNodePipa = buatNodePipa(0); 
            address newNodeTPipa = buatNodeTPipa(0, newNodePipa->tinggi);
            insertBelakang(newNodePipa, newNodeTPipa);
            return; 
        }

        pipaTemp = pipaTemp->next;
        tpipaTemp = tpipaTemp->next;
    }
}


void Gambar_pipa(int Skor){
    address pipaTemp = plist->head;
    address tpipaTemp = tplist->head;
    int i = 0;

    while(pipaTemp != KOSONG && tpipaTemp != KOSONG) {
        if(Skor > 14 && Stop) {
            // Update posisi naik turun per node
            if (Gerak[i] == 0) {
                pipaTemp->tinggi += 1; 
                if (pipaTemp->tinggi >= TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 50) {
                    Gerak[i] = 1;
                }
            } else {
                pipaTemp->tinggi -= 1;
                if (pipaTemp->tinggi <= 50) {
                    Gerak[i] = 0;
                }
            }
        }

        // Gambar setiap pipa
        DrawRectangle(pipaTemp->korx, 0, LEBAR_PIPA, pipaTemp->tinggi, GREEN);
        DrawRectangle(tpipaTemp->korx, pipaTemp->tinggi - 30, LEBAR_PIPA + 20, 30, GREEN);
        DrawRectangle(pipaTemp->korx, pipaTemp->tinggi + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA, TINGGI_LAYAR - pipaTemp->tinggi - JARAK_PIPA_ATAS_BAWAH, GREEN);
        DrawRectangle(tpipaTemp->korx, pipaTemp->tinggi + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA + 20, 30, GREEN);
        pipaTemp = pipaTemp->next;
        tpipaTemp = tpipaTemp->next;
        i++;
    }
    DrawRectangle(0, 420, LEBAR_LAYAR, 100, TANAH);
    DrawRectangle(0, 420, LEBAR_LAYAR, 10, RUMPUT);
}


void Hapus_semua_pipa(){
    if (plist != KOSONG && tplist != KOSONG){ 
    freeList();
    }
}

void Pipa_berhenti(bool Cek){
    Stop = Cek;
}







// Color TANAH = {240, 164, 0 , 255};
// Color RUMPUT = {0, 150, 0 , 255};
// int Pipa[3][3], TutupPipa[3][3], Gerak[3];
// bool Stop = true;
// void Buat_pipa(int Pipa[3][3], int TutupPipa[3][3]){
//     for(int i = 0; i < 3; i++){
//         Pipa[i][0] = (LEBAR_LAYAR + i * 300) + 10;
//         Pipa[i][1] = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50;
//         Pipa[i][2] = 0;
//         TutupPipa[i][0] = (LEBAR_LAYAR + i * 300);
//         TutupPipa[i][1] = Pipa[i][2];
//         TutupPipa[i][2] = 0;
//         Gerak[i] = (rand()% 2) ? 1 : 0;
//     }   
// }

// void Pergerakan_pipa(int Pipa[3][3], int TutupPipa[3][3]){
//     for(int i = 0; i < 3; i++){
//         Pipa[i][0] -= KECEPATAN_PIPA;
//         TutupPipa[i][0] -= KECEPATAN_PIPA;
//         if(Pipa[i][0] + LEBAR_PIPA < 0){
//             Pipa[i][0] = LEBAR_LAYAR + 20;
//             Pipa[i][1] = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50;
//             Pipa[i][2] = 0;   
//         }
//         if(TutupPipa[i][0] + LEBAR_PIPA + 20 < 0){
//             TutupPipa[i][0] = LEBAR_LAYAR;
//             TutupPipa[i][1] = Pipa[i][2];
//             TutupPipa[i][2] = 0;
//         }
//     }
// }


// void Gambar_pipa(int Pipa[3][3], int TutupPipa[3][3], int s){
//     for(int i = 0; i < 3; i++){
//             if(s > 14 && Stop){
//                 Pipa_naik_turun(i);
//             }    
//             Munculkan_Pipa(i);
//     }
// }

// void Pipa_naik_turun(int i){    
//     if (Gerak[i] == 0) {
//         Pipa[i][1] += 1; 
//         if (Pipa[i][1] >= TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 50) {
//             Gerak[i] = 1; // Ubah arah jadi naik
//         }
//     } else { 
//         Pipa[i][1] -= 1; 
//         if (Pipa[i][1] <= 50) {
//             Gerak[i] = 0; // Ubah arah jadi turun
//         }
//     }
// }

// void Munculkan_Pipa(int i){
//     for(i ; i < 3; i ++){
//         DrawRectangle(Pipa[i][0], 0, LEBAR_PIPA, Pipa[i][1], GREEN);
//         DrawRectangle(TutupPipa[i][0], Pipa[i][1] - 30, LEBAR_PIPA + 20, 30, GREEN);
//         DrawRectangle(Pipa[i][0], Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA, TINGGI_LAYAR - Pipa[i][1] - JARAK_PIPA_ATAS_BAWAH, GREEN);
//         DrawRectangle(TutupPipa[i][0], Pipa[i][1] + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA + 20, 30, GREEN);    
//         DrawRectangle(0, 420, LEBAR_LAYAR, 100, TANAH);
//         DrawRectangle(0, 420, LEBAR_LAYAR, 10, RUMPUT);
//     }
// }

// void Pipa_berhenti(bool Cek){
//     Stop = Cek;
// }