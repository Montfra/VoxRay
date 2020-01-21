#include "hud.h"

hud::hud(SDL_Renderer* renderer) : score(renderer, "background.bmp", { 0, 0, 320, 240 }), keyboard(renderer, "background.bmp", { (1280 / 2) - 320 / 2, 720 - (240 / 2), 320, 240 }) {}

void hud::print(SDL_Renderer* renderer) {
	score.print(renderer);
	keyboard.print(renderer);
}