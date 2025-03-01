#pragma once

#include <SDL3/SDL.h>

typedef struct MyAppState {
	SDL_Window* window;
	SDL_Renderer* renderer;

	//Play Area: big grid, around the board
	//Board: subsection of the play area

	//Visual constants
	float BOARD_PADDING;

	//Gameplay variables
	int BOARD_COLUMNS;
	int BOARD_ROWS;
	int PUZZLE_PIECES_COUNT;

	float globalCellSize;
} MyAppState;

void CalculateGlobalCellSize(MyAppState* appState);
