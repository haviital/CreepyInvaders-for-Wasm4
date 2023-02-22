#include "wasm4.h"
#include "gameObjects.h"

// *** class Bullet

void Bullet::update()
{
    if(isActive)
    {
        x += speedX;
        y += speedY; 

        checkCollision();
    }

    // Deactivate if oob
    if(x< -BULLET_WIDTH || x> 160 || y< -BULLET_HEIGHT || y > 160 )
        isActive = false;
}

void Bullet::draw()
{
    *DRAW_COLORS = 0x3420;
    if(isActive)
        blitSub(gfx, (int)x, (int)y, BULLET_WIDTH,  BULLET_HEIGHT, BULLET_POS_ON_BM_ATLAS_X, BULLET_POS_ON_BM_ATLAS_Y, gfxWidth, gfxFlags);
}

bool Bullet::checkCollision()
{
    
    // Invader rows.
    for(int ii=0; ii<MAX_INVADER_ROWS; ii++)
    {
        if(y >= invaderRows[ii].y && y < invaderRows[ii].y +INVADER_HEIGHT )
        {
            // Might be colliding.
            for(int i=0; i < INVADER_COUNT_IN_ROW; i++)
            {
                // If is alive.
                uint8_t bit = (0x80>>i);
                if(invaderRows[ii].aliveInvadersBitmask & bit) 
                {
                    int invPosX = (int)10 + (int)invaderRows[ii].x + i*14;
                    if(invaderRows[ii].dir<0)
                        invPosX = (int)10 + (rowEndToEndDistance - (int)invaderRows[ii].x) + i*14;
                    if(frameNum%30 > 15)
                        invPosX--;
                    const int shrink = 1;
                    if((int)x >= invPosX+shrink && (int)x < invPosX + INVADER_WIDTH - shrink)
                    {
                        // Collided!

                        // Clear the alive bit
                        invaderRows[ii].aliveInvadersBitmask &= ~bit;
                        
                        // Remove the bullet
                        isActive = false;

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// *** class Player

void Player::update()
{
    // Move
    uint8_t gamepad = *(GAMEPAD1 + id);
    if (gamepad & BUTTON_RIGHT)
    {
        if((int)x < 160-SHIP_WIDTH)
            x += 1.f;
    }    
    else if (gamepad & BUTTON_LEFT)
    {
        if((int)x > 0)
            x -= 1.f;
    }    
    
    // Shoot 
    bool isJustPressedButton1 = !(lastGamepad & BUTTON_1) && (gamepad & BUTTON_1);
    //tracef("just pressed=%d. gamepad=0x%x, lastGamepad=0x%x", isJustPressedButton1,gamepad, lastGamepad);
    if (isJustPressedButton1 )
    {
        Bullet* bullet = findNextInactiveBullet();
        if(bullet)
        {
            bullet->isActive = true;
            bullet->x = x+6;
            bullet->y = y;
            bullet->speedY = -3;
            bullet->speedX = 0;
        }
    } 

    lastGamepad = gamepad; 
}

void Player::draw()
{
    *DRAW_COLORS = 0x3420;
    blitSub(gfx, (int)x, (int)y, SHIP_WIDTH, SHIP_HEIGHT, SHIP_POS_X, SHIP_POS_Y, gfxWidth, gfxFlags);

    // Draw dots based on the id

    *DRAW_COLORS = 0x02;
    for(uint32_t i = 0; i<=id; i++)
        hline((int)x+2+((int)i*2),(int)y+SHIP_HEIGHT-2, 1);
        //hline((int)x+5,(int)y+SHIP_HEIGHT-2, 2);
}

Bullet* Player::findNextInactiveBullet()
{
    for(int i=0; i<MAX_BULLETS; i++)
    {
        if(! bullets[i].isActive)
            return &(bullets[i]);  
    }
    return nullptr; // No free bullets!
}

// *** class InvaderRow

void InvaderRow::update()
{
    int x3 = frameNum%rowEndToEndTime; // 0 -- rowEndToEndTime-1
    x = (float)rowEndToEndDistance * (float)x3/((float)rowEndToEndTime); // 0 -- 42

    // Change moving direction
    if(x3==0)
        dir = -dir;
}

void InvaderRow::draw()
{       
    *DRAW_COLORS = 0x3421;
    for(int i=0; i < INVADER_COUNT_IN_ROW; i++)
    {
        if(aliveInvadersBitmask & (0x80>>i)) 
        {
            int posX = (int)10 + (int)x + i*14;
            if(dir<0)
                posX = (int)10 + (rowEndToEndDistance - (int)x) + i*14;
            uint32_t flags = gfxFlags;
            if(frameNum%30 > 15)
            {
                flags |= BLIT_FLIP_X;
                posX--;
            }
            blitSub(gfx, posX, (int)y, INVADER_WIDTH, INVADER_HEIGHT, INVADER_POS_X, INVADER_POS_Y, gfxWidth, flags);
        }
    }
}

