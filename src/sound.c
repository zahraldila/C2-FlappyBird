#include "sound.h"

static Sound soundEffects[5];
static Music menuMusic;

void InitSounds() {
    soundEffects[SOUND_FLAP] = LoadSound("assets/sound/hu2u1-101soundboards.mp3");
    soundEffects[SOUND_SCORE] = LoadSound("assets/sound/sound_skor.wav");
    soundEffects[SOUND_COLLIDE] = LoadSound("assets/sound/sound_tabrakan.wav");
    soundEffects[SOUND_HIT_GROUND] = LoadSound("assets/sound/sound_jatuh.wav");
    soundEffects[SOUND_GAME_OVER] = LoadSound("assets/sound/game-over-101soundboards.mp3"); // Pakai yang sama

    menuMusic = LoadMusicStream("assets/sound/sound_menu.wav");
    PlayMusicStream(menuMusic);
}

void PlaySoundEffect(int soundID) {
    if (soundID >= 0 && soundID <= 4) {
        PlaySound(soundEffects[soundID]);
    }
}

void PlayMenuMusic() {
    PlayMusicStream(menuMusic);
}

void StopMenuMusic() {
    StopMusicStream(menuMusic);
}

void UpdateMusic() {
    UpdateMusicStream(menuMusic);
}

void UnloadSounds() {
    for (int i = 0; i < 5; i++) {
        UnloadSound(soundEffects[i]);
    }
    UnloadMusicStream(menuMusic);
}
