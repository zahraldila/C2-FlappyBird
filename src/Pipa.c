#include "raylib.h"
#include "Alexandrio.h" // Untuk konstanta LEBAR_LAYAR, TINGGI_LAYAR, dll.
#include "qlio.h"       // Untuk 'score'
#include "pipa_ll.h"    // Untuk address, dll.
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>      // Untuk TraceLog via raylib

// plist dan tplist global, di-extern dari Alexandrio.h, didefinisikan di PipaLinkedList.c

int Gerak[3]; // Untuk 3 pasang pipa yang didaur ulang
Color TANAH = {240, 164, 0 , 255};
Color RUMPUT = {0, 150, 0 , 255};
bool Stop = true;

// Fungsi utilitas untuk menghasilkan tinggi pipa atas yang valid
int generateValidPipeHeight() {
    int minHeight = 75;
    int maxHeight = TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 75; // Memastikan ada ruang untuk pipa bawah & celah
    if (minHeight >= maxHeight) { // Jika layar terlalu kecil atau jarak pipa terlalu besar
        return TINGGI_LAYAR / 3; // Nilai aman default
    }
    int randomHeight = rand() % (maxHeight - minHeight + 1) + minHeight;
    return randomHeight;
}

void Buat_pipa_linkedlist() {
    if (!plist || !tplist) {
        TraceLog(LOG_ERROR, "PIPA: plist atau tplist belum dialokasi saat Buat_pipa_linkedlist!");
        return;
    }
    // freeListPasangan(); // PENTING: Pastikan ini dipanggil SEBELUM Buat_pipa_linkedlist
                        // di main.c atau game_state.c setiap kali game direset/dimulai.

    int initial_pipe_count = 0;
    for(int i = 0; i < 3; i++) { // Membuat 3 pasang pipa awal
        int tinggi_pipa_atas = generateValidPipeHeight();

        address newNodePipa = buatNodePipaGenerik(tinggi_pipa_atas);
        if (!newNodePipa) continue; // Lewati jika gagal
        newNodePipa->korx = LEBAR_LAYAR + i * 300; // Jarak antar pipa awal 300

        address newNodeTPipa = buatNodePipaGenerik(tinggi_pipa_atas); // Data tinggi di tplist bisa jadi tidak krusial jika p->tinggi yang dipakai
        if (!newNodeTPipa) {
            free(newNodePipa);
            continue;
        }
        newNodeTPipa->korx = newNodePipa->korx; // korx sama

        Gerak[i] = (rand() % 2);
        insertBelakangPasangan(newNodePipa, newNodeTPipa);
        initial_pipe_count++;
    }
    // TraceLog(LOG_INFO, "PIPA: %d pasang pipa awal dibuat.", initial_pipe_count);
}

void Pergerakan_pipa(){
    if (!plist || !plist->head) return; // Tidak ada pipa untuk digerakkan

    // 1. Gerakkan semua pipa yang ada
    address current_p = plist->head;
    address current_t = (tplist && tplist->head) ? tplist->head : NULL;
    while (current_p != NULL) {
        current_p->korx -= KECEPATAN_PIPA;
        if (current_t) {
            current_t->korx = current_p->korx; // Pastikan korx tplist sinkron
            current_t = current_t->next;
        }
        current_p = current_p->next;
    }

    // 2. Cek apakah pipa paling depan sudah keluar layar
    //    Jika ya, hapus dan tambahkan yang baru di belakang.
    //    Ini menjaga jumlah pipa tetap (misalnya 3 pasang).
    current_p = plist->head; // Kembali ke head untuk cek
    if (current_p != NULL && (current_p->korx + LEBAR_PIPA < 0)) {
        deleteFirstPasangan(); // Hapus pasangan pipa terdepan

        int tinggi_pipa_atas_baru = generateValidPipeHeight();
        address newPipaNode = buatNodePipaGenerik(tinggi_pipa_atas_baru);
        if (!newPipaNode) {
            TraceLog(LOG_ERROR, "PIPA: Gagal membuat newPipaNode saat daur ulang!");
            return;
        }
        address newTPipaNode = buatNodePipaGenerik(tinggi_pipa_atas_baru);
        if (!newTPipaNode) {
            free(newPipaNode);
            TraceLog(LOG_ERROR, "PIPA: Gagal membuat newTPipaNode saat daur ulang!");
            return;
        }

        // Tentukan korx pipa baru berdasarkan pipa terakhir yang ADA di list
        if (plist->tail != NULL) {
            newPipaNode->korx = plist->tail->korx + 300; // Jarak 300 dari pipa terakhir
        } else {
            // Jika list kebetulan kosong (seharusnya tidak terjadi jika selalu daur ulang dari 3 pipa)
            newPipaNode->korx = LEBAR_LAYAR; // Posisi default pipa pertama
        }
        newTPipaNode->korx = newPipaNode->korx; // Samakan korx

        // Daur ulang status gerakan untuk array 'Gerak'
        // Ini asumsi ada 3 pipa yang dilacak status geraknya
        Gerak[0] = Gerak[1];
        Gerak[1] = Gerak[2];
        Gerak[2] = rand() % 2; // Status gerak acak untuk pipa baru

        insertBelakangPasangan(newPipaNode, newTPipaNode);
        // TraceLog(LOG_DEBUG, "PIPA: Pipa didaur ulang. Korx baru: %d", newPipaNode->korx);
    }
}

