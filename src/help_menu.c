#include "zahra.h"
#include "raylib.h"

void HandleHelpState(GameState *currentState) {
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)) {
        *currentState = MENU; // Gunakan MENU dari GameState
    }
}

void DrawHelpScreen(void) {
    // Gambar background semi-transparan untuk seluruh layar
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    // Judul Bantuan
    const char* title = "CARA BERMAIN FLAPPY BIRD";
    DrawText(title, GetScreenWidth()/2 - MeasureText(title, 40)/2, 50, 40, RAYWHITE);

    const char* instructions[] = {
        "1. Tekan SPACE BAR untuk melompat dan membuat burung terbang.",
        "2. Hindari pipa-pipa yang datang dari kanan.",
        "3. Setiap berhasil melewati sepasang pipa, skor Anda akan bertambah 1.",
        "4. Jangan sampai burung menyentuh tanah atau menabrak pipa.",
        "5. Tekan tombol 'P' saat bermain untuk pause/resume game.",
        "6. Di layar Game Over, tekan ENTER untuk bermain lagi atau BACKSPACE untuk ke Menu.",
        "7. Masukkan nama Anda di awal game untuk dicatat di Leaderboard!"
    };

    // Variabel untuk ukuran teks dan posisi kotak instruksi
    int instructionTextSize = 18; 
    int instructionLineHeight = 25;
    int instructionStartX = GetScreenWidth()/2;
    int instructionStartY = 150;

  
    int maxInstructionWidth = 0;
    for (int i = 0; i < sizeof(instructions)/sizeof(instructions[0]); i++) {
        int currentTextWidth = MeasureText(instructions[i], instructionTextSize);
        if (currentTextWidth > maxInstructionWidth) {
            maxInstructionWidth = currentTextWidth;
        }
    }

    // Hitung tinggi total kotak instruksi
    int totalInstructionsHeight = (sizeof(instructions)/sizeof(instructions[0])) * instructionLineHeight;

    // Tambah padding untuk kotak
    int padding = 15;
    Rectangle instructionBox = {
        instructionStartX - maxInstructionWidth/2 - padding, // X
        instructionStartY - padding,                           // Y
        maxInstructionWidth + (padding * 2),                  // Lebar
        totalInstructionsHeight + (padding * 2)               // Tinggi
    };

    // Gambar kotak background untuk instruksi
    DrawRectangleRec(instructionBox, Fade(BLUE, 0.4f)); // Warna biru semi-transparan
    DrawRectangleLines((int)instructionBox.x, (int)instructionBox.y, (int)instructionBox.width, (int)instructionBox.height, RAYWHITE); // Border kotak

    // Gambar setiap baris instruksi di dalam kotak
    for (int i = 0; i < sizeof(instructions)/sizeof(instructions[0]); i++) {
        // Teks digambar relatif terhadap tengah kotak, dengan ukuran baru
        DrawText(instructions[i], instructionStartX - MeasureText(instructions[i], instructionTextSize)/2,
                 instructionStartY + i * instructionLineHeight, instructionTextSize, RAYWHITE);
    }

    // Teks untuk kembali (posisi tetap atau disesuaikan jika perlu)
    const char* backMsg = "Tekan BACKSPACE untuk Kembali ke Menu";
    DrawText(backMsg, GetScreenWidth()/2 - MeasureText(backMsg, 20)/2, GetScreenHeight() - 70, 20, LIGHTGRAY);
}
