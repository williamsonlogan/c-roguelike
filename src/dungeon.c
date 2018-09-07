#include "dungeon.h"

Room ** buildDungeon()
{
	//TODO: make dungeon size compatable with something other than 9
	Room ** dungeon;
	dungeon = malloc(sizeof(Room*)*DUNGEONSIZE);

	for(int i = 0; i < 9; i++)
	{
		// Build Rooms
		if(i < 3)
		{
			dungeon[i] = createRoom(1, i * CELLWIDTH, intRand(4, CELLWIDTH - CELLSPACER), intRand(4, CELLHEIGHT - CELLSPACER));
		}
		else if(i < 6)
		{
			dungeon[i] = createRoom(CELLHEIGHT, (i - 3) * CELLWIDTH, intRand(4, CELLWIDTH - CELLSPACER), intRand(4, CELLHEIGHT - CELLSPACER));
		}
		else
		{
			dungeon[i] = createRoom((CELLHEIGHT * 2) - 1, (i - 6) * CELLWIDTH, intRand(4, CELLWIDTH - CELLSPACER), intRand(4, CELLHEIGHT - CELLSPACER));
		}

		// Random offset
		if(dungeon[i]->position.x + dungeon[i]->width != (i+1) * CELLWIDTH && dungeon[i]->position.x != i * CELLWIDTH
		        && dungeon[i]->type != HALLPIVOT)
		{
			int offset = intRand(0, (CELLWIDTH - dungeon[i]->width)- (CELLSPACER/2));
			dungeon[i]->position.x += offset;
			dungeon[i]->doors[NORTH].position.x += offset;
			dungeon[i]->doors[EAST].position.x += offset;
			dungeon[i]->doors[SOUTH].position.x += offset;
			dungeon[i]->doors[WEST].position.x += offset;
		}

		if(dungeon[i]->type != HALLPIVOT)
		{
			//if (dungeon[i]->discovered)
			drawRoom(dungeon[i]);
		}
		else
		{
			dungeon[i]->width = 1;
			dungeon[i]->height = 1;
			dungeon[i]->position.x += CELLWIDTH/2;
			dungeon[i]->position.y += CELLHEIGHT/2;
		}
	}

	Room * curRoom = malloc(sizeof(Room));
	Room * nextRoom = malloc(sizeof(Room));
	int x = intRand(0, DUNGEONWIDTH - 1);
	int y = intRand(0, DUNGEONHEIGHT - 1);
	int newX, newY;
	curRoom = dungeon[(y*DUNGEONWIDTH) + x];

	while(!checkConnections(dungeon))
	{
        for(int i = 0; i < 4; i++)
        {
            if(curRoom->connections[i] == NULL)
            {
                switch(i)
                {
                    case NORTH:
                        newX = x;
                        newY = y - 1;
                        break;
                    case EAST:
                        newX = x + 1;
                        newY = y;
                        break;
                    case SOUTH:
                        newX = x;
                        newY = y + 1;
                        break;
                    case WEST:
                        newX = x - 1;
                        newY = y;
                        break;
                    default:
                        newX = x;
                        newY = y;
                        printw("Oops! Connection not found...");
                        getch();
                        break;
                }

                if(newX >= DUNGEONWIDTH || newX < 0 || newY >= DUNGEONHEIGHT || newY < 0)
                    continue;
                    else{
                nextRoom = dungeon[(newY * DUNGEONWIDTH) + newX];
                break;
                }
            }
            else
                continue;
        }

        x = newX;
        y = newY;
        drawHall(curRoom, nextRoom);
        curRoom = nextRoom;
	}

	free(curRoom);
	free(nextRoom);

	return dungeon;
}


Room * createRoom(int y, int x, int width, int height)
{
	Room * newRoom;
	newRoom = malloc(sizeof(Room));
	newRoom->doors = malloc(sizeof(Door)*4);
	newRoom->connections = malloc(sizeof(Room *)*4);

	for(int i = 0; i < 4; i++)
	{
		switch(i)
		{
			case NORTH:
				newRoom->doors[i].position.x = x + (width/2);
				newRoom->doors[i].position.y = y;
				break;
			case EAST:
				newRoom->doors[i].position.x = x + width;
				newRoom->doors[i].position.y = y + (height/2);
				break;
			case SOUTH:
				newRoom->doors[i].position.x = x + (width/2);
				newRoom->doors[i].position.y = y + height;
				break;
			case WEST:
				newRoom->doors[i].position.x = x;
				newRoom->doors[i].position.y = y + (height/2);
				break;
			default:
				//TODO: ERROR HANDLING
				break;
		}

		newRoom->connections[i] = NULL;
	}

	newRoom->position.x = x;
	newRoom->position.y = y;
	newRoom->gridPos.x = x/CELLWIDTH;
	newRoom->gridPos.y = y/CELLHEIGHT;
	newRoom->width = width;
	newRoom->height = height;
	newRoom->discovered = false;
	newRoom->connected = false;

	int hallRand = intRand(0, 100);

	if(hallRand > 85)
	{
		newRoom->type = HALLPIVOT;
	}
	else
	{
		newRoom->type = DEFAULT;
	}

	return newRoom;
}

