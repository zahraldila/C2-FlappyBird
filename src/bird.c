// bird.c
#include "raylib.h"
#include "dava.h"
#include <stdlib.h>
#include <time.h>

float bgX;

// Buat satu node burung dan return head-nya
BirdNode* InitBirdsLinkedList(int count) {
    Image img = LoadImage("Flappy.png");
    ImageResize(&img, img.width / 3, img.height / 3);

    BirdNode *head = NULL;
    BirdNode *tail = NULL;

    for (int i = 0; i < count; i++) {
        BirdNode *newNode = (BirdNode *)malloc(sizeof(BirdNode));
        newNode->bird.texture = LoadTextureFromImage(img);
        newNode->bird.position = (Vector2){0, 200};
        newNode->bird.speed = 0;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    UnloadImage(img);
    return head;
}

void UpdateBirds(BirdNode *head, YNode **yHead) {
    BirdNode *current = head;
    while (current != NULL) {
        current->bird.speed += GRAVITY;

        if (IsKeyPressed(KEY_SPACE)) {
            current->bird.speed = FLAP_STRENGTH;
        }

        float newY = current->bird.position.y + current->bird.speed;
        AddPositionY(yHead, newY);
        UpdateBirdYMovement(&current->bird, yHead);

        if (current->bird.position.y > 385) {
            current->bird.position.y = 385;
            current->bird.speed = 0;
        }

        if (current->bird.position.y < -15) {
            current->bird.position.y = -15;
            current->bird.speed = 0;
        }

        current = current->next;
    }
}

void DrawBirds(BirdNode *head) {
    BirdNode *current = head;
    while (current != NULL) {
        DrawTexture(current->bird.texture, (int)current->bird.position.x, (int)current->bird.position.y, WHITE);
        current = current->next;
    }
}

void UnloadBirds(BirdNode *head) {
    BirdNode *current = head;
    while (current != NULL) {
        UnloadTexture(current->bird.texture);
        BirdNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void InitBackground(Texture2D *bg) {
    *bg = LoadTexture("city.png");
}

void UpdateBackground(float *bgX) {
    *bgX -= 0.5f;
    if (*bgX <= -LEBAR_LAYAR) {
        *bgX = 0;
    }
}

void DrawBackground(Texture2D bg, float bgX) {
    DrawTextureEx(bg, (Vector2){bgX, 0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(bg, (Vector2){bgX + LEBAR_LAYAR, 0}, 0.0f, 1.0f, WHITE);
}

// Linked list posisi Y
void AddPositionY(YNode **head, float y) {
    YNode *newNode = (YNode*)malloc(sizeof(YNode));
    newNode->y = y;
    newNode->prev = NULL;
    newNode->next = *head;

    if (*head != NULL)
        (*head)->prev = newNode;

    *head = newNode;
}

void UpdateBirdYMovement(Bird *bird, YNode **yHead) {
    if (*yHead == NULL) return;

    bird->position.y = (*yHead)->y;

    YNode *temp = *yHead;
    *yHead = (*yHead)->next;

    if (*yHead != NULL)
        (*yHead)->prev = NULL;

    free(temp);
}

void FreeYMovementList(YNode **head) {
    while (*head != NULL) {
        YNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
