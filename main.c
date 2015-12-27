#include "math.h"
#include "stdio.h"
#include "defines.h"
#include "sprite.h"
#include "levels.h"
#include "enemy.h"
#include "tower.h"
#include "font.h"

MULTIBOOT

typedef struct tagSprite
{
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
}Sprite,*pSprite;

//Create our sprite array
//Sprite sprites[128];
//MAX_ENEMIES_PER_WAVE + MAX_TOWERS_PER_LEVEL + MAX_TOWERS_PER_LEVEL + 1 <= 128
Sprite enemySprites[MAX_ENEMIES_PER_WAVE];
Sprite towerSprites[MAX_TOWERS_PER_LEVEL+1];    //'+1' for the selection tower
Sprite bulletSprites[MAX_TOWERS_PER_LEVEL];

//Create level array
Level levels[1];

//These are used in the checkKeys during VBLANK
unsigned char towPlace = 0;     //0 - Not in tower placement mode,1 - In tower placement mode
unsigned char curTow = 1;       //Which tower type to display on screen
Coord screenTCoord = {0,0};     //Coordinate of tower displayed for selection
//Globals used in the checkKeys() function
Enemy* vblEnemyArr;
Tower* vblTowerArr;
Bullet* vblBulletArr;
Level* vblLevel;

void waitForVsync(void);
void updateSpriteMemory(void);
void startWave(Level*,Enemy*);
void updateEnemies(Level*,Enemy*);
void updateEnemySprites(Level*,Enemy*);
void fillLevel(Level*,unsigned short*,unsigned char*,signed short,unsigned short,unsigned short*,unsigned short*,Coord*);
int findDirection(Coord*,Coord*);
int getDistance(Coord*,Coord*);
void killEnemy(Level*,Enemy*);
void hideSprites();
void updateTowers(Level*,Enemy*,Tower*,Bullet*);
void updateBulletSprites(Level*,Bullet*);
void updateBullets(Level*,Bullet*,Tower*,Enemy*);
void checkKeys();
void buildTower(Level*,Tower*,Bullet*,Coord*,unsigned char);
void updateStatusBar(Level*);
void drawPixel(unsigned char,unsigned char,unsigned short);
void drawChar(unsigned char,unsigned char,unsigned char,unsigned short);
void print(unsigned char,unsigned char,unsigned char*,unsigned short);

