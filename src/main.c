#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "dungeon.h"

int initScreen();

int main(int argc, char *argv[])
{
	initScreen();

	srand(time(NULL));

	buildDungeon();

	getch();

	endwin();

	return 0;
}

int initScreen()
{
	initscr();
	noecho();
	refresh();

	return 0;
}

