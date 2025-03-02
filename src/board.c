#include "board.h"

/// Also calculates the globalCellSize
void DrawBoardAndTray(MyAppState* appState) {
	//get safe area to draw in (phone status bars etc could be overlapping)
	SDL_Rect safeRect;
	SDL_GetWindowSafeArea(appState->window, &safeRect);
	if ((float) safeRect.y < appState->BOARD_PADDING) {
		safeRect.y = (int) appState->BOARD_PADDING;
	}

	int width, height;
	SDL_GetWindowSize(appState->window, &width, &height);
	const float windowAspectRatio = (float) width / (float) height;

	const int cols = appState->BOARD_COLUMNS;
	const int rows = appState->BOARD_ROWS;

	//horizontally stacked
	const int horizontallyStackedCols = cols * 2 + 1; //+1 for the gap
	const int horizontallyStackedRows = rows;
	const float horizontallyStackedBoardAspectRatio = (float) horizontallyStackedCols / (float) horizontallyStackedRows;

	//vertically stacked
	const int verticallyStackedCols = cols;
	const int verticallyStackedRows = rows * 2 + 1; //+1 for the gap
	const float verticallyStackedBoardAspectRatio = (float) verticallyStackedCols / (float) verticallyStackedRows;

	//determine which aspect ratio to use
	const float distanceFromHorizontallyStacked = SDL_fabsf(windowAspectRatio - horizontallyStackedBoardAspectRatio);
	const float distanceFromVerticallyStacked = SDL_fabsf(windowAspectRatio - verticallyStackedBoardAspectRatio);
	const bool useHorizontallyStacked = distanceFromHorizontallyStacked < distanceFromVerticallyStacked;

	//calculate play area columns and rows
	int playAreaColumns;
	int playAreaRows;
	if (useHorizontallyStacked) {
		//use horizontally stacked
		playAreaColumns = horizontallyStackedCols;
		playAreaRows = horizontallyStackedRows;
	} else {
		//use vertically stacked
		playAreaColumns = verticallyStackedCols;
		playAreaRows = verticallyStackedRows;
	}

	//calculate global cell size
	const float playAreaX = (float) width - appState->BOARD_PADDING * 2.0f;
	const float playAreaY = (float) height - appState->BOARD_PADDING * 2.0f;
	const float potentialWidth = playAreaX / (float) playAreaColumns;
	const float potentialHeight = playAreaY / (float) playAreaRows;
	appState->globalCellSize = SDL_min(potentialWidth, potentialHeight);

	//calculate board size
	const SDL_FRect boardRect = {
		(float) safeRect.x + ((float) width - appState->globalCellSize * (float) playAreaColumns) / 2.0f,
		(float) safeRect.y,
		appState->globalCellSize * (float) appState->BOARD_COLUMNS,
		appState->globalCellSize * (float) appState->BOARD_ROWS
	};

	//draw board
	SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(appState->renderer, &boardRect);

	//calculate tray size
	SDL_FRect tray = {
		0.0f,
		0.0f,
		boardRect.w,
		boardRect.h
	};
	if (useHorizontallyStacked) {
		tray.x = boardRect.x + appState->globalCellSize + boardRect.w;
		tray.y = boardRect.y;
	} else {
		tray.x = boardRect.x;
		tray.y = boardRect.y + appState->globalCellSize + boardRect.h;
	}

	//draw tray
	SDL_SetRenderDrawBlendMode(appState->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(appState->renderer, 100, 100, 100, 100);
	SDL_RenderFillRect(appState->renderer, &tray);
	SDL_SetRenderDrawBlendMode(appState->renderer, SDL_BLENDMODE_NONE);

	//calculate grid line thickness
	const float thickness = SDL_max(appState->globalCellSize / 50.0f, 2.0f);

	//draw grid
	SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, 255);
	// vertical lines
	for (int col = 0; col < (appState->BOARD_COLUMNS + 1); col++) {
		SDL_FRect line = {
			boardRect.x + (float) col * appState->globalCellSize - thickness / 2,
			boardRect.y,
			thickness,
			appState->globalCellSize * (float) appState->BOARD_ROWS
		};
		SDL_RenderFillRect(appState->renderer, &line);
	}

	// horizontal lines
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
