#include "leaderboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h" // Pastikan raylib.h disertakan untuk fungsi-fungsi drawing
#include "Alexandrio.h" // Untuk LEBAR_LAYAR/SCREEN_WIDTH

LeaderboardList *g_leaderboardList = NULL;
const char* LEADERBOARD_FILE_PATH = "leaderboard_data.txt";

// Inisialisasi doubly linked list leaderboard
void InitLeaderboard(void) {
    g_leaderboardList = (LeaderboardList *)malloc(sizeof(LeaderboardList));
    if (g_leaderboardList == NULL) {
        TraceLog(LOG_FATAL, "LEADERBOARD: Gagal mengalokasikan memori untuk LeaderboardList.");
        return;
    }
    g_leaderboardList->head = NULL;
    g_leaderboardList->tail = NULL;
    g_leaderboardList->count = 0;
    LoadLeaderboard(); // Muat data yang sudah ada
}

// Membebaskan memori yang digunakan oleh leaderboard
void FreeLeaderboard(void) {
    if (g_leaderboardList == NULL) return;

    LeaderboardNode *current = g_leaderboardList->head;
    while (current != NULL) {
        LeaderboardNode *next = current->next;
        free(current);
        current = next;
    }
    free(g_leaderboardList);
    g_leaderboardList = NULL;
}

// Muat leaderboard dari file
void LoadLeaderboard(void) {
    if (g_leaderboardList == NULL) InitLeaderboard(); // Pastikan list sudah diinisialisasi
    else { // Kosongkan list yang ada sebelum memuat yang baru
        LeaderboardNode *current = g_leaderboardList->head;
        while (current != NULL) {
            LeaderboardNode *next = current->next;
            free(current);
            current = next;
        }
        g_leaderboardList->head = NULL;
        g_leaderboardList->tail = NULL;
        g_leaderboardList->count = 0;
    }

    FILE *file = fopen(LEADERBOARD_FILE_PATH, "r");
    if (file != NULL) {
        LeaderboardEntry entry;
        while (fscanf(file, "%s %d", entry.name, &entry.scoreValue) == 2) {
            // Untuk memuat, kita langsung sisipkan saja dan biarkan AddScoreToLeaderboard yang mengurutkan
            // Batasi jumlah entri yang dimuat agar tidak melebihi MAX_LEADERBOARD_ENTRIES
            if (g_leaderboardList->count < MAX_LEADERBOARD_ENTRIES) {
                AddScoreToLeaderboard(entry.name, entry.scoreValue);
            }
        }
        fclose(file);
        // TraceLog(LOG_INFO, "LEADERBOARD: Leaderboard dimuat dari %s. Total entri: %d", LEADERBOARD_FILE_PATH, g_leaderboardList->count);
    } else {
        TraceLog(LOG_INFO, "LEADERBOARD: File %s tidak ditemukan, memulai leaderboard kosong.", LEADERBOARD_FILE_PATH);
        // Inisialisasi entri kosong jika file tidak ada
        for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) {
            AddScoreToLeaderboard("---", 0); // Tambahkan entri dummy agar selalu ada MAX_LEADERBOARD_ENTRIES
        }
    }
}

// Simpan leaderboard ke file
void SaveLeaderboard(void) {
    if (g_leaderboardList == NULL) return;

    FILE *file = fopen(LEADERBOARD_FILE_PATH, "w");
    if (file != NULL) {
        LeaderboardNode *current = g_leaderboardList->head;
        int savedCount = 0;
        while (current != NULL && savedCount < MAX_LEADERBOARD_ENTRIES) {
            if (current->data.scoreValue > 0 || strcmp(current->data.name, "---") != 0) { // Hanya simpan entri yang valid
                fprintf(file, "%s %d\n", current->data.name, current->data.scoreValue);
                savedCount++;
            }
            current = current->next;
        }
        fclose(file);
        TraceLog(LOG_INFO, "LEADERBOARD: Leaderboard disimpan ke %s. Total entri: %d", LEADERBOARD_FILE_PATH, savedCount);
    } else {
        TraceLog(LOG_ERROR, "LEADERBOARD: Gagal menyimpan leaderboard ke %s", LEADERBOARD_FILE_PATH);
    }
}