int main(void)
{
	unsigned short n;
	
	unsigned short eOrder[]	= {0,1,2,3,2,1,2,3,2,1};
	unsigned short eCount[]	= {4,2,3,3,5,7,2,6,8,9};
	Coord waypoints[]		= {{0,132},
							   {92,132},
							   {92,92},
							   {36,92},
							   {36,20},
							   {109,20},
							   {109,60},
							   {165,60},
							   {165,20},
							   {213,20},
                               {213,92},
                               {151,92},
                               {151,133},
                               {239,133}};
    unsigned char validTowers[(160/16)*(240/8)] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	Level currentLevel;
	fillLevel(&currentLevel,bg1_ian,validTowers,10,14,eOrder,eCount,waypoints);
	Enemy enemyArr[MAX_ENEMIES_PER_WAVE];      //Array of enemies on the 'field'
	Tower towerArr[MAX_TOWERS_PER_LEVEL];
	Bullet bulletArr[MAX_TOWERS_PER_LEVEL];
	vblEnemyArr      = enemyArr;
    vblTowerArr      = towerArr;
    vblBulletArr     = bulletArr;
    vblLevel         = &currentLevel;

    //Set to video mode 3, with bg2, with sprites, 1D mapping
    SetMode(3 | OBJ_ENABLE | OBJ_MAP_1D | BG2_ENABLE);
    
    REG_BLDMOD = (1<<4) | (1<<10);
    REG_COLEV = 8 + (8<<8);

    //Draw the background
    for(n = 0;n < 38400;n++)
    {
        videoBuffer[n] = currentLevel.bg[n];
    }

    //Hide all sprites offscreen
    hideSprites();

    //Set the sprite palette
    for(n = 0;n < 256;n++)
    {
        SpritePal[n] = spritePalette[n];
    }

    //Copy the sprite images into memory (4 32-length sprites)
    for(n = 0;n < (32*4);n++)
    {
        SpriteData3[n] = spriteData[n];
    }

    //Copy the tower images into memory (2 64-length sprite)
    for(n = 32*4;n < (64*2)+(32*4);n++)
    {
        SpriteData3[n] = towerData[n-(32*4)];
    }
    
    //Copy the bullet images into memory (1 32-length sprite)
    for(n = (64*2)+(32*4);n < (64*2)+(32*4)+(32*1);n++)
    {
        SpriteData3[n] = bulletData[n-(64*2)+(32*4)];
    }
    
    //Enable timer0 to 64 cycles, timer1 gets updated 4 times per second
    // if timer0 is 256 cycles, timer1 gets updated 1 time per second
    // if timer0 is SYSTEM cycles, timer1 gets updated 256 times per second
    REG_TM0CNT = TIMER_FREQUENCY_64 | TIMER_ENABLE;
    REG_TM1CNT = TIMER_OVERFLOW | TIMER_ENABLE;
    
    REG_TM2CNT = TIMER_FREQUENCY_SYSTEM | TIMER_ENABLE;
    REG_TM3CNT = TIMER_OVERFLOW | TIMER_ENABLE;
    
    REG_IME = 0x00;
    REG_INTERRUPT = (unsigned int)checkKeys;
    REG_IE |= INT_VBLANK;
    REG_DISPSTAT |= 0x08;
    REG_IME = 0x01;
    
    
    while((currentLevel.currentWave < currentLevel.waves) || ((currentLevel.currentWave == currentLevel.waves) && (currentLevel.enemiesAlive > 0)))
    {
		if(currentLevel.enemiesAlive == 0)
		{
			currentLevel.currentWave++;
			startWave(&currentLevel,enemyArr);
		}
		
		//checkTowers(&currentLevel,towerArr,enemyArr);

        /*DEBUG CODE--METHOD 1 FOR MOVEMENT--UPDATE x TIMES PER FRAME
        for(n = 0;n < enemyArr[0].moveSpeed;n++)
        {
            updateEnemies(&currentLevel,enemyArr);
        }
        END DEBUG CODE*/
        
        /*DEBUG CODE--METHOD 2 FOR MOVEMENT--UPDATE EVERY x FRAMES*/
        updateEnemies(&currentLevel,enemyArr);
        /*END DEBUG CODE*/
        updateTowers(&currentLevel,enemyArr,towerArr,bulletArr);
        updateBullets(&currentLevel,bulletArr,towerArr,enemyArr);

		updateEnemySprites(&currentLevel,enemyArr);
		updateBulletSprites(&currentLevel,bulletArr);
		//updateStatusBar(&currentLevel);
		
        //Wait for vertical retrace
        waitForVsync();

        //Display the sprite
        updateSpriteMemory();
    }
    
    return 0;
}

/*--------------------------------------------------------
waitForVsync()

Waits for vertical retrace
--------------------------------------------------------*/
void waitForVsync(void)
{
    while(!(REG_DISPSTAT & 1));
    while(REG_DISPSTAT & 1);
}

/*--------------------------------------------------------
drawPixel()

Plot a pixel on screen
--------------------------------------------------------*/
void drawPixel(unsigned char x,unsigned char y,unsigned short color)
{
    videoBuffer[y * 240 + x] = color;
}

/*--------------------------------------------------------
drawChar()

Draw a character on screen
--------------------------------------------------------*/
void drawChar(unsigned char left,unsigned char top,unsigned char letter,unsigned short color)
{
    unsigned char x,y;
    
    for(y = 0;y < 8;y++)
    {
        for(x = 0;x < 8;x++)
        {
            if(font[(letter-32)*64 + y*8 + x])
            {
                drawPixel(left+x,top+y,color);
            }
            else
            {
                drawPixel(left+x,top+y,COLOR_BLACK);
            }
        }
    }
}

/*--------------------------------------------------------
print()

Print a string on screen
--------------------------------------------------------*/
void print(unsigned char left,unsigned char top,unsigned char* str,unsigned short color)
{
    unsigned char pos = 0;

    while(*str)
    {
        drawChar(left+pos,top,*str++,color);
        pos += 8;
    }
}

