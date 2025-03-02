#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;

	//Board: grid of cells. This is where the pieces are moved to.
	//Tray: grid of cells, next to the board. This is where the pieces spawn (overlapped is fine)
	//Play Area: board + tray

	//Visual constants
	float BOARD_PADDING;

	//Gameplay variables
	int BOARD_COLUMNS;
	int BOARD_ROWS;
	int PUZZLE_PIECES_COUNT;

	float globalCellSize;
} MyAppState;
