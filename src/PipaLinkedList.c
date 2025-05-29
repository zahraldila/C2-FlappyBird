#include "Alexandrio.h"
#include <stdlib.h>
#include <stdio.h>

void initPipaList(){
    initList(plist);
    initList(tplist);
}

void initList(Singlelinkedlist *L){    
    L->head = KOSONG;
    L->tail = KOSONG;
}

address buatNodePipa(int i){
    address newNode = (address)malloc(sizeof(Node));
    newNode->korx = (LEBAR_LAYAR + i * 300) + 10;
    newNode->tinggi = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50 ;
    newNode->status = 0;
    newNode->next = KOSONG;
    return newNode;
}
address buatNodeTPipa(int i, int tinggi){
    address newNode = (address)malloc(sizeof(Node));
    newNode->korx = (LEBAR_LAYAR + i * 300);
    newNode->tinggi = tinggi;
    newNode->status = 0;
    newNode->next = KOSONG;
    return newNode;
}


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