/*--------------------------------------------------------
checkKeys()

Check for keypresses and respond
--------------------------------------------------------*/
void checkKeys()
{
    REG_IME = 0x00;
    unsigned short intFlag = REG_IF;
    
    if((REG_IF & INT_VBLANK) == INT_VBLANK)
    {
        if(F_CTRLINPUT_B_PRESSED)
        {
            towPlace = !towPlace;
        }
        
        if(F_CTRLINPUT_LEFT_PRESSED)
        {
            if(towPlace && screenTCoord.x > 7)
            {
                screenTCoord.x -= 8;
            }
        }
        
        if(F_CTRLINPUT_RIGHT_PRESSED)
        {
            if(towPlace && screenTCoord.x < 232)
            {
                screenTCoord.x += 8;
            }
        }
        
        if(F_CTRLINPUT_UP_PRESSED)
        {
            if(towPlace && screenTCoord.y > 15)
            {
                screenTCoord.y -= 16;
            }
        }
        
        if(F_CTRLINPUT_DOWN_PRESSED)
        {
            if(towPlace && screenTCoord.y < 144)
            {
                screenTCoord.y += 16;
            }
        }
        
        if(F_CTRLINPUT_L_PRESSED)
        {
            if(curTow > 1)
            {
                curTow--;
            }
            else if(curTow == 1)
            {
                curTow = T_TYPES;
            }
        }
        
        if(F_CTRLINPUT_R_PRESSED)
        {
            if(curTow < T_TYPES)
            {
                curTow++;
            }
            else if(curTow == T_TYPES)
            {
                curTow = 1;
            }
        }
        
        if(F_CTRLINPUT_A_PRESSED)
        {
            buildTower(vblLevel,vblTowerArr,vblBulletArr,&screenTCoord,curTow-1);
        }

        if(towPlace)
        {
            towerSprites[MAX_TOWERS_PER_LEVEL].attribute0 = COLOR_256 | screenTCoord.y | WIDE | MODE_TRANSPARENT;
            towerSprites[MAX_TOWERS_PER_LEVEL].attribute1 = SIZE_8 | screenTCoord.x;
            towerSprites[MAX_TOWERS_PER_LEVEL].attribute2 = 512+(TOWER_GFX_OFFSET+(TOWER_GFX_LENGTH*(curTow-1)));
        }
        else
        {
            towerSprites[MAX_TOWERS_PER_LEVEL].attribute0 = 240;
            towerSprites[MAX_TOWERS_PER_LEVEL].attribute1 = 160;
        }
    }
    
    REG_IF = intFlag;
    REG_IME = 0x01;
}

/*--------------------------------------------------------
updateStatusBar()

Update status bar text
--------------------------------------------------------*/
void updateStatusBar(Level* theLevel)
{
    unsigned char status[30];
    sprintf(status,"W %d E %d T %d",theLevel->currentWave,theLevel->enemiesAlive,theLevel->towersBuilt);
    print(1,1,status,COLOR_LIME);
}

