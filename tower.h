typedef struct
{
    Coord currentLoc;           //Current location
    Coord destLoc;              //Where this guy's headed
    unsigned char isActive;     //0-Not Active,1-Active
    unsigned char moveSpeed;    //Bullet will move every moveSpeed (moveSpeed / 256 = seconds)
    unsigned short lastMove;
    unsigned char sprOffset;    //Offset from BULLET_GFX_OFFSET
    unsigned short target;     //Enemy # to 'chase' (# = offset into enemy array)
    unsigned char aoe;          //How close the bullet has to be to hit the enemy (in 'pixels')
}Bullet;

typedef struct
{
	Coord location;            //Location of the tower
	Coord bLocation;           //Bullet location (aka point of fire)
	unsigned short upgrades;   //See structure below
	unsigned short damage;     //Damage
	unsigned short radius;     //Firing radius
	unsigned short firingRate; //Firing rate (firingRate/4 = seconds)
	unsigned char sprOffset;   //Offset from TOWER_GFX_OFFSET
	unsigned short lastFire;
}Tower;



/*
Tower.upgrades layout
0-4 - Radius bonus
5-9 - Firing rate bonus
10-15 - Damage bonus
*/
