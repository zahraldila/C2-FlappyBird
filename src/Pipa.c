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

//Modul untuk inisialisasi objek pipa beserta pergerakan vertikalnya ketika skornya sudah lebih dari 14
void Buat_pipa() { 
    for(int PIPA_KE = 0; PIPA_KE < 3; PIPA_KE++) {
        address newNodePipa = buatNodePipa(PIPA_KE);
        address newNodeTPipa = buatNodeTPipa(PIPA_KE, newNodePipa->tinggi);
        // Simpan gerakan awal 
        Gerak[PIPA_KE] = (rand() % 2) ? 1 : 0;
        insertBelakang(newNodePipa, newNodeTPipa);
    }
}
//Modul untuk mengatur pergerakan pipa secara horizontal dari kanan ke kiri
void Pergerakan_pipa(){
    address pipaTemp = plist->head;
    address tpipaTemp = tplist->head;

    while (pipaTemp != KOSONG && tpipaTemp != KOSONG) {
        pipaTemp->korx -= KECEPATAN_PIPA;
        tpipaTemp->korx -= KECEPATAN_PIPA;
        //jika pipa sudah keluar dari layar maka pipa tersebut akan dihapus dan digantikan dengan pipa yang baru dengan modul insertBelakang
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
//modul yang berisi proses untuk menampilkan pipa ke layar
void Gambar_pipa(int Skor){
    address pipaTemp = plist->head;
    address tpipaTemp = tplist->head;
    int i = 0;

    while(pipaTemp != KOSONG && tpipaTemp != KOSONG) {
        //Jika skor pemain lebih dari 14 (yaitu 15) maka pipa akan bergerak naik turun (vertikal) selain bergerak secara horizontal
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
        Munculkan_Pipa(pipaTemp, tpipaTemp);
        pipaTemp = pipaTemp->next;
        tpipaTemp = tpipaTemp->next;
        i++;
    }
    DrawRectangle(0, 420, LEBAR_LAYAR, 100, TANAH);
    DrawRectangle(0, 420, LEBAR_LAYAR, 10, RUMPUT);
}


//Modul untuk memunculkan gambar objek pipa ke layar
void Munculkan_Pipa(address pipaTemp,address tpipaTemp){
    DrawRectangle(pipaTemp->korx, 0, LEBAR_PIPA, pipaTemp->tinggi, GREEN);
    DrawRectangle(tpipaTemp->korx, pipaTemp->tinggi - 30, LEBAR_PIPA + 20, 30, GREEN);
    DrawRectangle(pipaTemp->korx, pipaTemp->tinggi + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA, TINGGI_LAYAR - pipaTemp->tinggi - JARAK_PIPA_ATAS_BAWAH, GREEN);
    DrawRectangle(tpipaTemp->korx, pipaTemp->tinggi + JARAK_PIPA_ATAS_BAWAH, LEBAR_PIPA + 20, 30, GREEN);
}

//Modul untuk menghapus semua pipa agar tidak terjadi memory leak
void Hapus_semua_pipa(){
    if (plist != KOSONG && tplist != KOSONG){ 
    freeList();
    }
}

//Modul untuk menghentikan pergerakan pipa (dipanggil saat game over)
void Pipa_berhenti(bool Cek){
    Stop = Cek;
}