/*--------------------------------------------------------
buildTower()

Place tower at specified location
--------------------------------------------------------*/
void buildTower(Level* theLevel,Tower* towerArr,Bullet* bulletArr,Coord* tLoc,unsigned char towerType)
{
    if(theLevel->towersBuilt < MAX_TOWERS_PER_LEVEL)
    {
        if(theLevel->validLoc[((tLoc->y/16)*(240/8))+(tLoc->x/8)] == T_OKLOC)
        {
            switch(towerType)
            {
                case 0:
                    towerArr[theLevel->towersBuilt].bLocation.x = tLoc->x+BTYPE0_XOFFSET;
                    towerArr[theLevel->towersBuilt].bLocation.y = tLoc->y+BTYPE0_YOFFSET;
                    towerArr[theLevel->towersBuilt].damage      = TTYPE0_DAMAGE;
                    towerArr[theLevel->towersBuilt].radius      = TTYPE0_RADIUS;
                    towerArr[theLevel->towersBuilt].firingRate  = TTYPE0_FIRINGRATE;
                    towerArr[theLevel->towersBuilt].lastFire    = 0;
                    towerArr[theLevel->towersBuilt].sprOffset   = TTYPE0_SPROFFSET;
                    
                    bulletArr[theLevel->towersBuilt].isActive   = 0;
                    bulletArr[theLevel->towersBuilt].moveSpeed  = BTYPE0_MOVESPEED;
                    bulletArr[theLevel->towersBuilt].lastMove   = 0;
                    bulletArr[theLevel->towersBuilt].sprOffset  = BTYPE0_SPROFFSET;
                    bulletArr[theLevel->towersBuilt].aoe        = BTYPE0_AOE;
                    bulletArr[theLevel->towersBuilt].currentLoc = towerArr[theLevel->towersBuilt].bLocation;
                break;
                
                case 1:
                    towerArr[theLevel->towersBuilt].bLocation.x = tLoc->x+BTYPE1_XOFFSET;
                    towerArr[theLevel->towersBuilt].bLocation.y = tLoc->y+BTYPE1_YOFFSET;
                    towerArr[theLevel->towersBuilt].damage      = TTYPE1_DAMAGE;
                    towerArr[theLevel->towersBuilt].radius      = TTYPE1_RADIUS;
                    towerArr[theLevel->towersBuilt].firingRate  = TTYPE1_FIRINGRATE;
                    towerArr[theLevel->towersBuilt].lastFire    = 0;
                    towerArr[theLevel->towersBuilt].sprOffset   = TTYPE1_SPROFFSET;

                    bulletArr[theLevel->towersBuilt].isActive   = 0;
                    bulletArr[theLevel->towersBuilt].moveSpeed  = BTYPE1_MOVESPEED;
                    bulletArr[theLevel->towersBuilt].lastMove   = 0;
                    bulletArr[theLevel->towersBuilt].sprOffset  = BTYPE1_SPROFFSET;
                    bulletArr[theLevel->towersBuilt].aoe        = BTYPE1_AOE;
                    bulletArr[theLevel->towersBuilt].currentLoc = towerArr[theLevel->towersBuilt].bLocation;
                break;
            }
            
            towerArr[theLevel->towersBuilt].location.x  = tLoc->x;
            towerArr[theLevel->towersBuilt].location.y  = tLoc->y;
            
            towerSprites[theLevel->towersBuilt].attribute0 = COLOR_256 | towerArr[theLevel->towersBuilt].location.y | WIDE;
            towerSprites[theLevel->towersBuilt].attribute1 = SIZE_8 | towerArr[theLevel->towersBuilt].location.x;
            towerSprites[theLevel->towersBuilt].attribute2 = 512+(TOWER_GFX_OFFSET+(TOWER_GFX_LENGTH*(towerArr[theLevel->towersBuilt].sprOffset)));
            
            bulletSprites[theLevel->towersBuilt].attribute0 = COLOR_256 | bulletArr[theLevel->towersBuilt].currentLoc.y;
            bulletSprites[theLevel->towersBuilt].attribute1 = SIZE_8 | bulletArr[theLevel->towersBuilt].currentLoc.x;
            bulletSprites[theLevel->towersBuilt].attribute2 = 512+(BULLET_GFX_OFFSET+(BULLET_GFX_LENGTH*(bulletArr[theLevel->towersBuilt].sprOffset)));
            
            theLevel->towersBuilt++;
            theLevel->validLoc[((tLoc->y/16)*(240/8))+(tLoc->x/8)] = T_USEDLOC;
        }
    }
}

/*--------------------------------------------------------
hideSprites()

Hide all sprites offscreen
--------------------------------------------------------*/
void hideSprites()
{
    unsigned short j;
    for(j = 0;j < MAX_ENEMIES_PER_WAVE;j++)
    {
        enemySprites[j].attribute0 = 160;
        enemySprites[j].attribute1 = 240;
    }
    for(j = 0;j < MAX_TOWERS_PER_LEVEL;j++)
    {
        towerSprites[j].attribute0 = 160;
        towerSprites[j].attribute1 = 240;
        bulletSprites[j].attribute0 = 160;
        bulletSprites[j].attribute1 = 240;
    }
}

