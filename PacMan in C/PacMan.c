#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "module.h"
#include <stdbool.h>
#include <conio.h>

#define ROW		21
#define COL		28
#define SPACE	0
#define WALL	1
#define DOT		2

#define PLAYER	'0'
#define GHOST1	'A'
#define GHOST2	'B'
#define GHOST3	'C'
#define GHOST4	'D'

/// A map array represents wall, space, and dot
int map[ROW][COL] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
	{1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, },
	{1, 2, 1, 0, 0, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 1, 2, 1, 1, 2, 1, },
	{1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, },
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
	{1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
	{0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
	{0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
	{1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
	{0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, },
	{1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
	{0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, },
	{1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, },
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
	{1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, },
	{1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 2, 0, 0, 2, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, },
	{1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, },
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

/// An enum object that maps directional arrows into ASCII
typedef enum Arrows {
	INVALID = -1,
	LEFT = 75,
	RIGHT = 77,
	UP = 72,
	DOWN = 80,
} Arrows;

/// A struct for a player
typedef struct Player
{
	int x, y;
} Player;

/// A struct for a ghost
typedef struct Ghost
{
	int		x, y;
	int		deltaX, deltaY;
	int		lastUpdated;
	char	model;
} Ghost;

/// Clampe the given value from 0 to max
int clamp(int val, int max);

/// Returns a map tile by coord
char getMapTile(int x, int y);

/// Converts x, y direction into an arrow order, LEFT, RIGHT, UP, DOWN
int directionToFlatOrder(int x, int y);

/// Returns an arrow direction by user input
Arrows handleArrowInput();

/// Moves the player location by Arrow
void tryMovePlayerPosition(Player* player, Arrows arrow);

/// An array of ghost instances
Ghost** ghosts;

/// Number of ghosts
int ghostCount = 0;

/// remaining dots to eat
int dots = 0;

/// Stores how many mileseconds are elapsed from start
clock_t gameMileseconds;

/// Creates a ghost in the given position
Ghost* createGhost(int x, int y, int deltaX, int deltaY, char model);

/// Updates ghost position and render again
void updateGhost(Ghost* ghost);

/// Prints out a debug message in the given position
void DebugMessage(int x, int y, const char* str, ...)
{
	gotoxy(x, y);
	va_list _ArgList;
	va_start(_ArgList, x, y, str);
	vprintf(str, _ArgList);
	va_end(_ArgList);
}

void main()
{
	// Initialize
	randomize();
	showcursor(false);

	// Draws the map first
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			char tile = getMapTile(j, i);
			_putch(tile);

			if (tile == '.')
			{
				dots++;
			}
		}
		_putch('\n');
	}

	Player player;
	// Init player position
	player.x = 13; player.y = 16;
	gotoxy(player.x, player.y);
	_putch(PLAYER);

	clock_t milesecond = clock();

	while (true)
	{
		Arrows arrowInput = handleArrowInput();

		if (arrowInput != INVALID)
		{
			tryMovePlayerPosition(&player, arrowInput);

			// When no dots left in the map, terminate the game
			if (dots == 0) break;
			DebugMessage(6, 25, " VICTORY ", dots);

			for (int i = 0; i < ghostCount; i++)
			{
				Ghost* ghost = ghosts[i];

				// When the player collides with a ghost, game lose
				if (ghost->x == player.x && ghost->y == player.y)
				{
					DebugMessage(6, 25, " YOU LOSE ", dots);
					goto end;
				}
			}
		}

		// At gametime 1 sec, creates the first ghost
		if (ghostCount < 1 && gameMileseconds > 1000)
		{
			createGhost(11, 7, 1, 0, 'A');
		}
		else if (ghostCount < 2 && gameMileseconds > 4000) // At game time 4 sec, creates the second ghost
		{
			createGhost(11, 7, 1, 0, 'B');
		}
		else if (ghostCount < 3 && gameMileseconds > 8000) // At game time 8 sec, creates the third ghost
		{
			createGhost(11, 7, 1, 0, 'C');
		}
		else if (ghostCount < 4 && gameMileseconds > 12000) // At game time 12 sec, creates the fourth ghost
		{
			createGhost(11, 7, 1, 0, 'D');
		}

		// Tick ghosts
		for (int i = 0; i < ghostCount; i++)
		{
			Ghost* ghost = ghosts[i];
			updateGhost(ghost);
		}

		clock_t milesecond2 = clock();

		// Increment time count
		gameMileseconds += milesecond2 - milesecond;
		milesecond = milesecond2;

		// Prints game time
		if (dots >= 100)
			DebugMessage(6, 25, "dots left = %d", dots);
		else if (dots >= 10)
			DebugMessage(6, 25, "dots left =  %d", dots); // added a space in front of the score to clear 3 characters
		else
			DebugMessage(6, 25, "dots left =   %d", dots); // added two spaces in front of the score to clear 3 characters
	}

end:

	gotoxy(0, 30);
	showcursor(true);

	// When the game ends, remove all ghosts from memory
	for (int i = 0; i < ghostCount; i++) free(ghosts[i]);
	free(ghosts);
}

int clamp(int val, int max)
{
	while (val < 0)		val += max;
	while (val >= max)	val -= max;
	return val;
}

char getMapTile(int x, int y)
{
	switch (map[y][x])
	{
	case WALL:
		return '@';
	case SPACE:
		return ' ';
	case DOT:
		return '.';
	}

	return ' ';
}

int directionToFlatOrder(int x, int y)
{
	if (x == 0)
	{
		if (y == -1)
		{
			return 2; // UP, (0, -1)
		}
		else if (y == 1)
		{
			return 3; // DOWN, (0, 1)
		}
	}
	else if (y == 0)
	{
		if (x == -1)
		{
			return 0; // LEFT, (-1, 0)
		}
		else if (x == 1)
		{
			return 1; // RIGHT, (1, 0)
		}
	}

	// Returns an invalid ordering number for an exceptional case
	return -1;
}

Arrows handleArrowInput()
{
	if (_kbhit()) {
		int ch = _getch();

		// Judge expand key
		if (ch == 0x00 || ch == 0xE0)
		{
			// Input the real character
			ch = _getch();

			if (ch == UP || ch == DOWN || ch == LEFT || ch == RIGHT)
			{
				return (Arrows)ch;
			}
		}
	}

	return INVALID;
}

void tryMovePlayerPosition(Player* player, Arrows arrow)
{
	int nextX = player->x, nextY = player->y;

	// Move the current coord by the arrow input
	switch (arrow)
	{
	case UP:
		nextY--;
		break;
	case DOWN:
		nextY++;
		break;
	case LEFT:
		nextX--;
		break;
	case RIGHT:
		nextX++;
		break;
	}

	// Clamp coordinates
	nextX = clamp(nextX, COL);
	nextY = clamp(nextY, ROW);

	if (map[nextY][nextX] == DOT)
	{
		map[nextY][nextX] = SPACE;
		--dots;
	}

	// Block movement when meets wall
	if (map[nextY][nextX] != WALL)
	{
		gotoxy(player->x, player->y);
		_putch(getMapTile(player->x, player->y));
		gotoxy(nextX, nextY);
		_putch(PLAYER);

		player->x = nextX;
		player->y = nextY;
	}
}

Ghost* createGhost(int x, int y, int deltaX, int deltaY, char model)
{
	Ghost* newGhost = (Ghost*)malloc(sizeof(Ghost));
	newGhost->x = x;
	newGhost->y = y;
	newGhost->deltaX = deltaX;
	newGhost->deltaY = deltaY;
	newGhost->model = model;
	newGhost->lastUpdated = 0;

	// Check if ghost array allocated
	if (ghosts)
	{
		// Re allocate memory with the size (ghostCount + 1)
		ghosts = (Ghost**)realloc(ghosts, sizeof(Ghost*) * (ghostCount + 1));
	}
	else
	{
		ghosts = (Ghost**)malloc(sizeof(Ghost*) * (ghostCount + 1));
	}

	ghosts[ghostCount++] = newGhost;
	gotoxy(newGhost->x, newGhost->y);
	_putch(newGhost->model);

	return newGhost;
}

void updateGhost(Ghost* ghost)
{
	// Update the ghost position in each 0.5 second since the last time updated
	if (ghost->lastUpdated + 500 < gameMileseconds)
	{
		ghost->lastUpdated = gameMileseconds;
		int enabled[4] = { 0, 0, 0, 0 }; // In enum Arrows order, LEFT, RIGHT, UP, DOWN

		// Search for a new direction when next position is blocked
		for (int i = -1; i <= 1; i += 2)
		{
			// Clamp coordinates
			int nextX = ghost->x;
			int nextY = clamp(ghost->y + i, ROW);

			if (map[nextY][nextX] != WALL)
			{
				// Toggle direction flag
				enabled[i < 0 ? 2 : 3] = 1;
			}
		}

		for (int i = -1; i <= 1; i += 2)
		{
			// Clamp coordinates
			int nextX = clamp(ghost->x + i, COL);
			int nextY = ghost->y;

			if (map[nextY][nextX] != WALL)
			{
				// Toggle direction flag
				enabled[i < 0 ? 0 : 1] = 1;
			}
		}

		// Disables the backward direction from current delta, if current delta is progressable
		// This prevents the ghosts moving forward-backward continuously, which makes AI movement looks stupid
		int order = directionToFlatOrder(ghost->deltaX, ghost->deltaY);
		int selDirection = random(4);

		// When current direction is progressable
		if (enabled[order])
		{
			// Disables backward movement (no matter the direction is progressable)
			enabled[2 * (order / 2) + (1 - (order % 2))] = 0;
		}

		// Selects a random direction among possible directions
		// Loops until meets enabled direction
		while (!enabled[selDirection]) selDirection = random(4);

		if (enabled[selDirection])
		{
			switch (selDirection) {
			case 0:
				ghost->deltaX = -1;
				ghost->deltaY = 0;
				break;
			case 1:
				ghost->deltaX = 1;
				ghost->deltaY = 0;
				break;
			case 2:
				ghost->deltaX = 0;
				ghost->deltaY = -1;
				break;
			case 3:
				ghost->deltaX = 0;
				ghost->deltaY = 1;
				break;
			}
		}

		// Replace old position to tile
		gotoxy(ghost->x, ghost->y);
		_putch(getMapTile(ghost->x, ghost->y));

		// Update position
		ghost->x = clamp(ghost->x + ghost->deltaX, COL);
		ghost->y = clamp(ghost->y + ghost->deltaY, ROW);

		// Draw ghost in new position
		gotoxy(ghost->x, ghost->y);
		_putch(ghost->model);
	}
}