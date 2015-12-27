#define s_WIDTH     8
#define s_HEIGHT    8

#define MULTIBOOT           int __gba_multiboot;

#define SetMode(mode)       REG_DISPCNT = (mode)

#define REG_DISPCNT         *(volatile unsigned short*)0x4000000
#define BGPaletteMem        ((unsigned short*)0x5000000)
#define REG_VCOUNT          *(volatile unsigned short*)0x4000006
#define REG_DISPSTAT        *(volatile unsigned short*)0x4000004
#define REG_BLDMOD          *(unsigned short*)0x4000050
#define REG_COLEV           *(unsigned short*)0x4000052
#define REG_INTERRUPT       *(unsigned int*)0x3007FFC
#define REG_IME             *(unsigned short*)0x4000208
#define REG_IE              *(unsigned short*)0x4000200
#define REG_IF              *(unsigned short*)0x4000202

#define OBJ_MAP_2D          0x0
#define OBJ_MAP_1D          0x40
#define OBJ_ENABLE          0x1000
#define BG2_ENABLE          0x400

#define SpriteMem           ((unsigned short*)0x7000000)
#define SpriteData          ((unsigned short*)0x6010000)
unsigned short* SpriteData3 = SpriteData + 8192;            //Video modes 3-5, OAMData starts at 0x6010000+8192
#define SpritePal           ((unsigned short*)0x5000200)
#define videoBuffer         ((unsigned short*)0x6000000)

//attribute0 stuff
#define ROTATION_FLAG       0x100
#define SIZE_DOUBLE         0x200
#define MODE_NORMAL         0x0
#define MODE_TRANSPARENT    0x400
#define MODE_WINDOWED       0x800
#define MOSAIC              0x1000
#define COLOR_16            0x0000
#define COLOR_256           0x2000
#define SQUARE              0x0
#define TALL                0x4000
#define WIDE                0x8000

//attribute1 stuff
#define ROTDATA(n)          (n)<<9
#define HORIZONTAL_FLIP     0x1000
#define VERTICAL_FLIP       0x2000
#define SIZE_8              0x0
#define SIZE_16             0x4000
#define SIZE_32             0x8000
#define SIZE_64             0xC000

//attribute2 stuff
#define PRIORITY(n)         (n)<<10
#define PALETTE(n)          (n)<<12

//Here length refers to the number of tiles in sprite memory
//Offset is location of first tile of first sprite
#define ENEMY_GFX_OFFSET    0
#define ENEMY_GFX_LENGTH    2
#define TOWER_GFX_OFFSET    8
#define TOWER_GFX_LENGTH    4
#define BULLET_GFX_OFFSET   16
#define BULLET_GFX_LENGTH   2

#define MAX_ENEMIES_PER_WAVE	10
#define ENEMY_SEPARATION        20   //Pixels between enemies
#define UP                      0
#define DOWN                    1
#define LEFT                    2
#define RIGHT                   3

#define ENEMY_GROUND    0x0
#define ENEMY_AIR       0x80

#define T_BADLOC    0       //Not OK to build here
#define T_OKLOC     1       //OK to build here
#define T_USEDLOC   2       //Occupied space
#define T_TYPES     2       //How many different types of towers

//Enemy type 0
#define ETYPE0_MOVESPEED    32
#define ETYPE0_HEALTH       100
#define ETYPE0_RESISTANCE   0
#define ETYPE0_WEAKNESS     0
#define ETYPE0_SPRGA        0 | ENEMY_GROUND
#define ETYPE0_PRICE        10
#define ETYPE0_X0           0
#define ETYPE0_X1           8
#define ETYPE0_X2           0
#define ETYPE0_X3           8
#define ETYPE0_Y0           0
#define ETYPE0_Y1           0
#define ETYPE0_Y2           8
#define ETYPE0_Y3           8

//Enemy type 1
#define ETYPE1_MOVESPEED    64
#define ETYPE1_HEALTH       200
#define ETYPE1_RESISTANCE   0
#define ETYPE1_WEAKNESS     0
#define ETYPE1_SPRGA        1 | ENEMY_GROUND
#define ETYPE1_PRICE        20
#define ETYPE1_X0           0
#define ETYPE1_X1           8
#define ETYPE1_X2           0
#define ETYPE1_X3           8
#define ETYPE1_Y0           0
#define ETYPE1_Y1           0
#define ETYPE1_Y2           8
#define ETYPE1_Y3           8

//Enemy type 2
#define ETYPE2_MOVESPEED    96
#define ETYPE2_HEALTH       250
#define ETYPE2_RESISTANCE   0
#define ETYPE2_WEAKNESS     0
#define ETYPE2_SPRGA        2 | ENEMY_AIR
#define ETYPE2_PRICE        30
#define ETYPE2_X0           0
#define ETYPE2_X1           8
#define ETYPE2_X2           0
#define ETYPE2_X3           8
#define ETYPE2_Y0           0
#define ETYPE2_Y1           0
#define ETYPE2_Y2           8
#define ETYPE2_Y3           8

