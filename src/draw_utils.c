#include "draw_utils.h"

void DrawRectOutline(SDL_Renderer* renderer, const SDL_FRect* rect, const float thickness) {
	SDL_SetRenderScale(renderer, thickness, thickness);
	const SDL_FRect scaledRect = {rect->x / thickness, rect->y / thickness, rect->w / thickness, rect->h / thickness};
	SDL_RenderRect(renderer, &scaledRect);
	SDL_SetRenderScale(renderer, 1, 1);
}
