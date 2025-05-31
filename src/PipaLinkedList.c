#include "Alexandrio.h"
#include <stdlib.h>
#include <stdio.h>

//modul untuk inisialisasi linked list
void initPipaList(){
    initList(plist);
    initList(tplist);
}

void initList(Singlelinkedlist *L){    
    L->head = KOSONG;
    L->tail = KOSONG;
}

//modul untuk menambahkan data ke linked list (plist)
//data berupa tinggi pipa, posisi kordinat x pipa, serta status nya berupa 0 berarti pipa belum dilewati objek burung
address buatNodePipa(int i){
    address newNode = (address)malloc(sizeof(Node));
    newNode->korx = (LEBAR_LAYAR + i * 300) + 10;
    newNode->tinggi = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50 ;
    newNode->status = 0;
    newNode->next = KOSONG;
    return newNode;
}

//modul untuk menambahkan data ke linked list (tplist) = tutup pipa
//data berupa tinggi tutup pipa, posisi kordinat x tutup pipa, serta status nya berupa 0 berarti tutup pipa belum dilewati objek burung
address buatNodeTPipa(int i, int tinggi){
    address newNode = (address)malloc(sizeof(Node));
    newNode->korx = (LEBAR_LAYAR + i * 300);
    newNode->tinggi = tinggi;
    newNode->status = 0;
    newNode->next = KOSONG;
    return newNode;
}

//modul untuk menginsert (menambah node) dibelakang list
//modul dipanggil ketika pipa di node terdepan sudah keluar dari frame/layar
void insertBelakang(address newNodePipa, address newNodeTPipa) {
if(plist->head == KOSONG && tplist->head == KOSONG){
    plist->head = plist->tail = newNodePipa;
    tplist->head = tplist->tail = newNodeTPipa;
} else {
    plist->tail->next = newNodePipa;
    plist->tail = newNodePipa;

    tplist->tail->next = newNodeTPipa;
    tplist->tail = newNodeTPipa;
}
}

//modul untuk menghapus node pertama
//modul dipanggil ketika pipa di node terdepan sudah keluar dari frame/layar
void deleteFirst(){
    if(plist->head == KOSONG || tplist->head == KOSONG){
        printf("List kosong, tidak bisa dihapus.\n");
        return;
    }

    // Hapus node dari plist
    address tempPipa = plist->head;
    plist->head = plist->head->next;
    if(plist->head == KOSONG) {
        plist->tail = KOSONG;
    }
    free(tempPipa);

    // Hapus node dari tplist
    address tempTPipa = tplist->head;
    tplist->head = tplist->head->next;
    if(tplist->head == KOSONG) {
        tplist->tail = KOSONG;
    }
    free(tempTPipa);
}

//modul untuk membebaskan semua node dari memori
void freeList(){
    // Kosongkan plist
    address pipaSaatIni = plist->head;
    while(pipaSaatIni != KOSONG){
        address temp = pipaSaatIni;
        pipaSaatIni = pipaSaatIni->next;
        free(temp);
    }
    plist->head = plist->tail = KOSONG;

    // Kosongkan tplist
    address tpipaSaatIni = tplist->head;
    while(tpipaSaatIni != KOSONG){
        address temp = tpipaSaatIni;
        tpipaSaatIni = tpipaSaatIni->next;
        free(temp);
    }
    tplist->head = tplist->tail = KOSONG;
}