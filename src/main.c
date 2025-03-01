#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "draw_utils.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

//Play Area: big grid, around the board
//Board: subsection of the play area

//Visual constants
static const float BOARD_PADDING = 48;

//Gameplay variables
static int BOARD_COLUMNS = 10;
static int BOARD_ROWS = 10;

static float globalCellSize = -1;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	SDL_SetAppMetadata("Tetsaw", "1.0", "com.technicjelle.tetsaw");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Tetsaw", 1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// mouseposrect.x = mouseposrect.y = -1000; // -1000 so it's offscreen at start
	// mouseposrect.w = mouseposrect.h = 50;

	return SDL_APP_CONTINUE;
}

void CalculateGlobalCellSize() {
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	const float playAreaX = (float) width - BOARD_PADDING * 2;
	const float playAreaY = (float) height - BOARD_PADDING * 2;
	const float potentialWidth = playAreaX / (float) BOARD_COLUMNS;
	const float potentialHeight = playAreaY / (float) BOARD_ROWS;
	globalCellSize = SDL_min(potentialWidth, potentialHeight);
}

void DrawBoard() {
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	const float boardWidth = globalCellSize * (float) BOARD_COLUMNS;
	const float boardHeight = globalCellSize * (float) BOARD_ROWS;
	const SDL_FRect boardRect = {((float)width - globalCellSize * (float)BOARD_COLUMNS) / 2.0f, BOARD_PADDING, boardWidth, boardHeight};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &boardRect);

	const float thickness = SDL_max(globalCellSize / 50, 2);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//vertical lines
	for (int col = 0; col < (BOARD_COLUMNS + 1); col++) {
		SDL_FRect line = {
			boardRect.x + (float) col * globalCellSize - thickness / 2,
			boardRect.y,
			thickness,
			globalCellSize * (float) BOARD_ROWS
		};
		SDL_RenderFillRect(renderer, &line);
	}

	//horizontal lines
	for (int row = 0; row < (BOARD_ROWS + 1); row++) {
		SDL_FRect line = {
			boardRect.x,
			boardRect.y + (float) row * globalCellSize - thickness / 2,
			globalCellSize * (float) BOARD_COLUMNS,
			thickness
		};
		SDL_RenderFillRect(renderer, &line);
	}
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	CalculateGlobalCellSize();

	SDL_SetRenderDrawColor(renderer, 69, 9, 122, 255);
	SDL_RenderClear(renderer);

	//Draw Cells
	// for (int col = 0; col < BOARD_COLUMNS; col++) {
	// 	for (int row = 0; row < BOARD_ROWS; row++) {
	// 		int width, height;
	// 		SDL_GetWindowSize(window, &width, &height);
	// 		float x = ((float)width - globalCellSize * (float)BOARD_COLUMNS)/2 + globalCellSize * (float)col;
	// 		float y = globalCellSize * (float)row + BOARD_PADDING;
	// 		SDL_FRect rect = {x, y, globalCellSize, globalCellSize};
	//
	// 		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// 		SDL_RenderFillRect(renderer, &rect);
	//
	// 		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// 		DrawRectOutline(renderer, &rect, thickness);
	// 	}
	// }

	DrawBoard();

	// put everything we drew to the screen.
	SDL_RenderPresent(renderer);

	return
			SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT: // triggers on last window close and other things. End the program.
			return SDL_APP_SUCCESS;

		case SDL_EVENT_KEY_DOWN: // quit if user hits ESC key
			if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
				return SDL_APP_SUCCESS;
			}
			break;

		case SDL_EVENT_MOUSE_MOTION: // keep track of the latest mouse position
			// mouseposrect.x = event->motion.x - (mouseposrect.w / 2);
			// mouseposrect.y = event->motion.y - (mouseposrect.h / 2);
			break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
