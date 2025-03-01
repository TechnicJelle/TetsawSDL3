#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "myappstate.h"
#include "board.h"

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	SDL_SetAppMetadata("Tetsaw", "1.0", "com.technicjelle.tetsaw");

	MyAppState* myAppState = SDL_malloc(sizeof(MyAppState));

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Tetsaw", 1280, 720, SDL_WINDOW_RESIZABLE, &myAppState->window, &myAppState->renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	myAppState->BOARD_PADDING = 48;

	myAppState->BOARD_COLUMNS = 10;
	myAppState->BOARD_ROWS = 10;
	myAppState->PUZZLE_PIECES_COUNT = 8;

	myAppState->globalCellSize = -1;

	*appstate = (void*) myAppState;

	// mouseposrect.x = mouseposrect.y = -1000; // -1000 so it's offscreen at start
	// mouseposrect.w = mouseposrect.h = 50;

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	MyAppState* myAppState = (MyAppState*) appstate;

	CalculateGlobalCellSize(myAppState);

	SDL_SetRenderDrawColor(myAppState->renderer, 69, 9, 122, 255);
	SDL_RenderClear(myAppState->renderer);

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

	DrawBoard(myAppState);

	// put everything we drew to the screen.
	SDL_RenderPresent(myAppState->renderer);

	return SDL_APP_CONTINUE;
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
	const MyAppState* myAppState = (MyAppState*) appstate;

	SDL_DestroyRenderer(myAppState->renderer);
	SDL_DestroyWindow(myAppState->window);
	SDL_Quit();
}
