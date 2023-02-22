#include "wasm4.h"
#include "gameObjects.h"
#include <stdio.h>
#include <chrono>

// Global variables
int frameNum=0;
Bullet bullets[MAX_BULLETS];
Player players[MAX_PLAYERS];
InvaderRow invaderRows[2];

uint32_t frameTime_ms = 0;
int rowEndToEndTime = 60*2; // 8 sec.

void start()
{
    //*SYSTEM_FLAGS |= SYSTEM_PRESERVE_FRAMEBUFFER; //!!HV

    // Set palette  
    PALETTE[0] = 0x000000;  // color 1: Black
    PALETTE[1] = 0xf70c0c;  // color 2: Red
    PALETTE[2] = 0xffffff;  // color 3: White
    PALETTE[3] = 0x57fa28;  // color 4: Green  

    // Init invader rows.
    for(int i=0; i<MAX_INVADER_ROWS; i++)
    {
        invaderRows[i].y = (float)(40 + (10*i));
    }

    // Init player ships.
    for(int i=0; i<MAX_PLAYERS; i++)
    {
        players[i].x = (float)(24 + (30*i));
        players[i].y = (float)150;
        players[i].id = (uint32_t)i;
    }


}

void update() 
{
    // *** UPDATE

    // Should restart?
    bool hasInvadersLeft = false;
    for(int i=0; i<MAX_INVADER_ROWS; i++)
    {
        if(invaderRows[i].aliveInvadersBitmask != 0)
            hasInvadersLeft = true;
    }

    // Update invader rows.
    for(int i=0; i<MAX_INVADER_ROWS; i++)
    {
        // Should respawn
        if(!hasInvadersLeft)
        {
            invaderRows[i].aliveInvadersBitmask = 0xff;
            rowEndToEndTime -= 5;
        }

        invaderRows[i].update();
    }

    // Update player ships.
    for(int i=0; i<MAX_PLAYERS; i++)
    {
        players[i].update();
    }

    // Update bullets.
    for(int i=0; i<MAX_PLAYERS; i++)
    {
        bullets[i].update();
    }

    // *** DRAW

    // Draw invader rows.
    for(int i=0; i<MAX_INVADER_ROWS; i++)
    {
        invaderRows[i].draw();
    }

    // Draw player ships.
    for(int i=0; i<MAX_PLAYERS; i++)
    {
        players[i].draw();
    }

    // Draw bullets.
    for(int i=0; i<MAX_PLAYERS; i++)
    {
        bullets[i].draw();
    }

    // *** Draw shelters
    *DRAW_COLORS = 0x3421;
    blitSub(gfx, 20, 140, SHELTER_WIDTH, SHELTER_HEIGHT, SHELTER_POS_X, SHELTER_POS_Y, gfxWidth, gfxFlags);
    blitSub(gfx, 70, 140, SHELTER_WIDTH, SHELTER_HEIGHT, SHELTER_POS_X, SHELTER_POS_Y, gfxWidth, gfxFlags);
    blitSub(gfx, 118, 140, SHELTER_WIDTH, SHELTER_HEIGHT, SHELTER_POS_X, SHELTER_POS_Y, gfxWidth, gfxFlags);

    // Draw title.
    *DRAW_COLORS = 0x4;
    char text1[32];
    text("Creepy Invaders", 21, 1);    
    *DRAW_COLORS = 0x3;
    text("*online co-op*", 26, 10);    

    frameNum++;

}
