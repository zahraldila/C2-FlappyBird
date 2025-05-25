#include "Alexandrio.h" // Untuk konstanta LEBAR_LAYAR, TINGGI_LAYAR, JARAK_PIPA_ATAS_BAWAH
#include "pipa_ll.h"    // Untuk definisi struct Node, Singlelinkedlist, address
#include <stdlib.h>
#include <stdio.h>

// Definisi variabel global plist dan tplist
Singlelinkedlist *plist = NULL;
Singlelinkedlist *tplist = NULL;

void initList(Singlelinkedlist *L){
    L->head = NULL;
    L->tail = NULL;
}

address buatNodePipa(int i){ // Untuk plist (pipa bawah, tapi tingginya akan jadi referensi tinggi pipa atas)
    address newNode = (address)malloc(sizeof(Node));
    if (newNode == NULL) {
        // Handle alokasi gagal jika perlu, misalnya dengan TraceLog atau exit
        printf("Gagal alokasi memori untuk newNodePipa!\n");
        return NULL;
    }
    newNode->korx = (LEBAR_LAYAR + i * 300); // Dibuat sama untuk pasangan pipa
    // 'tinggi' di sini akan dianggap sebagai tinggi visual pipa ATAS
    newNode->tinggi = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 100) + 50 ; // -100 agar tidak terlalu pendek/tinggi, +50 batas bawah
    if (newNode->tinggi < 50) newNode->tinggi = 50; // Pastikan tinggi minimal
    if (newNode->tinggi > TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 50) newNode->tinggi = TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 50; // Pastikan tinggi maksimal

    newNode->status = 0;
    newNode->next = NULL;
    return newNode;
}

// Node untuk tplist (pipa atas). Jika tplist hanya untuk korx (yang sekarang sama)
// atau status (yang juga bisa disamakan), mungkin bisa disederhanakan.
// Untuk sekarang, kita buat korx-nya sama dan tingginya menyalin dari p->tinggi
// karena logika gambar dan kolisi akan mengacu pada p->tinggi untuk pipa atas.
address buatNodeTPipa(int i, int tinggi_pipa_atas_referensi){
    address newNode = (address)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Gagal alokasi memori untuk newNodeTPipa!\n");
        return NULL;
    }
    newNode->korx = (LEBAR_LAYAR + i * 300); // Sama dengan pasangannya di plist
    newNode->tinggi = tinggi_pipa_atas_referensi; // Menyalin tinggi pipa atas dari pasangannya
    newNode->status = 0; // Status bisa jadi independen jika diperlukan
    newNode->next = NULL;
    return newNode;
}

void insertBelakang(address newNodePipa, address newNodeTPipa) {
    if (newNodePipa == NULL || newNodeTPipa == NULL) return; // Jangan proses jika node gagal dibuat

    if(plist->head == NULL && tplist->head == NULL){
        plist->head = plist->tail = newNodePipa;
        tplist->head = tplist->tail = newNodeTPipa;
    } else {
        if (plist->tail) plist->tail->next = newNodePipa;
        plist->tail = newNodePipa;

        if (tplist->tail) tplist->tail->next = newNodeTPipa;
        tplist->tail = newNodeTPipa;
    }
}

void deleteFirst(){
    if(plist->head == NULL || tplist->head == NULL){
        // printf("List kosong, tidak bisa dihapus.\n"); // Komentari jika terlalu berisik
        return;
    }

    // Hapus node dari plist
    if (plist->head) {
        address tempPipa = plist->head;
        plist->head = plist->head->next;
        if(plist->head == NULL) {
            plist->tail = NULL;
        }
        free(tempPipa);
    }


    // Hapus node dari tplist
    if (tplist->head) {
        address tempTPipa = tplist->head;
        tplist->head = tplist->head->next;
        if(tplist->head == NULL) {
            tplist->tail = NULL;
        }
        free(tempTPipa);
    }
}

void freeList(){
    // Kosongkan plist
    address currentP = plist->head;
    while(currentP != NULL){
        address temp = currentP;
        currentP = currentP->next;
        free(temp);
    }
    if (plist) { // Hanya jika plist tidak NULL
      plist->head = plist->tail = NULL;
    }


    // Kosongkan tplist
    address currentT = tplist->head;
    while(currentT != NULL){
        address temp = currentT;
        currentT = currentT->next;
        free(temp);
    }
    if (tplist) { // Hanya jika tplist tidak NULL
      tplist->head = tplist->tail = NULL;
    }
}