/*--------------------------------------------------------
updateSpriteMemory()

Copy sprite array into OAM memory
--------------------------------------------------------*/
void updateSpriteMemory()
{
    //128 sprites, each has 4 unsigned shorts, 128*4 unsigned short's
    int n;
    unsigned short* temp = (unsigned short*)enemySprites;

    for(n = 0;n < MAX_ENEMIES_PER_WAVE*4;n++)
    {
        SpriteMem[n] = temp[n];
    }
    
    temp = (unsigned short*)towerSprites;
    for(n = MAX_ENEMIES_PER_WAVE*4;n < (MAX_ENEMIES_PER_WAVE*4)+((MAX_TOWERS_PER_LEVEL+1)*4);n++)
    {
        SpriteMem[n] = temp[n-(MAX_ENEMIES_PER_WAVE*4)];
    }
    
    temp = (unsigned short*)bulletSprites;
    for(n = (MAX_ENEMIES_PER_WAVE*4)+((MAX_TOWERS_PER_LEVEL+1)*4);n < (MAX_ENEMIES_PER_WAVE*4)+(((MAX_TOWERS_PER_LEVEL+1)*4)+(MAX_TOWERS_PER_LEVEL*4));n++)
    {
        SpriteMem[n] = temp[n-(MAX_ENEMIES_PER_WAVE*4)-((MAX_TOWERS_PER_LEVEL+1)*4)];
    }
}

/*--------------------------------------------------------
updateEnemySprites()

Update all enemy graphics
--------------------------------------------------------*/
void updateEnemySprites(Level* theLevel,Enemy* enemyArr)
{
	int n;
	for(n = 0;n < MAX_ENEMIES_PER_WAVE;n++)
	{
	    if(!enemyArr[n].isAlive)
	    {
	        enemySprites[n].attribute0 = COLOR_256 | 160;
	        enemySprites[n].attribute1 = SIZE_8 | 240;
        }
        else
        {
            enemySprites[n].attribute0 = COLOR_256 | enemyArr[n].currentLoc.y;
            enemySprites[n].attribute1 = SIZE_8 | enemyArr[n].currentLoc.x;
            enemySprites[n].attribute2 = 512+(ENEMY_GFX_OFFSET+(ENEMY_GFX_LENGTH*(enemyArr[n].sprGA & 0x7F)));
        }
    }
}

/*--------------------------------------------------------
updateBulletSprites()

Update all bullet graphics
--------------------------------------------------------*/
void updateBulletSprites(Level* theLevel,Bullet* bulletArr)
{
	int n;
	for(n = 0;n < theLevel->towersBuilt;n++)
	{
	    if(!bulletArr[n].isActive)
	    {
	        bulletSprites[n].attribute0 = COLOR_256 | 160;
	        bulletSprites[n].attribute1 = SIZE_8 | 240;
        }
        else
        {
            bulletSprites[n].attribute0 = COLOR_256 | bulletArr[n].currentLoc.y;
            bulletSprites[n].attribute1 = SIZE_8 | bulletArr[n].currentLoc.x;
            bulletSprites[n].attribute2 = 512+(BULLET_GFX_OFFSET+(BULLET_GFX_LENGTH*(bulletArr[n].sprOffset)));
        }
	}
}

/*--------------------------------------------------------
updateEnemies()

Update all enemy positions
--------------------------------------------------------*/
void updateEnemies(Level* theLevel,Enemy* enemyArr)
{
	int n;
	unsigned short tempTime;

	if(theLevel->currentWave >= theLevel->waves)
	{
		return;
	}
	
	for(n = 0;n < theLevel->eCount[theLevel->currentWave];n++)
	{
	    tempTime = REG_TM3D;
	    
		if(enemyArr[n].isAlive && (enemyArr[n].lastMove + enemyArr[n].moveSpeed <= tempTime))
		{
			if((enemyArr[n].currentLoc.x) == (theLevel->ptArr[enemyArr[n].destLoc].x))
    		{
        		if((enemyArr[n].currentLoc.y) < (theLevel->ptArr[enemyArr[n].destLoc].y))
        		{
            		enemyArr[n].currentLoc.y++;
            		enemyArr[n].lastMove = tempTime;
        		}
        		else if((enemyArr[n].currentLoc.y) > (theLevel->ptArr[enemyArr[n].destLoc].y))
        		{
            		enemyArr[n].currentLoc.y--;
            		enemyArr[n].lastMove = tempTime;
        		}
    		}
    		else if((enemyArr[n].currentLoc.y) == (theLevel->ptArr[enemyArr[n].destLoc].y))
    		{
        		if((enemyArr[n].currentLoc.x) < (theLevel->ptArr[enemyArr[n].destLoc].x))
        		{
            		enemyArr[n].currentLoc.x++;
            		enemyArr[n].lastMove = tempTime;
        		}
        		else if((enemyArr[n].currentLoc.x) > (theLevel->ptArr[enemyArr[n].destLoc].x))
        		{
            		enemyArr[n].currentLoc.x--;
            		enemyArr[n].lastMove = tempTime;
        		}
    		}
    	
    		if(((enemyArr[n].currentLoc.x) == (theLevel->ptArr[enemyArr[n].destLoc].x)) && ((enemyArr[n].currentLoc.y) == (theLevel->ptArr[enemyArr[n].destLoc].y)))
    		{
				if(enemyArr[n].destLoc == (theLevel->points-1))
				{
				    killEnemy(theLevel,&enemyArr[n]);
				}
				else
				{
					enemyArr[n].destLoc++;
				}
			}
		}
	}
}

