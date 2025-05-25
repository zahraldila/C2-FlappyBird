#include "zakky.h"
#include "dava.h" // Untuk BackgroundSelector dan fungsi Next/PreviousBackground
#include <stdlib.h>  // Untuk exit() jika masih dipakai

// Variabel untuk tombol kembali (bisa dibuat lebih dinamis)
static Rectangle backButtonRec = { 20, 20, 150, 40 }; // Posisi kiri atas

// Implementasi fungsi menu utama yang sudah ada
void setupMenu()
{
    // GameState state = MENU; // Tidak perlu variabel state lokal di sini

    DrawText("Flappy Bird", LEBAR_LAYAR / 2 - MeasureText("Flappy Bird", 60) / 2, 80, 60, YELLOW); // Tengahkan judul

    // Gambar tombol "START"
    Rectangle startButton = { LEBAR_LAYAR / 2 - 100, 200, 200, 50 };
    DrawRectangleRec(startButton, BLUE);
    DrawText("START", startButton.x + startButton.width / 2 - MeasureText("START", 20) / 2, startButton.y + 15, 20, WHITE);

    // Gambar tombol "CHOOSE BACKGROUND"
    Rectangle bckButton = { LEBAR_LAYAR / 2 - 100, 270, 200, 50 }; // Di bawah start
    DrawRectangleRec(bckButton, BLUE);
    DrawText("BACKGROUND", bckButton.x + bckButton.width/2 - MeasureText("BACKGROUND",20)/2, bckButton.y + 15, 20, WHITE);

    // Gambar tombol "LEADERBOARD" (placeholder)
    Rectangle scoreButton = { LEBAR_LAYAR / 2 - 100, 340, 200, 50 }; // Di bawah background
    DrawRectangleRec(scoreButton, SKYBLUE); // Warna beda untuk placeholder
    DrawText("LEADERBOARD", scoreButton.x + scoreButton.width/2 - MeasureText("LEADERBOARD",20)/2, scoreButton.y + 15, 20, WHITE);

    // Gambar tombol "EXIT"
    Rectangle exitButton = { LEBAR_LAYAR / 2 - 100, 410, 200, 50 }; // Di bawah leaderboard
    DrawRectangleRec(exitButton, RED);
    DrawText("EXIT", exitButton.x + exitButton.width / 2 - MeasureText("EXIT", 20) / 2, exitButton.y + 15, 20, WHITE);

    // Tombol SKIN dan CREDIT bisa ditambahkan dengan pola yang sama jika diperlukan
}

GameState pilihMenu()
{
    GameState nextState = MENU; // Default kembali ke MENU (atau state saat ini)

    Vector2 mousePoint = GetMousePosition();

    Rectangle startButton = { LEBAR_LAYAR / 2 - 100, 200, 200, 50 };
    Rectangle bckButton = { LEBAR_LAYAR / 2 - 100, 270, 200, 50 };
    Rectangle scoreButton = { LEBAR_LAYAR / 2 - 100, 340, 200, 50 }; // Placeholder
    Rectangle exitButton = { LEBAR_LAYAR / 2 - 100, 410, 200, 50 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, startButton))
        {
            nextState = GAMEPLAY;
        }
        if (CheckCollisionPointRec(mousePoint, bckButton))
        {
            nextState = BACKGROUND_SELECTION; // Arahkan ke state baru
        }
        if (CheckCollisionPointRec(mousePoint, scoreButton ))
        {
            // nextState = LEADERBOARD; // Jika sudah ada
            TraceLog(LOG_INFO, "Tombol Leaderboard ditekan (belum ada fungsionalitas)");
        }
        if (CheckCollisionPointRec(mousePoint, exitButton))
        {
            // `shouldCloseGame` di main.c akan lebih baik, tapi ini cara cepat
            // Untuk integrasi yang lebih baik, DrawMenu bisa mengembalikan status khusus untuk keluar
            TraceLog(LOG_INFO, "Tombol EXIT ditekan. Menutup window.");
            CloseWindow(); // Ini akan membuat WindowShouldClose() true di main.c
            // exit(0); // Sebaiknya dihindari jika bisa, biarkan main loop yang keluar
        }
    }
    return nextState;
}

