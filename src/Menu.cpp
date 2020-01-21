#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer) : playButton(renderer, "background.bmp", { 1280 / 2 - 320 / 2, 720 / 2, 320, 240 }), title(renderer, "background.bmp", { 1280 / 2 - 320 / 2, 20, 320, 240 }), isActive(true) {}

bool Menu::getActive() { return isActive; }

void Menu::print(SDL_Renderer* renderer){
	title.print(renderer);
	playButton.print(renderer);
}

void Menu::setInactive() { isActive = false; }

