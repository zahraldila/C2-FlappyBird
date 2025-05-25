#ifndef SOUND_H
#define SOUND_H

#include <raylib.h>

// Definisi ID untuk efek suara
#define SOUND_FLAP 0
#define SOUND_SCORE 1
#define SOUND_COLLIDE 2
#define SOUND_HIT_GROUND 3
#define SOUND_GAME_OVER 4

// Deklarasi fungsi yang sudah ada
void InitSounds(void);
void PlaySoundEffect(int soundID);
void PlayMenuMusic(void);
void StopMenuMusic(void);
void UpdateMusic(void); // Untuk UpdateMusicStream
void UnloadSounds(void);

// Deklarasi fungsi BARU untuk mengecek status musik menu
bool IsMenuMusicCurrentlyPlaying(void); // Nama fungsi yang lebih deskriptif

#endif // SOUND_H