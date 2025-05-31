#include "Alexandrio.h"
#include "zakky.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

float bgX = 0; 

AwanNode* createAwan(float x, float y) 
{
    AwanNode *nodeBaru = (AwanNode *)malloc(sizeof(AwanNode));
    nodeBaru->x = x;
    nodeBaru->y = y;
    nodeBaru->next = NULL;

    return nodeBaru;
}

void insertAwan(AwanNode **head, float x, float y)
{
    AwanNode *nodeBaru = createAwan(x,y);
    if (*head == NULL)
    {
        *head = nodeBaru;
    }
    else
    {
        AwanNode *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = nodeBaru;
    }
}

void updateAwan(AwanNode *head) 
{
    AwanNode *temp = head;
    while (temp != NULL) 
    {
        temp->x -= KECEPATAN_AWAN;

        if (temp->x < -40) 
        { 
            temp->x =  LEBAR_LAYAR + 20;
            temp->y = rand() % (TINGGI_LAYAR / 2);

            printf("Awan direset ke posisi (%.0f, %.0f)\n", temp->x, temp->y);
        }
        temp = temp->next;
    }
}

void gambarAwan(AwanNode *head)
{
    AwanNode *temp = head;
    while (temp != NULL)
    {
        DrawCircle(temp->x, temp->y, 20, WHITE);
        DrawCircle(temp->x + 15, temp->y + 5, 15, WHITE);
        DrawCircle(temp->x - 15, temp->y + 5, 15, WHITE);
        temp = temp->next;
    }
}

void freeAwan(AwanNode **head)
{
    AwanNode *temp = *head;
    while (temp != NULL)
    {
        AwanNode *toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }
    *head = NULL;
}






