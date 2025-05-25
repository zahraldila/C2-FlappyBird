#include "raylib.h"
#include "Alexandrio.h" // Untuk konstanta dan extern plist, tplist
#include "qlio.h"       // Untuk variabel score (s)
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Variabel global plist dan tplist sudah didefinisikan di PipaLinkedList.c
// extern Singlelinkedlist *plist;
// extern Singlelinkedlist *tplist;

int Gerak[3]; // Ukuran array Gerak harus cukup untuk jumlah maksimum pipa yang mungkin ada di layar secara bersamaan. 3 mungkin cukup.
Color TANAH = {240, 164, 0 , 255};
Color RUMPUT = {0, 150, 0 , 255};
bool Stop = true; // Variabel ini mengontrol apakah pipa bergerak naik/turun

void Buat_pipa_linkedlist() {
    // freeList(); // Panggil freeList di main.c atau game_state.c sebelum memanggil ini jika ingin mereset total
    // initList(plist); // Sudah di-malloc dan init di main.c
    // initList(tplist);

    for(int i = 0; i < 3; i++) { // Membuat 3 pasang pipa awal
        address newNodePipa = buatNodePipa(i); // Node untuk plist (mengandung tinggi pipa atas)
        if (newNodePipa == NULL) continue; // Lewati jika gagal alokasi

        // Node untuk tplist, korx sama, tinggi menyalin dari newNodePipa (karena p->tinggi dipakai sebagai ref)
        address newNodeTPipa = buatNodeTPipa(i, newNodePipa->tinggi);
        if (newNodeTPipa == NULL) {
            free(newNodePipa); // Bebaskan node pipa jika pasangannya gagal dibuat
            continue;
        }

        Gerak[i] = (rand() % 2); // 0 untuk turun, 1 untuk naik (atau sebaliknya)
        insertBelakang(newNodePipa, newNodeTPipa);
    }
}

void Pergerakan_pipa(){
    if (!plist || !tplist) return; // Guard clause

    address p = plist->head;
    address t = tplist->head;

    while (p != NULL) { // Cukup iterasi satu list karena panjangnya harusnya sama
        p->korx -= KECEPATAN_PIPA;
        if (t) t->korx -= KECEPATAN_PIPA; // Gerakkan t jika t masih ada

        if (p->korx + LEBAR_PIPA < 0) { // Jika pipa keluar layar
            deleteFirst(); // Hapus pasangan pipa terdepan

            // Buat pasangan pipa baru di belakang
            address newNodePipa = buatNodePipa(2); // parameter i di sini mungkin perlu disesuaikan jika ingin posisi spawn yang lebih dinamis
            if (!newNodePipa) return; // Gagal buat node baru, mungkin keluar atau coba lagi nanti
            address newNodeTPipa = buatNodeTPipa(2, newNodePipa->tinggi);
            if (!newNodeTPipa) {
                free(newNodePipa);
                return;
            }
            // Untuk pipa baru, tentukan arah gerak awalnya
            // Perlu cara untuk mengelola array 'Gerak' jika jumlah pipa dinamis dan lebih dari 3
            // Untuk sementara, kita bisa menggeser atau merandom ulang untuk pipa baru
            // Misal, Gerak[0] = Gerak[1]; Gerak[1] = Gerak[2]; Gerak[2] = rand() % 2; (jika selalu 3 pipa)
            // Atau jika kita tahu pipa mana yang dihapus dan mana yang ditambahkan.
            // Jika menggunakan 3 pipa statis yang didaur ulang, indeks bisa tetap.

            insertBelakang(newNodePipa, newNodeTPipa);
            // Karena kita hanya menghapus dan menambah satu, loop bisa di-break jika hanya ingin proses 1 pipa per frame
            // Namun, jika KECEPATAN_PIPA besar, mungkin perlu cek semua.
            // Untuk Flappy Bird standar, cukup proses satu yang keluar.
            return; // Diasumsikan hanya satu pipa keluar per frame
        }

        p = p->next;
        if (t) t = t->next;
    }
}

void Gambar_pipa(int s) {
    if (!plist) return;

    address p = plist->head;
    // address t = tplist->head; // Tidak perlu iterasi t jika tidak dipakai untuk atribut unik
    int pipa_idx = 0; // Untuk mengakses array Gerak[]

    float tutup_lebar = LEBAR_PIPA + 20;
    float tutup_tinggi = 30;
    float tutup_offset_x = (LEBAR_PIPA - tutup_lebar) / 2; // Agar tutup simetris

    while(p != NULL) {
        // Logika naik turun pipa
        if(s > 14 && Stop) { // s adalah skor
            if (pipa_idx < 3) { // Pastikan tidak keluar batas array Gerak
                if (Gerak[pipa_idx] == 0) { // Turun
                    p->tinggi += 1;
                    if (p->tinggi >= TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 50 - tutup_tinggi) { // -50 batas, -tutup_tinggi agar tutup tidak keluar
                        Gerak[pipa_idx] = 1;
                    }
                } else { // Naik
                    p->tinggi -= 1;
                    if (p->tinggi <= 50 + tutup_tinggi) { // +50 batas, +tutup_tinggi agar tutup tidak keluar
                        Gerak[pipa_idx] = 0;
                    }
                }
            }
        }

        // Gambar PIPA ATAS
        // Badan pipa atas
        DrawRectangle(p->korx, 0, LEBAR_PIPA, p->tinggi, GREEN);
        // Tutup pipa atas (bagian bawah pipa atas)
        DrawRectangle(p->korx + tutup_offset_x, p->tinggi - tutup_tinggi, tutup_lebar, tutup_tinggi, GREEN);

        // Gambar PIPA BAWAH
        float y_pipa_bawah = p->tinggi + JARAK_PIPA_ATAS_BAWAH;
        float tinggi_pipa_bawah = TINGGI_LAYAR - y_pipa_bawah;
        // Badan pipa bawah
        DrawRectangle(p->korx, y_pipa_bawah, LEBAR_PIPA, tinggi_pipa_bawah, GREEN);
        // Tutup pipa bawah (bagian atas pipa bawah)
        DrawRectangle(p->korx + tutup_offset_x, y_pipa_bawah, tutup_lebar, tutup_tinggi, GREEN);

        p = p->next;
        // if (t) t = t->next; // Jika tplist masih dipakai
        pipa_idx++;
    }

    // Gambar tanah dan rumput setelah semua pipa
    DrawRectangle(0, TINGGI_LAYAR - 30, LEBAR_LAYAR, 30, TANAH); // Tanah dibuat lebih sederhana
    DrawRectangle(0, TINGGI_LAYAR - 30 - 10, LEBAR_LAYAR, 10, RUMPUT); // Rumput di atas tanah
}

void Pipa_berhenti(bool cek){
    Stop = cek;
}