int drawRoom(Room * room)
{
	int x, y;

	// draw top and bottom
	for(x = room->position.x; x < room->position.x + room->width; x++)
	{
		mvprintw(room->position.y, x, "-");
		mvprintw(room->position.y + room->height, x, "-");
	}

	// draw floors and walls
	for (y = room->position.y + 1; y < room->position.y + room->height; y++)
	{
		mvprintw(y, room->position.x, "|");
		mvprintw(y, room->position.x + room->width - 1, "|");

		for(x = room->position.x + 1; x < room->position.x +room->width - 1; x++)
		{
			mvprintw(y, x, ".");
		}
	}

	return 0;
}

bool checkConnections(Room ** rooms)
{
    for(int i = 0; i < DUNGEONSIZE; i++)
    {
        if(rooms[i]->connected == false)
        {
            return false;
        }
    }

    return true;
}

// Draw hallways using a midpoint algorithm
int drawHall(Room * room1, Room * room2)
{
	int dir; // Vertical or horizontal
	int d1; // room1 door dir
	int d2; // room2 door dir
	int x0;
	int y0;
	int x1;
	int y1;

	if (room1->gridPos.x == room2->gridPos.x)
	{
		if(room1->gridPos.y < room2->gridPos.y)
		{
			d1 = SOUTH;
			d2 = NORTH;
		}
		else
		{
			d1 = NORTH;
			d2 = SOUTH;
		}

		dir = VERTICAL;
	}
	else
	{
		if(room1->gridPos.x < room2->gridPos.x)
		{
			d1 = EAST;
			d2 = WEST;
		}
		else
		{
			d1 = WEST;
			d2 = EAST;
		}

		dir = HORIZONTAL;
	}

	room1->connections[d1] = room2;
	room1->connected = true;
	room2->connections[d2] = room1;
	room2-> connected = true;

	if(room1->type != HALLPIVOT)
	{
		x0 = room1->doors[d1].position.x;
		y0 = room1->doors[d1].position.y;

		switch(d1)
		{
			case NORTH:
				y0 -= 1;
				break;
			case SOUTH:
				y0 += 1;
				break;
			case EAST:
				//x0 += 1;
				break;
			case WEST:
				//x0 -= 1;
				break;
			default:
				break;
		}
	}
	else
	{
		x0 = room1->position.x;
		y0 = room1->position.y;
	}

	if(room2->type != HALLPIVOT)
	{
		x1 = room2->doors[d2].position.x;
		y1 = room2->doors[d2].position.y;

		switch(d2)
		{
			case NORTH:
				y1 -= 1;
				break;
			case SOUTH:
				y1 += 1;
				break;
			case EAST:
				x1 += 1;
				break;
			case WEST:
				x1 -= 1;
				break;
			default:
				break;
		}
	}
	else
	{
		x1 = room2->position.x;
		y1 = room2->position.y;
	}

	int mid;

	if(dir == HORIZONTAL)
	{
		mid = x0 + ((x1 - x0)/2);
		while(x0 <= mid)
		{
			mvprintw(y0, x0, "#");
			x0++;
		}
		while(x1 > mid)
		{
			mvprintw(y1, x1, "#");
			x1--;
		}

		int dy;
		int ddy;
		if(y0 > y1)
		{
			dy = y0;
			ddy = y1;
		}
		else
		{
			dy = y1;
			ddy = y0;
		}

		while(dy >= ddy)
		{
			mvprintw(dy, x0, "#");
			dy --;
		}
	}
	else
	{
		mid = y0 + ((y1 - y0)/2);
		while(y0 <= mid)
		{
			mvprintw(y0, x0, "#");
			y0++;
		}
		while(y1 > mid)
		{
			mvprintw(y1, x1, "#");
			y1--;
		}

		int dx;
		int ddx;
		if(x0 > x1)
		{
			dx = x0;
			ddx = x1;
		}
		else
		{
			dx = x1;
			ddx = x0;
		}

		while(dx >= ddx)
		{
			mvprintw(y1, dx, "#");
			dx --;
		}
	}
}

