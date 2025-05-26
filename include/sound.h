#ifndef SOUND_H
#define SOUND_H

#include "raylib.h"

#define SOUND_FLAP 0
#define SOUND_SCORE 1
#define SOUND_COLLIDE 2
#define SOUND_HIT_GROUND 3
#define SOUND_GAME_OVER 4

void InitSounds();
void PlaySoundEffect(int soundID);
void PlayMenuMusic();
void StopMenuMusic();
void UpdateMusic();
void UnloadSounds();

#endif
