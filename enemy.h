typedef struct
{
	Coord currentLoc;          //Enemy's current location
	unsigned short destLoc;    //Enemy's destination (used as index into waypoint array)
	unsigned short isAlive;    //0-Dead,1-Alive
	signed short health;       //Health left
	unsigned char moveSpeed;   //Enemy will move every moveSpeed (moveSpeed / 256 = seconds)
	unsigned char lastMove;    //Last time the enemy moved
	unsigned char resistance;  //Resistances, see below
	unsigned char weakness;    //Weaknesses, see below
	unsigned char sprGA;       //sprGA = sprITE Ground Air, see below
	unsigned char price;       //Money earned for killing this monster
	unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    unsigned char y0;
    unsigned char y1;
    unsigned char y2;
    unsigned char y3;
}Enemy;

/*
These define corners of the bounding 'hit-able' area on an enemy
0     1
*-----*
|     |
|     |
|     |
|     |
*-----*
2     3
*/

/*
Enemy.resistance layout - 0 indicates no resistance, 1 indicates resistance
0 - Tower Type A
1 - Tower Type B
2 - Tower Type C
3 - Tower Type D
4 - Tower Type E
5 - Tower Type F
6 - Tower Type G
7 - Tower Type H
*/

/*
Enemy.weakness layout - 0 indicates no weakness, 1 indicates weakness
0 - Tower Type A
1 - Tower Type B
2 - Tower Type C
3 - Tower Type D
4 - Tower Type E
5 - Tower Type F
6 - Tower Type G
7 - Tower Type H
*/

/*
Enemy.sprGA layout
0-6 - Index into sprite array
7   - 0 = Ground, 1 = Air
*/