/*--------------------------------------------------------
updateBullets()

Update all bullet positions
--------------------------------------------------------*/
void updateBullets(Level* theLevel,Bullet* bulletArr,Tower* towerArr,Enemy* enemyArr)
{
	int i;
	unsigned short tempTime;
	
	for(i = 0;i < theLevel->towersBuilt;i++)
	{
	    tempTime = REG_TM3D;
	    
	    if(bulletArr[i].isActive && (bulletArr[i].lastMove + bulletArr[i].moveSpeed <= tempTime))
	    {
	        if(!enemyArr[bulletArr[i].target].isAlive)
	        {
	            bulletArr[i].isActive = 0;
	            bulletArr[i].currentLoc = towerArr[i].bLocation;
	            continue;
            }
            
            bulletArr[i].destLoc = enemyArr[bulletArr[i].target].currentLoc;

            if(bulletArr[i].currentLoc.x < bulletArr[i].destLoc.x)
            {
                bulletArr[i].currentLoc.x++;
                bulletArr[i].lastMove = tempTime;
            }
            else if(bulletArr[i].currentLoc.x > bulletArr[i].destLoc.x)
            {
                bulletArr[i].currentLoc.x--;
                bulletArr[i].lastMove = tempTime;
            }

            if(bulletArr[i].currentLoc.y < bulletArr[i].destLoc.y)
            {
                bulletArr[i].currentLoc.y++;
                bulletArr[i].lastMove = tempTime;
            }
            else if(bulletArr[i].currentLoc.y > bulletArr[i].destLoc.y)
            {
                bulletArr[i].currentLoc.y--;
                bulletArr[i].lastMove = tempTime;
            }

            if(getDistance(&bulletArr[i].currentLoc,&bulletArr[i].destLoc) <= bulletArr[i].aoe)
            {
                enemyArr[bulletArr[i].target].health -= towerArr[i].damage;
                bulletArr[i].isActive = 0;
                bulletArr[i].currentLoc = towerArr[i].bLocation;
                if(enemyArr[bulletArr[i].target].health <= 0)
                {
                    killEnemy(theLevel,&enemyArr[bulletArr[i].target]);
                }
            }
        }
	}
}

/*--------------------------------------------------------
killEnemy()

Kill the enemy
--------------------------------------------------------*/
void killEnemy(Level* theLevel,Enemy* theEnemy)
{
    theEnemy->isAlive       = 0;
    theEnemy->currentLoc.x  = 240;
    theEnemy->currentLoc.y  = 160;
    theLevel->enemiesAlive--;
}

/*--------------------------------------------------------
findDirection()

Find the direction to head from coordA to coordB
*Uses GBA coordinates (eg [0,0] is above [0,10])
*Return values defined in levels.h
--------------------------------------------------------*/
int findDirection(Coord* coordA,Coord* coordB)
{
    if((coordA->x < coordB->x) && (coordA->y == coordB->y))
    {
        return RIGHT;
    }
    
    if((coordA->x > coordB->x) && (coordA->y == coordB->y))
    {
        return LEFT;
    }
    
    if((coordA->y < coordB->y) && (coordA->x == coordB->x))
    {
        return DOWN;
    }
    
    if((coordA->y > coordB->y) && (coordA->x == coordB->x))
    {
        return UP;
    }
    
    return -1;
}

