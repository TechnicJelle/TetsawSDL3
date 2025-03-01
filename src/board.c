#include "board.h"

void DrawBoard(const MyAppState* appState) {
	int width, height;
	SDL_GetWindowSize(appState->window, &width, &height);
	const float boardWidth = appState->globalCellSize * (float) appState->BOARD_COLUMNS;
	const float boardHeight = appState->globalCellSize * (float) appState->BOARD_ROWS;
	const SDL_FRect boardRect = {((float)width - appState->globalCellSize * (float)appState->BOARD_COLUMNS) / 2.0f, appState->BOARD_PADDING, boardWidth, boardHeight};
	SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(appState->renderer, &boardRect);

	const float thickness = SDL_max(appState->globalCellSize / 50, 2);
	SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, 255);

	//vertical lines
	for (int col = 0; col < (appState->BOARD_COLUMNS + 1); col++) {
		SDL_FRect line = {
			boardRect.x + (float) col * appState->globalCellSize - thickness / 2,
			boardRect.y,
			thickness,
			appState->globalCellSize * (float) appState->BOARD_ROWS
		};
		SDL_RenderFillRect(appState->renderer, &line);
	}

	//horizontal lines
	for (int row = 0; row < (appState->BOARD_ROWS + 1); row++) {
		SDL_FRect line = {
			boardRect.x,
			boardRect.y + (float) row * appState->globalCellSize - thickness / 2,
			appState->globalCellSize * (float) appState->BOARD_COLUMNS,
			thickness
		};
		SDL_RenderFillRect(appState->renderer, &line);
	}
}