void Gambar_pipa(int s) {
    // DEBUG: Hitung jumlah node di plist untuk ditampilkan di layar
    int node_count = 0;
    if (plist) {
        address iter_count = plist->head;
        while (iter_count != NULL) {
            node_count++;
            iter_count = iter_count->next;
        }
    }
    // DrawText(TextFormat("Pipes in list: %d", node_count), 10, SCREEN_HEIGHT - 25, 20, BLACK);

    if (!plist || !plist->head) return;

    address p = plist->head;
    int pipa_idx_gerak = 0;

    float tutup_lebar_visual = LEBAR_PIPA + 20;
    float tutup_tinggi_visual = 30;
    float tutup_offset_x = (LEBAR_PIPA - tutup_lebar_visual) / 2.0f;

    while(p != NULL) {
        if (pipa_idx_gerak < 3 && s > 14 && Stop) {
            if (Gerak[pipa_idx_gerak] == 0) {
                p->tinggi += 1;
                if (p->tinggi >= TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 75 - tutup_tinggi_visual) {
                    p->tinggi = TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 75 - tutup_tinggi_visual;
                    Gerak[pipa_idx_gerak] = 1;
                }
            } else {
                p->tinggi -= 1;
                if (p->tinggi <= 75 + tutup_tinggi_visual) {
                    p->tinggi = 75 + tutup_tinggi_visual;
                    Gerak[pipa_idx_gerak] = 0;
                }
            }
        }

        // PIPA ATAS (p->tinggi adalah tinggi visual pipa atas)
        DrawRectangle(p->korx, 0, LEBAR_PIPA, p->tinggi, GREEN);
        DrawRectangle(p->korx + tutup_offset_x, p->tinggi - tutup_tinggi_visual, tutup_lebar_visual, tutup_tinggi_visual, DARKGREEN);

        // PIPA BAWAH
        float y_pipa_bawah_mulai = p->tinggi + JARAK_PIPA_ATAS_BAWAH;
        float tinggi_pipa_bawah_visual = TINGGI_LAYAR - y_pipa_bawah_mulai;
        DrawRectangle(p->korx, y_pipa_bawah_mulai, LEBAR_PIPA, tinggi_pipa_bawah_visual, GREEN);
        DrawRectangle(p->korx + tutup_offset_x, y_pipa_bawah_mulai, tutup_lebar_visual, tutup_tinggi_visual, DARKGREEN);

        p = p->next;
        pipa_idx_gerak = (pipa_idx_gerak + 1) % 3; // Untuk array Gerak
    }

    DrawRectangle(0, TINGGI_LAYAR - 30, LEBAR_LAYAR, 30, TANAH);
    DrawRectangle(0, TINGGI_LAYAR - 30 - 10, LEBAR_LAYAR, 10, RUMPUT);
}

void Pipa_berhenti(bool cek){
    Stop = cek;
}