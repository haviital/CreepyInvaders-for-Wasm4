#include "wasm4.h"

// gfx
#define gfxWidth 44
#define gfxHeight 8
#define gfxFlags BLIT_2BPP
const uint8_t gfx[88] = { 0x3f,0xf0,0x00,0x0a,0xaa,0xaa,0xa0,0x00,0x00,0x00,0x01,0x37,0x70,0x02,0xaa,0xaa,0xaa,0xaa,0x80,0x00,0x20,0x01,0x3d,0xf0,0x0a,0xaa,0xaa,0xaa,0xaa,0xa0,0x00,0xa8,0x00,0x0f,0xc0,0x2a,0xaa,0xaa,0xaa,0xaa,0xa8,0x00,0xa8,0x00,0xf3,0x3f,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0x00,0xa8,0x00,0xc3,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xaa,0x00,0x0c,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x2a,0xaa,0xa0,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0xaa,0xa8 };

// Constants
const int INVADER_POS_X = 0;
const int INVADER_POS_Y = 0;
const int INVADER_WIDTH = 8;
const int INVADER_HEIGHT = 8;
const int INVADER_COUNT_IN_ROW = 8;
const int SHELTER_POS_X = INVADER_WIDTH;
const int SHELTER_POS_Y = 0;
const int SHELTER_WIDTH = 24;
const int SHELTER_HEIGHT = 5;
const int SHIP_POS_X = INVADER_WIDTH+SHELTER_WIDTH;
const int SHIP_POS_Y = 0;
const int SHIP_WIDTH = 11;
const int SHIP_HEIGHT = 8;
const int MAX_PLAYERS = 4;
const int MAX_BULLETS = 20;
const int MAX_INVADER_ROWS = 2;
const int MAX_INVADERS_IN_ROW = 8;
const int INVADERS_ROW_START_X = 8;
const int BULLET_WIDTH = 1;  
const int BULLET_HEIGHT = 2; 
const int BULLET_POS_ON_BM_ATLAS_X = INVADER_WIDTH+SHELTER_WIDTH+SHIP_WIDTH; 
const int BULLET_POS_ON_BM_ATLAS_Y = 0;
const int rowEndToEndDistance = 42; // pixels

class GameObject
{
public:
    virtual void init()
    {
    }

    virtual void update()
    {
    }

    virtual void draw()
    {
    }

public:
    float x = 0;
    float y = 0;
    bool isActive = false;
};

class Bullet: public GameObject
{
public:
    void init() override
    {
    }

    void update() override;

    void draw() override;

    bool checkCollision();

public:
    float speedX = 0;
    float speedY = 0;

};

class Player: public GameObject
{
public:
    void init() override
    {
    }

    void update() override;

    void draw() override;

    Bullet* findNextInactiveBullet();

public:

    uint32_t id = 0;
    uint8_t lastGamepad = 0;
};

class InvaderRow: public GameObject
{
public:
    void init() override
    {
    }

    void update() override;

    void draw() override;

public:
    
    uint8_t aliveInvadersBitmask = 0xff;
    int dir = 1;
};

extern int frameNum;
extern Bullet bullets[];
extern Player players[];
extern InvaderRow invaderRows[];
extern int rowEndToEndTime;