/*--------------------------------------------------------
startWave()

Setup data to start the next wave
--------------------------------------------------------*/
void startWave(Level* theLevel,Enemy* enemyArr)
{
    int n;
	signed short addX,addY,health;
	unsigned char moveSpeed,resistance,weakness,sprGA,price,x0,x1,x2,x3,y0,y1,y2,y3;
	
	switch(findDirection(&(theLevel->ptArr[0]),&(theLevel->ptArr[1])))
	{
        case UP:
            addX = 0;
            addY = ENEMY_SEPARATION;
        break;
        
        case DOWN:
            addX = 0;
            addY = -ENEMY_SEPARATION;
        break;
        
        case LEFT:
            addX = ENEMY_SEPARATION;
            addY = 0;
        break;
        
        case RIGHT:
            addX = -ENEMY_SEPARATION;
            addY = 0;
        break;
        
        default:
            addX = 0;
            addY = 0;
        break;
	}

	switch(theLevel->eOrder[theLevel->currentWave])
	{
        case 0:
            moveSpeed   = ETYPE0_MOVESPEED;
            health      = ETYPE0_HEALTH;
            resistance  = ETYPE0_RESISTANCE;
            weakness    = ETYPE0_WEAKNESS;
            sprGA       = ETYPE0_SPRGA;
            price       = ETYPE0_PRICE;
            x0          = ETYPE0_X0;
            y0          = ETYPE0_Y0;
            x1          = ETYPE0_X1;
            y1          = ETYPE0_Y1;
            x2          = ETYPE0_X2;
            y2          = ETYPE0_Y2;
            x3          = ETYPE0_X3;
            y3          = ETYPE0_Y3;
        break;
        
        case 1:
            moveSpeed   = ETYPE1_MOVESPEED;
            health      = ETYPE1_HEALTH;
            resistance  = ETYPE1_RESISTANCE;
            weakness    = ETYPE1_WEAKNESS;
            sprGA       = ETYPE1_SPRGA;
            price       = ETYPE1_PRICE;
            x0          = ETYPE1_X0;
            y0          = ETYPE1_Y0;
            x1          = ETYPE1_X1;
            y1          = ETYPE1_Y1;
            x2          = ETYPE1_X2;
            y2          = ETYPE1_Y2;
            x3          = ETYPE1_X3;
            y3          = ETYPE1_Y3;
        break;
        
        case 2:
            moveSpeed   = ETYPE2_MOVESPEED;
            health      = ETYPE2_HEALTH;
            resistance  = ETYPE2_RESISTANCE;
            weakness    = ETYPE2_WEAKNESS;
            sprGA       = ETYPE2_SPRGA;
            price       = ETYPE2_PRICE;
            x0          = ETYPE2_X0;
            y0          = ETYPE2_Y0;
            x1          = ETYPE2_X1;
            y1          = ETYPE2_Y1;
            x2          = ETYPE2_X2;
            y2          = ETYPE2_Y2;
            x3          = ETYPE2_X3;
            y3          = ETYPE2_Y3;
        break;
        
        case 3:
            moveSpeed   = ETYPE3_MOVESPEED;
            health      = ETYPE3_HEALTH;
            resistance  = ETYPE3_RESISTANCE;
            weakness    = ETYPE3_WEAKNESS;
            sprGA       = ETYPE3_SPRGA;
            price       = ETYPE3_PRICE;
            x0          = ETYPE3_X0;
            y0          = ETYPE3_Y0;
            x1          = ETYPE3_X1;
            y1          = ETYPE3_Y1;
            x2          = ETYPE3_X2;
            y2          = ETYPE3_Y2;
            x3          = ETYPE3_X3;
            y3          = ETYPE3_Y3;
        break;
        
        default:
            moveSpeed   = ETYPE0_MOVESPEED;
            health      = ETYPE0_HEALTH;
            resistance  = ETYPE0_RESISTANCE;
            weakness    = ETYPE0_WEAKNESS;
            sprGA       = ETYPE0_SPRGA;
            price       = ETYPE0_PRICE;
            x0          = ETYPE0_X0;
            y0          = ETYPE0_Y0;
            x1          = ETYPE0_X1;
            y1          = ETYPE0_Y1;
            x2          = ETYPE0_X2;
            y2          = ETYPE0_Y2;
            x3          = ETYPE0_X3;
            y3          = ETYPE0_Y3;
        break;
	}
	
	for(n = 0;n < theLevel->eCount[theLevel->currentWave];n++)
    {
        enemyArr[n].currentLoc.x	= (theLevel->ptArr[0].x)+(addX*n);
        enemyArr[n].currentLoc.y	= (theLevel->ptArr[0].y)+(addY*n);
        enemyArr[n].destLoc		= 1;
        enemyArr[n].isAlive		= 1;
        enemyArr[n].moveSpeed   = moveSpeed;
        enemyArr[n].lastMove    = 0;
        enemyArr[n].resistance  = resistance;
        enemyArr[n].weakness    = weakness;
        enemyArr[n].sprGA       = sprGA;
        enemyArr[n].price       = price;
        enemyArr[n].health      = health;
        enemyArr[n].x0          = x0;
        enemyArr[n].y0          = y0;
        enemyArr[n].x1          = x1;
        enemyArr[n].y1          = y1;
        enemyArr[n].x2          = x2;
        enemyArr[n].y2          = y2;
        enemyArr[n].x3          = x3;
        enemyArr[n].y3          = y3;
    }
    for(n = theLevel->eCount[theLevel->currentWave];n < MAX_ENEMIES_PER_WAVE;n++)
    {
        enemyArr[n].currentLoc	= theLevel->ptArr[(theLevel->points)-1];
        enemyArr[n].destLoc		= (theLevel->points)-1;
        enemyArr[n].isAlive		= 0;
    }
    
	theLevel->enemiesAlive = theLevel->eCount[theLevel->currentWave];
}

