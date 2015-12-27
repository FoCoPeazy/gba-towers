typedef struct {
    signed short x;
    signed short y;
}Coord;

typedef struct
{
    unsigned short* bg;             //Pointer to background map
    unsigned char* validLoc;        //[160/16][240/8] array of valid tower locations 
	signed short waves;				//Number of waves in this level
	unsigned short points;			//Number of waypoints
	unsigned short enemiesAlive;	//How many enemies still alive
	unsigned short towersBuilt;     //# of towers built
	signed short currentWave;		//Current wave (used as index into eOrder/eCount arrays)
	unsigned short* eOrder;			//Type of enemy for each wave
	unsigned short* eCount;			//How many enemies per wave
	Coord* ptArr;					//Waypoints on the level;the first is starting location, the last is ending
}Level;
