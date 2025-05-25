#include <stddef.h>
#include "zahra.h"      // Untuk GameOverState
#include "raylib.h"
#include "bird_struct.h"
#include "pipa_ll.h"    // Untuk Singlelinkedlist, address
#include "Alexandrio.h" // Untuk TINGGI_LAYAR, LEBAR_PIPA, JARAK_PIPA_ATAS_BAWAH
#include "sound.h"      // Untuk PlaySoundEffect, SOUND_COLLIDE, SOUND_HIT_GROUND

GameOverState UpdateGameCollision(Bird bird, Singlelinkedlist *collision_plist, Singlelinkedlist *collision_tplist) {
    // collision_tplist mungkin tidak diperlukan jika semua data ada di collision_plist
    // Jika tidak ada pipa, tidak ada tabrakan pipa, burung masih bisa jatuh ke tanah atau terbang ke atas.
    // if (!collision_plist || !collision_plist->head) return GAME_ACTIVE;

    address p = NULL;
    if (collision_plist != NULL) {
        p = collision_plist->head;
    }

    // bird.scale diasumsikan 1.0f karena gambar sudah diedit/disesuaikan ukurannya
    Rectangle birdRect = { bird.position.x, bird.position.y, bird.collisionWidth, bird.collisionHeight };

    // Cek tabrakan dengan pipa-pipa
    while (p != NULL) {
        // Bounding Box Pipa ATAS
        Rectangle pipaAtasRect;
        pipaAtasRect.x = (float)p->korx;
        pipaAtasRect.y = 0.0f;
        pipaAtasRect.width = (float)LEBAR_PIPA;
        pipaAtasRect.height = (float)p->tinggi; // p->tinggi adalah tinggi visual pipa atas

        // Bounding Box Pipa BAWAH
        Rectangle pipaBawahRect;
        pipaBawahRect.x = (float)p->korx;
        pipaBawahRect.y = (float)(p->tinggi + JARAK_PIPA_ATAS_BAWAH);
        pipaBawahRect.width = (float)LEBAR_PIPA;
        pipaBawahRect.height = (float)(TINGGI_LAYAR - (p->tinggi + JARAK_PIPA_ATAS_BAWAH));

        // Periksa tabrakan dengan pipa atas atau bawah
        if (CheckCollisionRecs(birdRect, pipaAtasRect) || CheckCollisionRecs(birdRect, pipaBawahRect)) {
            PlaySoundEffect(SOUND_COLLIDE); // Mainkan suara tabrakan pipa
            // PlaySoundEffect(SOUND_GAME_OVER); // Suara game over umum bisa diputar di main.c
            return GAME_OVER;
        }
        p = p->next;
    }

    // Cek apakah bird kena BATAS BAWAH layar (tanah)
    // Tinggi tanah visual sekitar 30 piksel dari bawah
    if (bird.position.y + bird.collisionHeight >= TINGGI_LAYAR - 30) {
        PlaySoundEffect(SOUND_HIT_GROUND); // Mainkan suara jatuh ke tanah
        // PlaySoundEffect(SOUND_GAME_OVER); // Suara game over umum bisa diputar di main.c
        return GAME_OVER;
    }

    // Interaksi dengan batas atas layar (tidak game over) sudah ditangani di bird.c
    // if (bird.position.y <= 0) {
    //    // Tidak ada game over di sini
    // }

    return GAME_ACTIVE;
}