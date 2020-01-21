#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer) : playButton(renderer, "playButton.bmp", { 1280 / 2 - 300 / 2, 100 / 2 + 400, 300, 100 }), title(renderer, "voxRayLogo.bmp", { 1280 / 2 - 640 / 2, 20, 640, 240 }), isActive(true) {}

bool Menu::getActive() { return isActive; }

void Menu::print(SDL_Renderer* renderer){
	title.print(renderer);
	playButton.print(renderer);
}

void Menu::setInactive() { isActive = false; }