// Tambahkan skor baru ke leaderboard (disisipkan secara terurut)
void AddScoreToLeaderboard(const char* playerName, int newScore) {
    if (g_leaderboardList == NULL) {
        InitLeaderboard(); // Pastikan list sudah diinisialisasi
        if (g_leaderboardList == NULL) return; // Gagal inisialisasi
    }

    LeaderboardNode *newNode = (LeaderboardNode *)malloc(sizeof(LeaderboardNode));
    if (newNode == NULL) {
        TraceLog(LOG_ERROR, "LEADERBOARD: Gagal mengalokasikan memori untuk LeaderboardNode.");
        return;
    }

    strncpy(newNode->data.name, playerName, MAX_PLAYER_NAME_LENGTH);
    newNode->data.name[MAX_PLAYER_NAME_LENGTH] = '\0'; // Pastikan null-terminated
    newNode->data.scoreValue = newScore;
    newNode->next = NULL;
    newNode->prev = NULL;

    // Sisipkan node baru secara terurut (dari skor tertinggi ke terendah)
    if (g_leaderboardList->head == NULL || newScore >= g_leaderboardList->head->data.scoreValue) {
        // Sisipkan di awal (atau jika list kosong)
        newNode->next = g_leaderboardList->head;
        if (g_leaderboardList->head != NULL) {
            g_leaderboardList->head->prev = newNode;
        } else {
            g_leaderboardList->tail = newNode; // Jika list kosong, newNode juga jadi tail
        }
        g_leaderboardList->head = newNode;
    } else {
        // Cari posisi yang tepat
        LeaderboardNode *current = g_leaderboardList->head;
        while (current->next != NULL && newScore < current->next->data.scoreValue) {
            current = current->next;
        }
        // Sisipkan di antara current dan current->next
        newNode->next = current->next;
        newNode->prev = current;
        if (current->next != NULL) {
            current->next->prev = newNode;
        } else {
            g_leaderboardList->tail = newNode; // Jika sisip di akhir, newNode jadi tail
        }
        current->next = newNode;
    }

    g_leaderboardList->count++;

    // Jika jumlah entri melebihi MAX_LEADERBOARD_ENTRIES, hapus yang terakhir (skor terendah)
    while (g_leaderboardList->count > MAX_LEADERBOARD_ENTRIES) {
        if (g_leaderboardList->tail != NULL) {
            LeaderboardNode *toDelete = g_leaderboardList->tail;
            g_leaderboardList->tail = g_leaderboardList->tail->prev;
            if (g_leaderboardList->tail != NULL) {
                g_leaderboardList->tail->next = NULL;
            } else {
                g_leaderboardList->head = NULL; // List kosong
            }
            free(toDelete);
            g_leaderboardList->count--;
        }
    }
}

// Cek apakah skor baru cukup tinggi untuk masuk leaderboard
bool IsScoreHighEnough(int newScore) {
    if (g_leaderboardList == NULL) {
        // Jika leaderboard belum diinisialisasi, anggap skor cukup tinggi untuk masuk
        // (ini akan memicu InitLeaderboard dan mengisi dengan data awal)
        return true;
    }

    // Jika list belum penuh
    if (g_leaderboardList->count < MAX_LEADERBOARD_ENTRIES) {
        return true;
    }

    // Jika list sudah penuh, cek apakah skor baru lebih tinggi dari skor terendah
    if (g_leaderboardList->tail != NULL && newScore > g_leaderboardList->tail->data.scoreValue) {
        return true;
    }

    return false;
}