//Enemy type 3
#define ETYPE3_MOVESPEED    128
#define ETYPE3_HEALTH       300
#define ETYPE3_RESISTANCE   0
#define ETYPE3_WEAKNESS     0
#define ETYPE3_SPRGA        3 | ENEMY_AIR
#define ETYPE3_PRICE        40
#define ETYPE3_X0           0
#define ETYPE3_X1           8
#define ETYPE3_X2           0
#define ETYPE3_X3           8
#define ETYPE3_Y0           0
#define ETYPE3_Y1           0
#define ETYPE3_Y2           8
#define ETYPE3_Y3           8

//Tower & Bullet Type 0
#define TTYPE0_UPGRADES     0
#define TTYPE0_DAMAGE       50
#define TTYPE0_RADIUS       60
#define TTYPE0_FIRINGRATE   2
#define TTYPE0_SPROFFSET    0
#define BTYPE0_MOVESPEED    1
#define BTYPE0_SPROFFSET    0
#define BTYPE0_AOE          3
#define BTYPE0_XOFFSET      3   //bullet fires from towerX+xOffset
#define BTYPE0_YOFFSET      3   //bullet fires from towerY+yOffset

//Tower & Bullet Type 1
#define TTYPE1_UPGRADES     0
#define TTYPE1_DAMAGE       70
#define TTYPE1_RADIUS       50
#define TTYPE1_FIRINGRATE   3
#define TTYPE1_SPROFFSET    1
#define BTYPE1_MOVESPEED    1
#define BTYPE1_SPROFFSET    0
#define BTYPE1_AOE          3
#define BTYPE1_XOFFSET      3
#define BTYPE1_YOFFSET      3

#define MAX_TOWERS_PER_LEVEL        20
#define TOWER_RADIUS_BONUS          0x001F
#define TOWER_FIRING_BONUS          0x03E0
#define TOWER_DAMAGE_BONUS          0xFC00

#define TIMER_FREQUENCY_SYSTEM  0x0
#define TIMER_FREQUENCY_64      0x1
#define TIMER_FREQUENCY_256     0x2
#define TIMER_FREQUENCY_1024    0x3
#define TIMER_OVERFLOW          0x4
#define TIMER_IRQ_ENABLE        0x40
#define TIMER_ENABLE            0x80

//Timer control
#define REG_TM0CNT      *(volatile unsigned short*)0x4000102
#define REG_TM1CNT      *(volatile unsigned short*)0x4000106
#define REG_TM2CNT      *(volatile unsigned short*)0x400010A
#define REG_TM3CNT      *(volatile unsigned short*)0x400010E

//Timer data
#define REG_TM0D        *(volatile unsigned short*)0x4000100
#define REG_TM1D        *(volatile unsigned short*)0x4000104
#define REG_TM2D        *(volatile unsigned short*)0x4000108
#define REG_TM3D        *(volatile unsigned short*)0x400010C

//Key presses (copied from mygba.h)
#define R_CTRLINPUT     *(volatile unsigned short*)0x4000130
#define F_CTRLINPUT_A_PRESSED       (~R_CTRLINPUT & 0x0001)
#define F_CTRLINPUT_B_PRESSED       ((~R_CTRLINPUT & 0x0002)>>1)
#define F_CTRLINPUT_SELECT_PRESSED  ((~R_CTRLINPUT & 0x0004)>>2)
#define F_CTRLINPUT_START_PRESSED   ((~R_CTRLINPUT & 0x0008)>>3)
#define F_CTRLINPUT_RIGHT_PRESSED   ((~R_CTRLINPUT & 0x0010)>>4)
#define F_CTRLINPUT_LEFT_PRESSED    ((~R_CTRLINPUT & 0x0020)>>5)
#define F_CTRLINPUT_UP_PRESSED      ((~R_CTRLINPUT & 0x0040)>>6)
#define F_CTRLINPUT_DOWN_PRESSED    ((~R_CTRLINPUT & 0x0080)>>7)
#define F_CTRLINPUT_R_PRESSED       ((~R_CTRLINPUT & 0x0100)>>8)
#define F_CTRLINPUT_L_PRESSED       ((~R_CTRLINPUT & 0x0200)>>9)

//Interrupts for REG_IE
#define INT_VBLANK      0x0001
#define INT_HBLANK      0x0002
#define INT_VCOUNT      0x0004
#define INT_TIMER0      0x0008
#define INT_TIMER1      0x0010
#define INT_TIMER2      0x0020
#define INT_TIMER3      0x0040
#define INT_COM         0x0080
#define INT_DMA0        0x0100
#define INT_DMA1        0x0200
#define INT_DMA2        0x0400
#define INT_DMA3        0x0800
#define INT_BUTTON      0x1000
#define INT_CART        0x2000

#define COLOR_BLACK        0x0000
#define COLOR_MAROON       0x0010
#define COLOR_GREEN        0x0200
#define COLOR_OLIVE        0x0210
#define COLOR_NAVY         0x4000
#define COLOR_PURPLE       0x4010
#define COLOR_TEAL         0x4200
#define COLOR_GRAY         0x4210
#define COLOR_SILVER       0x6318
#define COLOR_RED          0x001F
#define COLOR_LIME         0x03E0
#define COLOR_YELLOW       0x03FF
#define COLOR_BLUE         0x7C00
#define COLOR_FUCHSIA      0x7C1F
#define COLOR_WHITE        0x7FFF
#define COLOR_MONEYGREEN   0x6378
#define COLOR_SKYBLUE      0x7B34
#define COLOR_CREAM        0x7BFF
#define COLOR_MEDGRAY      0x529
