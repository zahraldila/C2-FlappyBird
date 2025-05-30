#include <stddef.h>
#include "zahra.h"
#include "raylib.h"
#include "bird_struct.h"
#include "pipa_ll.h"    
#include "Alexandrio.h"
#include "sound.h"

GameOverState UpdateGameCollision(Bird bird, Singlelinkedlist *collision_plist, Singlelinkedlist *collision_tplist) {
    // Jika tidak ada pipa, tidak ada tabrakan pipa, burung masih bisa jatuh ke tanah atau terbang ke atas.
    // if (!collision_plist || !collision_plist->head) return GAME_ACTIVE;
    address p = NULL;
    if (collision_plist != NULL) {
        p = collision_plist->head;
    }

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
            PlaySoundEffect(SOUND_COLLIDE); 
            return GAME_OVER;
        }
        p = p->next;
    }

    // Cek apakah bird kena BATAS BAWAH layar (tanah)
    if (bird.position.y + bird.collisionHeight >= TINGGI_LAYAR - 30) {
        PlaySoundEffect(SOUND_HIT_GROUND);
        return GAME_OVER;
    }
    return GAME_ACTIVE;
}