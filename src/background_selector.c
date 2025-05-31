#include "dava.h"
#include <stdlib.h>

// Array path (jalur file) ke semua gambar background yang tersedia.
const char* backgroundPaths[] = {
    "assets/background/nature_1.png",
    "assets/background/nature_2.png",
    "assets/background/nature_3.png",
    "assets/background/nature_4.png",
    "assets/background/nature_5.png"
};

#define BACKGROUND_SPEED 0.1f // Kecepatan untuk scrolling background.

// Inisialisasi BackgroundSelector.
BackgroundSelector* InitBackgroundSelector() {
    BackgroundSelector* selector = (BackgroundSelector*)malloc(sizeof(BackgroundSelector));
    selector->head = NULL;
    selector->current = NULL;
    selector->total = 0;

    BackgroundNode* prev = NULL;

    // Loop untuk membuat node untuk setiap path background.
    for (int i = 0; i < 5; i++) {
        BackgroundNode* node = (BackgroundNode*)malloc(sizeof(BackgroundNode));
        node->filePath = backgroundPaths[i];
        node->texture = LoadTexture(node->filePath);
        node->next = NULL;
        node->prev = NULL;

        if (selector->head == NULL) {
            selector->head = node;
            selector->current = node;
        } else {
            prev->next = node;
            node->prev = prev;
        }

        prev = node;
        selector->total++;
    }

    // Membuat linked list menjadi sirkular (circular doubly linked list).
    if (selector->head && prev) {
        selector->head->prev = prev;
        prev->next = selector->head;
    }

    return selector;
}

// Menggambar background yang sedang dipilih.
void DrawSelectedBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {
        DrawTexture(selector->current->texture, 0, 0, WHITE);
    }
}

// Menggambar background yang dipilih dengan efek scrolling berulang.
void LoopDrawSelectedBackground(BackgroundSelector* selector, float* bgX) {
    if (!selector || !selector->current || selector->current->texture.id == 0) return;

    DrawTexture(selector->current->texture, *bgX, 0, WHITE);
    DrawTexture(selector->current->texture, *bgX + selector->current->texture.width, 0, WHITE);
}

// Memilih background berikutnya.
void NextBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {
        selector->current = selector->current->next;
    }
}

// Memilih background sebelumnya.
void PreviousBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {
        selector->current = selector->current->prev;
    }
}

// Membebaskan memori yang digunakan oleh BackgroundSelector.
void UnloadBackgroundSelector(BackgroundSelector* selector) {
    if (!selector || !selector->head) return;

    BackgroundNode* temp = selector->head;
    BackgroundNode* firstNode = selector->head;

    if (temp) { // Pastikan temp (head) tidak NULL
        do {
            BackgroundNode* nextNode = temp->next;
            UnloadTexture(temp->texture);
            free(temp);
            temp = nextNode;
        } while (temp != firstNode && temp != NULL);
    }


    free(selector);
}