/*--------------------------------------------------------
updateTowers()

Update towers to fire;update bullet locations
--------------------------------------------------------*/
void updateTowers(Level* theLevel,Enemy* enemyArr,Tower* towerArr,Bullet* bulletArr)
{
    Coord e0,e1,e2,e3;  //These define the 'hitbox' of the enemy
    int i,j;
    
    for(i = 0;i < theLevel->towersBuilt;i++)
    {
        for(j = 0;j < theLevel->eCount[theLevel->currentWave];j++)
        {
            if(enemyArr[j].isAlive)
            {
                e0.x = enemyArr[j].currentLoc.x + enemyArr[j].x0;
                e0.y = enemyArr[j].currentLoc.y + enemyArr[j].y0;
                e1.x = enemyArr[j].currentLoc.x + enemyArr[j].x1;
                e1.y = enemyArr[j].currentLoc.y + enemyArr[j].y1;
                e2.x = enemyArr[j].currentLoc.x + enemyArr[j].x2;
                e2.y = enemyArr[j].currentLoc.y + enemyArr[j].y2;
                e3.x = enemyArr[j].currentLoc.x + enemyArr[j].x3;
                e3.y = enemyArr[j].currentLoc.y + enemyArr[j].y3;
                
                
                if(!bulletArr[i].isActive && (towerArr[i].lastFire + towerArr[i].firingRate <= REG_TM1D))
                {
                    if((getDistance(&(bulletArr[i].currentLoc),&e0) <= towerArr[i].radius) || (getDistance(&(bulletArr[i].currentLoc),&e1) <= towerArr[i].radius) || (getDistance(&(bulletArr[i].currentLoc),&e2) <= towerArr[i].radius) || (getDistance(&(bulletArr[i].currentLoc),&e3) <= towerArr[i].radius))
                    {
                        bulletArr[i].currentLoc = towerArr[i].bLocation;
                        bulletArr[i].isActive = 1;
                        towerArr[i].lastFire = REG_TM1D;
                        bulletArr[i].target = j;
                    }
                }
            }
        }
    }
}

/*--------------------------------------------------------
fillLevel()

Setup data to start the level
--------------------------------------------------------*/
void fillLevel(Level* theLevel,unsigned short* background,unsigned char* validLoc,signed short waves,unsigned short points,unsigned short* enemyOrder,unsigned short* enemyCount,Coord* waypointArr)
{
    theLevel->bg            = background;
    theLevel->validLoc      = validLoc;
    theLevel->waves         = waves;
    theLevel->points        = points;
    theLevel->eOrder        = enemyOrder;
    theLevel->eCount        = enemyCount;
    theLevel->ptArr         = waypointArr;
    theLevel->enemiesAlive  = 0;
    theLevel->currentWave   = -1;
    theLevel->towersBuilt   = 0;
}

/*--------------------------------------------------------
getDistance()

Find the distance between two coordinates
--------------------------------------------------------*/
int getDistance(Coord* a,Coord* b)
{
	return (int)(sqrt(((a->x - b->x)*(a->x - b->x)) + ((a->y - b->y)*(a->y - b->y))));
}
/* END OF FILE */
