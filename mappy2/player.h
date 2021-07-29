#pragma once
#include <allegro.h>

typedef struct SPRITE
{
    int dir, alive;
    int x, y;
    int width, height;
    int xspeed, yspeed;
    int xdelay, ydelay;
    int xcount, ycount;
    int curframe, maxframe, animdir;
    int framecount, framedelay;
}SPRITE;

extern SPRITE* player;

void InitPlayer(char* szName);
void RenderPlayer(BITMAP* buffer, int mapxoff, int mapyoff);
void UpdatePlayer();

