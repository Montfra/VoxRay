#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer) : playButton(renderer, "playButton.bmp", &r1), title(renderer, "voxRayLogo.bmp", &r2), isActive(true) {}

bool Menu::getActive() { return isActive; }

void Menu::print(SDL_Renderer* renderer){
	title.print(renderer);
	playButton.print(renderer);
}

void Menu::setInactive() { isActive = false; }

