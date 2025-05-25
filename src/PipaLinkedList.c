#include "Alexandrio.h" // Untuk konstanta
#include "pipa_ll.h"
#include <stdlib.h>
#include <stdio.h>    // Untuk TraceLog via raylib atau printf standar

// Definisi variabel global plist dan tplist
Singlelinkedlist *plist = NULL;
Singlelinkedlist *tplist = NULL;

void initList(Singlelinkedlist *L){
    if (L == NULL) {
        // TraceLog(LOG_WARNING, "PIPA_LL: Mencoba initList pada List NULL.");
        return;
    }
    L->head = NULL;
    L->tail = NULL;
}

address buatNodePipaGenerik(int tinggi_pipa_atas){
    address newNode = (address)malloc(sizeof(Node));
    if (newNode == NULL) {
        TraceLog(LOG_ERROR, "PIPA_LL: Gagal alokasi memori untuk Node Pipa!");
        return NULL;
    }
    newNode->tinggi = tinggi_pipa_atas; // korx diisi nanti
    newNode->status = 0;
    newNode->next = NULL;
    return newNode;
}

// Fungsi internal untuk satu list
void insertBelakang(Singlelinkedlist *list, address newNode) {
    if (list == NULL || newNode == NULL) return;
    if(list->head == NULL){
        list->head = list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

void deleteFirst(Singlelinkedlist *list){
    if (list == NULL || list->head == NULL) return;
    address tempNode = list->head;
    list->head = list->head->next;
    if(list->head == NULL) {
        list->tail = NULL;
    }
    free(tempNode);
}

void freeList(Singlelinkedlist *list){
    if (list == NULL) return;
    address current = list->head;
    while(current != NULL){
        address temp = current;
        current = current->next;
        free(temp);
    }
    list->head = list->tail = NULL;
}

// Fungsi untuk mengelola plist dan tplist global secara bersamaan
void insertBelakangPasangan(address newNodePipa, address newNodeTPipa) {
    if (newNodePipa != NULL && plist != NULL) {
        insertBelakang(plist, newNodePipa);
    } else if (newNodePipa == NULL && plist != NULL) {
        // TraceLog(LOG_WARNING, "PIPA_LL: newNodePipa adalah NULL saat insertBelakangPasangan");
    }

    if (newNodeTPipa != NULL && tplist != NULL) {
        insertBelakang(tplist, newNodeTPipa);
    } else if (newNodeTPipa == NULL && tplist != NULL) {
        // TraceLog(LOG_WARNING, "PIPA_LL: newNodeTPipa adalah NULL saat insertBelakangPasangan");
    }
}

void deleteFirstPasangan(){
    if (plist != NULL) deleteFirst(plist);
    if (tplist != NULL) deleteFirst(tplist);
}

void freeListPasangan(){
    if (plist != NULL) freeList(plist);
    if (tplist != NULL) freeList(tplist);
    // TraceLog(LOG_DEBUG, "PIPA_LL: freeListPasangan dipanggil, plist dan tplist dibersihkan.");
}