// Gambar konten leaderboard
void DrawLeaderboardContent(Font font) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char* title = "LEADERBOARD";
    DrawTextEx(font, title, (Vector2){ (float)screenWidth/2.0f - MeasureTextEx(font, title, 40, 2).x/2.0f, 30 }, 40, 2, YELLOW); // Judul naik menjadi Y=30

    float fontSizeEntry = 15; // Ukuran font untuk entri
    float spacingEntry = 3;  // Spacing antara karakter dalam teks
    float rowHeight = fontSizeEntry + 15; // Tinggi baris untuk setiap entri, ditambah padding
    float boxPadding = 5; // Padding di dalam kotak

    // Menghitung lebar tetap untuk setiap kotak berdasarkan teks terpanjang yang diharapkan
    // MAX_PLAYER_NAME_LENGTH adalah 6
    float rankBoxDesiredWidth = MeasureTextEx(font, "10.", fontSizeEntry, spacingEntry).x + boxPadding * 2;
    float nameBoxDesiredWidth = MeasureTextEx(font, "WWWWWW", fontSizeEntry, spacingEntry).x + boxPadding * 2;
    float scoreBoxDesiredWidth = MeasureTextEx(font, "9999", fontSizeEntry, spacingEntry).x + boxPadding * 2; // Asumsi skor max 4 digit

    // Menghitung total lebar semua kotak dan spasi antar kotak
    float gapBetweenBoxes = 25; // Spasi antar kotak peringkat-nama dan nama-skor
    float totalBoxesWidth = rankBoxDesiredWidth + gapBetweenBoxes + nameBoxDesiredWidth + gapBetweenBoxes + scoreBoxDesiredWidth;

    // Menghitung posisi X awal agar seluruh blok kotak berada di tengah layar
    float startX = (screenWidth / 2.0f) - (totalBoxesWidth / 2.0f);

    LeaderboardNode *current = g_leaderboardList->head;
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++) { // Mengulang sebanyak MAX_LEADERBOARD_ENTRIES
        char rankText[8];
        char nameText[MAX_PLAYER_NAME_LENGTH + 1];
        char scoreText[16];

        if (current != NULL) {
            sprintf(rankText, "%d.", i + 1);
            strncpy(nameText, current->data.name, MAX_PLAYER_NAME_LENGTH);
            nameText[MAX_PLAYER_NAME_LENGTH] = '\0'; // Pastikan null-terminated
            sprintf(scoreText, "%d", current->data.scoreValue);
            current = current->next;
        } else {
            sprintf(rankText, "%d.", i + 1);
            strcpy(nameText, "---");
            strcpy(scoreText, "---");
        }

        Color textColor = (strcmp(nameText, "---") == 0 && strcmp(scoreText, "---") == 0) ? WHITE : RAYWHITE;

        // Posisi Y untuk baris saat ini (dimulai lebih tinggi)
        float currentY = 90.0f + (i * rowHeight);

        // --- Gambar Kotak dan Teks untuk Peringkat ---
        Rectangle rankBoxRec = { startX, currentY, rankBoxDesiredWidth, rowHeight - 5}; // Kotak peringkat
        DrawRectangleRec(rankBoxRec, Fade(DARKBLUE, 0.7f));
        DrawRectangleLines((int)rankBoxRec.x, (int)rankBoxRec.y, (int)rankBoxRec.width, (int)rankBoxRec.height, WHITE);
        // Gambar nomor peringkat (di tengah kotak)
        Vector2 rankTextSize = MeasureTextEx(font, rankText, fontSizeEntry, spacingEntry);
        DrawTextEx(font, rankText, (Vector2){ rankBoxRec.x + rankBoxRec.width/2 - rankTextSize.x/2, rankBoxRec.y + rankBoxRec.height/2 - rankTextSize.y/2 }, fontSizeEntry, spacingEntry, textColor);

        // --- Gambar Kotak dan Teks untuk Nama ---
        Rectangle nameBoxRec = { startX + rankBoxDesiredWidth + gapBetweenBoxes, currentY, nameBoxDesiredWidth, rowHeight - 5 }; // Kotak nama
        DrawRectangleRec(nameBoxRec, Fade(DARKBLUE, 0.7f));
        DrawRectangleLines((int)nameBoxRec.x, (int)nameBoxRec.y, (int)nameBoxRec.width, (int)nameBoxRec.height, WHITE);
        // Gambar nama (di tengah kotak)
        Vector2 nameTextSize = MeasureTextEx(font, nameText, fontSizeEntry, spacingEntry);
        DrawTextEx(font, nameText, (Vector2){ nameBoxRec.x + nameBoxRec.width/2 - nameTextSize.x/2, nameBoxRec.y + nameBoxRec.height/2 - nameTextSize.y/2 }, fontSizeEntry, spacingEntry, textColor);

        // --- Gambar Kotak dan Teks untuk Skor ---
        Rectangle scoreBoxRec = { startX + rankBoxDesiredWidth + gapBetweenBoxes + nameBoxDesiredWidth + gapBetweenBoxes, currentY, scoreBoxDesiredWidth, rowHeight - 5 }; // Kotak skor
        DrawRectangleRec(scoreBoxRec, Fade(DARKBLUE, 0.7f));
        DrawRectangleLines((int)scoreBoxRec.x, (int)scoreBoxRec.y, (int)scoreBoxRec.width, (int)scoreBoxRec.height, WHITE);
        // Gambar skor (di tengah kotak)
        Vector2 scoreTextSize = MeasureTextEx(font, scoreText, fontSizeEntry, spacingEntry);
        DrawTextEx(font, scoreText, (Vector2){ scoreBoxRec.x + scoreBoxRec.width/2 - scoreTextSize.x/2, scoreBoxRec.y + scoreBoxRec.height/2 - scoreTextSize.y/2 }, fontSizeEntry, spacingEntry, textColor);
    }

    const char* backInstruction = "Press BACKSPACE to Return to Menu";
    Vector2 backTextSize = MeasureTextEx(font, backInstruction, 20, 2);
    DrawTextEx(font, backInstruction, (Vector2){ (float)screenWidth/2.0f - backTextSize.x/2.0f, (float)screenHeight - 50.0f }, 20, 2, WHITE);
}