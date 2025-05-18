#include <stddef.h>
#include "zahra.h"
#include "raylib.h"
#include "bird_struct.h"
#include "pipa_ll.h"

#define PIPA_WIDTH 52  // Contoh lebar pipa, sesuaikan dengan gambar pipamu

GameOverState UpdateGameCollision(Bird bird, Singlelinkedlist *plist, Singlelinkedlist *tplist) {
    address p = plist->head;    // linked list pipa bawah
    address t = tplist->head;   // linked list pipa atas

    // Ukuran bird sudah di-scale
    float birdWidth = bird.texture.width * bird.scale;
    float birdHeight = bird.texture.height * bird.scale;

    Rectangle birdRect = { bird.position.x, bird.position.y, birdWidth, birdHeight };

    while (p != NULL && t != NULL) {
        // Posisi dan ukuran pipa bawah
        Rectangle pipaRect;
        pipaRect.x = p->korx;
        pipaRect.width = PIPA_WIDTH;
        pipaRect.height = p->tinggi;
        pipaRect.y = TINGGI_LAYAR - p->tinggi;  // pipa bawah di dasar layar

        // Posisi dan ukuran pipa atas
        Rectangle tutupPipaRect;
        tutupPipaRect.x = t->korx;
        tutupPipaRect.width = PIPA_WIDTH;
        tutupPipaRect.height = t->tinggi;
        tutupPipaRect.y = 0;  // pipa atas di atas layar

        if (CheckCollisionRecs(birdRect, pipaRect) || CheckCollisionRecs(birdRect, tutupPipaRect)) {
            return GAME_OVER;
        }

        p = p->next;
        t = t->next;
    }

    // Cek apakah bird kena batas atas atau bawah layar
    if (bird.position.y >= TINGGI_LAYAR - birdHeight || bird.position.y <= 0) {
        return GAME_OVER;
    }

    return GAME_ACTIVE;
}
