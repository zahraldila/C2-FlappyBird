#include "dava.h"
#include <stdlib.h>

// Array path (jalur file) ke semua gambar background yang tersedia.
const char* backgroundPaths[] = {
    "assets/background/nature_1.png", // Path ke background alam 1.
    "assets/background/nature_2.png", // Path ke background alam 2.
    "assets/background/nature_3.png", // Path ke background alam 3.
    "assets/background/nature_4.png", // Path ke background alam 4.
    "assets/background/nature_5.png"  // Path ke background alam 5.
};

#define BACKGROUND_SPEED 0.1f // Kecepatan default untuk scrolling background.

// Inisialisasi BackgroundSelector.
BackgroundSelector* InitBackgroundSelector() {
    // Alokasi memori untuk struktur BackgroundSelector.
    BackgroundSelector* selector = (BackgroundSelector*)malloc(sizeof(BackgroundSelector));
    selector->head = NULL;     // Inisialisasi pointer head ke NULL (awal linked list kosong).
    selector->current = NULL;  // Inisialisasi pointer current ke NULL.
    selector->total = 0;       // Inisialisasi jumlah total background ke 0.

    BackgroundNode* prev = NULL; // Pointer untuk menyimpan node sebelumnya dalam pembuatan linked list.

    // Loop untuk membuat node untuk setiap path background.
    for (int i = 0; i < 5; i++) {                                               // Angka 5 sesuai dengan jumlah path di backgroundPaths.
        BackgroundNode* node = (BackgroundNode*)malloc(sizeof(BackgroundNode)); // Alokasi memori untuk node baru.
        node->filePath = backgroundPaths[i];                                    // Set path file untuk node ini.
        node->texture = LoadTexture(node->filePath);                            // Muat tekstur dari path file.
        node->next = NULL;                                                      // Inisialisasi pointer next ke NULL.
        node->prev = NULL;                                                      // Inisialisasi pointer prev ke NULL.

        if (selector->head == NULL) { // Jika ini adalah node pertama.
            selector->head = node;    // Set node ini sebagai head.
            selector->current = node; // Dan juga sebagai current (background aktif awal).
        } else {                      // Jika bukan node pertama.
            prev->next = node;        // Sambungkan node sebelumnya ke node ini.
            node->prev = prev;        // Sambungkan node ini ke node sebelumnya (doubly linked).
        }

        prev = node;             // Simpan node ini sebagai 'prev' untuk iterasi berikutnya.
        selector->total++;       // Tambah jumlah total background.
    }

    // Membuat linked list menjadi sirkular (circular doubly linked list).
    if (selector->head && prev) {    // Pastikan head dan prev tidak NULL.
        selector->head->prev = prev; // Sambungkan head ke node terakhir (prev).
        prev->next = selector->head; // Sambungkan node terakhir (prev) ke head.
    }

    return selector; // Kembalikan pointer ke BackgroundSelector yang sudah diinisialisasi.
}

// Menggambar background yang sedang dipilih (tampilan statis, tidak scrolling).
void DrawSelectedBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {                      // Pastikan selector dan current valid.
        DrawTexture(selector->current->texture, 0, 0, WHITE); // Gambar tekstur background saat ini di posisi (0,0).
    }
}

// Menggambar background yang dipilih dengan efek scrolling berulang.
void LoopDrawSelectedBackground(BackgroundSelector* selector, float* bgX) {
    if (!selector || !selector->current || selector->current->texture.id == 0) return; // Cek validitas, termasuk ID tekstur.

    DrawTexture(selector->current->texture, *bgX, 0, WHITE);                                    // Gambar bagian pertama background.
    DrawTexture(selector->current->texture, *bgX + selector->current->texture.width, 0, WHITE); // Gambar bagian kedua untuk efek loop, menggunakan lebar tekstur.
}

// Memilih background berikutnya.
void NextBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {             // Pastikan selector dan current valid.
        selector->current = selector->current->next; // Pindah pointer current ke node berikutnya.
    }
}

// Memilih background sebelumnya.
void PreviousBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {             // Pastikan selector dan current valid.
        selector->current = selector->current->prev; // Pindah pointer current ke node sebelumnya.
    }
}

// Membebaskan memori yang digunakan oleh BackgroundSelector.
void UnloadBackgroundSelector(BackgroundSelector* selector) {
    if (!selector || !selector->head) return; // Jika tidak ada selector atau head, tidak ada yang di-unload.

    BackgroundNode* temp = selector->head;      // Mulai dari head.
    BackgroundNode* firstNode = selector->head; // Simpan node pertama untuk menghentikan loop

    if (temp) { // Pastikan temp (head) tidak NULL
        do {
            BackgroundNode* nextNode = temp->next;    // Simpan pointer ke node berikutnya.
            UnloadTexture(temp->texture);             // Unload tekstur dari VRAM.
            free(temp);                               // Bebaskan memori node.
            temp = nextNode;                          // Pindah ke node berikutnya.
        } while (temp != firstNode && temp != NULL);  // Lanjutkan sampai kembali ke head atau jika list rusak (temp jadi NULL)
    }


    free(selector); // Bebaskan memori untuk struktur BackgroundSelector itu sendiri.
}