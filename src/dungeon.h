#ifndef _DUNGEON_H__
#define _DUNGEON_H__

#include <ncurses.h>
#include <stdlib.h>

#include "utility.h"

#define DUNGEONWIDTH 3
#define DUNGEONHEIGHT 3
#define DUNGEONSIZE DUNGEONWIDTH*DUNGEONHEIGHT //DO NOT EDIT UNTIL buildDungeon is fixed!
// These will eventually be calculated with DUNGEONSIZE
#define CELLWIDTH 27
#define CELLHEIGHT 9
#define CELLSPACER 3

enum DIRECTIONS
{
	NORTH = 0,
	EAST,
	SOUTH,
	WEST
} DIRECTIONS;

typedef enum ROOMTYPE
{
    ENTRANCE = 0,
    EXIT,
    HALLPIVOT,
    DEFAULT
} ROOMTYPE;

typedef struct Door
{
 	Coord position;
	bool enabled;
} Door;

typedef struct Room
{
	Coord position; // Literal position in screen space
	Coord gridPos; // Position in grid space i/e 1, 3 on a 3x3 grid
	Door * doors;
	struct Room ** connections;
	bool connected;
	int width;
	int height;
	ROOMTYPE type;
	bool discovered;
} Room;

Room ** buildDungeon();
Room * createRoom(int y, int x, int width, int height);
int drawRoom(Room * room);
int drawHall(Room * room1, Room * room2);
bool checkConnections(Room ** rooms);

#endif // _DUNGEON_H__