GameState DrawMenu()
{
    setupMenu();
    return pilihMenu();
}

// Fungsi Pause (sudah ada)
void jedapause (PauseState *tmblPause)
{
    tmblPause->isPause = false;
}

void tombolpause (PauseState *tmblPause)
{
    tmblPause->isPause = !tmblPause->isPause;
}

void DrawPauseScreen(PauseState *tmblPause)
{
    if (tmblPause->isPause)
    {
        // Gambar semi-transparan overlay
        DrawRectangle(0,0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        const char* pauseText = "GAME PAUSED";
        const char* resumeText = "Tekan P untuk Melanjutkan";
        DrawText(pauseText, GetScreenWidth()/2 - MeasureText(pauseText, 40)/2, GetScreenHeight()/2 - 40, 40, RAYWHITE);
        DrawText(resumeText, GetScreenWidth()/2 - MeasureText(resumeText, 20)/2, GetScreenHeight()/2 + 10, 20, RAYWHITE);
    }
}


// --- Implementasi untuk Layar Pemilihan Background ---
void InitBackgroundSelectionScreen() {
    // Bisa digunakan untuk memuat resource khusus layar ini jika ada,
    // atau mengatur variabel awal. Untuk sekarang mungkin tidak perlu.
    TraceLog(LOG_INFO, "Memasuki layar pemilihan background.");
}

GameState UpdateBackgroundSelectionScreen(BackgroundSelector *bgSelector, GameState currentGameState) {
    GameState nextState = currentGameState; // Default tetap di layar ini

    if (IsKeyPressed(KEY_RIGHT)) {
        NextBackground(bgSelector); // Fungsi dari dava.h/background_selector.c
    }
    if (IsKeyPressed(KEY_LEFT)) {
        PreviousBackground(bgSelector); // Fungsi dari dava.h/background_selector.c
    }
    if (IsKeyPressed(KEY_ENTER)) {
        // Background sudah terpilih secara otomatis saat Next/Previous dipanggil
        // Jadi, ENTER hanya kembali ke MENU
        TraceLog(LOG_INFO, "Background dipilih: %s", bgSelector->current->filePath);
        nextState = MENU;
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        nextState = MENU;
    }

    // Cek tombol "Kembali" dengan mouse
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), backButtonRec)) {
            nextState = MENU;
        }
    }
    return nextState;
}

void DrawBackgroundSelectionScreen(BackgroundSelector *bgSelector) {
    // Gambar background yang sedang di-preview (tidak bergerak)
    if (bgSelector && bgSelector->current && bgSelector->current->texture.id != 0) {
        DrawTexture(bgSelector->current->texture, 0, 0, WHITE);
    } else {
        DrawText("Gagal memuat preview background", 50, GetScreenHeight()/2 - 20, 20, RED);
    }

    // Instruksi
    const char *titleText = "PILIH BACKGROUND";
    DrawText(titleText, GetScreenWidth()/2 - MeasureText(titleText, 40)/2, 30, 40, YELLOW);

    DrawRectangle(0, GetScreenHeight() - 70, GetScreenWidth(), 70, Fade(BLACK, 0.5f)); // Panel bawah untuk instruksi
    DrawText("KIRI / KANAN : Ganti", 50, GetScreenHeight() - 55, 20, RAYWHITE);
    DrawText("ENTER : Pilih & Kembali", GetScreenWidth()/2 - MeasureText("ENTER : Pilih & Kembali", 20)/2, GetScreenHeight() - 55, 20, RAYWHITE);
    DrawText("BACKSPACE : Kembali", GetScreenWidth() - 50 - MeasureText("BACKSPACE : Kembali", 20), GetScreenHeight() - 55, 20, RAYWHITE);

    // Tombol "Kembali"
    DrawRectangleRec(backButtonRec, Fade(DARKBLUE, 0.7f));
    DrawText("KEMBALI", backButtonRec.x + backButtonRec.width/2 - MeasureText("KEMBALI", 20)/2, backButtonRec.y + 10, 20, WHITE);
}