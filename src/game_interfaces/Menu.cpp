#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer) : playButton(renderer, resourcePath("playButton.bmp").c_str(), &r1), title(renderer, resourcePath("voxRayLogo.bmp").c_str(), &r2), isActive(true) {}

bool Menu::getActive() { return isActive; }

void Menu::print(SDL_Renderer* renderer){
	title.print(renderer);
	playButton.print(renderer);
}

void Menu::setInactive() { isActive = false; }

