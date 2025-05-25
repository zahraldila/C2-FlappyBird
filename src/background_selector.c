#include "dava.h"
#include <stdlib.h>
#include <string.h>

const char* backgroundPaths[] = {
    "assets/background/nature_1.png",
    "assets/background/nature_2.png",
    "assets/background/nature_3.png",
    "assets/background/nature_4.png",
    "assets/background/nature_5.png"
};

#define BACKGROUND_SPEED 0.5f

BackgroundSelector* InitBackgroundSelector() {
    BackgroundSelector* selector = (BackgroundSelector*)malloc(sizeof(BackgroundSelector));
    selector->head = NULL;
    selector->current = NULL;
    selector->total = 0;

    BackgroundNode* prev = NULL;

    for (int i = 0; i < 5; i++) {
        BackgroundNode* node = (BackgroundNode*)malloc(sizeof(BackgroundNode));
        node->filePath = backgroundPaths[i];
        node->texture = LoadTexture(node->filePath);
        node->next = NULL;
        node->prev = NULL;

        if (selector->head == NULL) {
            selector->head = node;
            selector->current = node;
        } else {
            prev->next = node;
            node->prev = prev;
        }

        prev = node;
        selector->total++;
    }

    // Buat circular doubly linked list
    if (selector->head && prev) {
        selector->head->prev = prev;
        prev->next = selector->head;
    }

    return selector;
}

void DrawSelectedBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {
        DrawTexture(selector->current->texture, 0, 0, WHITE);
    }
}

void LoopDrawSelectedBackground(BackgroundSelector* selector, float* bgX) {
    if (!selector || !selector->current) return;

    *bgX -= BACKGROUND_SPEED;
    if (*bgX <= -SCREEN_WIDTH) {
        *bgX = 0;
    }

    DrawTexture(selector->current->texture, *bgX, 0, WHITE);
    DrawTexture(selector->current->texture, *bgX + SCREEN_WIDTH, 0, WHITE);
}

void NextBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {
        selector->current = selector->current->next;
    }
}

void PreviousBackground(BackgroundSelector* selector) {
    if (selector && selector->current) {
        selector->current = selector->current->prev;
    }
}

void UnloadBackgroundSelector(BackgroundSelector* selector) {
    if (!selector || !selector->head) return;

    BackgroundNode* temp = selector->head;
    for (int i = 0; i < selector->total; i++) {
        BackgroundNode* next = temp->next;
        UnloadTexture(temp->texture);
        free(temp);
        temp = next;
    }

    free(selector);
}
