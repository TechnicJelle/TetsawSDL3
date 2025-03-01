#include "myappstate.h"

void CalculateGlobalCellSize(MyAppState* appState) {
	int width, height;
	SDL_GetWindowSize(appState->window, &width, &height);
	const float playAreaX = (float) width - appState->BOARD_PADDING * 2;
	const float playAreaY = (float) height - appState->BOARD_PADDING * 2;
	const float potentialWidth = playAreaX / (float) appState->BOARD_COLUMNS;
	const float potentialHeight = playAreaY / (float) appState->BOARD_ROWS;
	appState->globalCellSize = SDL_min(potentialWidth, potentialHeight);
}
