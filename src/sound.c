#include "sound.h"
#include "raylib.h" // Untuk TraceLog jika diperlukan

static Sound soundEffects[5]; // Array untuk menyimpan efek suara
static Music menuMusic;       // Musik menu tetap static di sini

void InitSounds() {
    // Memuat efek suara
    soundEffects[SOUND_FLAP] = LoadSound("assets/sound/sound_sayap.mp3");
    soundEffects[SOUND_SCORE] = LoadSound("assets/sound/sound_skor.wav");
    soundEffects[SOUND_COLLIDE] = LoadSound("assets/sound/sound_tabrakan.wav");
    soundEffects[SOUND_HIT_GROUND] = LoadSound("assets/sound/sound_jatuh.wav");
    soundEffects[SOUND_GAME_OVER] = LoadSound("assets/sound/sound_gameover.mp3");

    // Memuat musik menu
    menuMusic = LoadMusicStream("assets/sound/sound_menu.wav");
    if (menuMusic.frameCount == 0) { // Cek jika gagal memuat
        TraceLog(LOG_WARNING, "SOUND: Gagal memuat menuMusic dari 'assets/sound/sound_menu.wav'");
    }
    // PlayMusicStream(menuMusic); // Dihapus dari sini, akan di-play dari main.c
}

void PlaySoundEffect(int soundID) {
    if (IsAudioDeviceReady()) { // Hanya mainkan jika audio device siap
        if (soundID >= 0 && soundID < (sizeof(soundEffects)/sizeof(soundEffects[0]))) {
            if (soundEffects[soundID].frameCount > 0) { // Cek apakah sound dimuat
                 PlaySound(soundEffects[soundID]);
            } else {
                // TraceLog(LOG_WARNING, "SOUND: Mencoba memainkan SoundEffect ID %d yang belum dimuat atau gagal dimuat.", soundID);
            }
        } else {
            // TraceLog(LOG_WARNING, "SOUND: SoundEffect ID %d di luar jangkauan.", soundID);
        }
    }
}

void PlayMenuMusic() {
    if (IsAudioDeviceReady() && menuMusic.frameCount > 0) {
        SetMusicVolume(menuMusic, 0.5f); // Atur volume (contoh)
        PlayMusicStream(menuMusic);
    }
}

void StopMenuMusic() {
    if (IsAudioDeviceReady() && menuMusic.frameCount > 0) {
        StopMusicStream(menuMusic);
    }
}

void UpdateMusic() {
    if (IsAudioDeviceReady() && menuMusic.frameCount > 0) { // Tidak perlu cek IsMusicStreamPlaying di sini, UpdateMusicStream menangani
        UpdateMusicStream(menuMusic); // Update buffer musik jika sedang streaming
    }
}

void UnloadSounds() {
    for (int i = 0; i < (sizeof(soundEffects)/sizeof(soundEffects[0])); i++) {
        if (soundEffects[i].frameCount > 0) UnloadSound(soundEffects[i]);
    }
    if (menuMusic.frameCount > 0) UnloadMusicStream(menuMusic);
}

// Implementasi fungsi BARU
bool IsMenuMusicCurrentlyPlaying(void) {
    if (IsAudioDeviceReady() && menuMusic.frameCount > 0) {
        return IsMusicStreamPlaying(menuMusic);
    }
    return false; // Jika audio tidak siap atau musik tidak dimuat, anggap tidak bermain
}