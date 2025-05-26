#include "Alexandrio.h"
#include <stdlib.h>
#include <stdio.h>

void initPipaList(){
    initList(plist);
    initList(tplist);
}

void initList(Singlelinkedlist *L){    
    L->head = NULL;
    L->tail = NULL;
}

address buatNodePipa(int i){
    address newNode = (address)malloc(sizeof(Node));
    newNode->korx = (LEBAR_LAYAR + i * 300) + 10;
    newNode->tinggi = rand() % (TINGGI_LAYAR - JARAK_PIPA_ATAS_BAWAH - 150) + 50 ;
    newNode->status = 0;
    newNode->next = NULL;
    return newNode;
}
address buatNodeTPipa(int i, int tinggi){
    address newNode = (address)malloc(sizeof(Node));
    newNode->korx = (LEBAR_LAYAR + i * 300);
    newNode->tinggi = tinggi;
    newNode->status = 0;
    newNode->next = NULL;
    return newNode;
}


void insertBelakang(address newNodePipa, address newNodeTPipa) {
if(plist->head == NULL && tplist->head == NULL){
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
    if(plist->head == NULL || tplist->head == NULL){
        printf("List kosong, tidak bisa dihapus.\n");
        return;
    }

    // Hapus node dari plist
    address tempPipa = plist->head;
    plist->head = plist->head->next;
    if(plist->head == NULL) {
        plist->tail = NULL;
    }
    free(tempPipa);

    // Hapus node dari tplist
    address tempTPipa = tplist->head;
    tplist->head = tplist->head->next;
    if(tplist->head == NULL) {
        tplist->tail = NULL;
    }
    free(tempTPipa);
}

void freeList(){
    // Kosongkan plist
    address currentP = plist->head;
    while(currentP != NULL){
        address temp = currentP;
        currentP = currentP->next;
        free(temp);
    }
    plist->head = plist->tail = NULL;

    // Kosongkan tplist
    address currentT = tplist->head;
    while(currentT != NULL){
        address temp = currentT;
        currentT = currentT->next;
        free(temp);
    }
    tplist->head = tplist->tail = NULL;
}