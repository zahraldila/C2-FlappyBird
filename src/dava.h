#ifndef DAVA_H
#define DAVA_H

#include "raylib.h"
#include "bird_struct.h"

#define MAX_BIRDS 1
#define GRAVITY 1.4f
#define FLAP_STRENGTH -14.0f

// Deklarasi fungsi
void InitBirds(Bird birds[], int count);
void UpdateBirds(Bird birds[], int count);
void DrawBirds(Bird birds[], int count);
void UnloadBirds(Bird birds[], int count);

#endif
