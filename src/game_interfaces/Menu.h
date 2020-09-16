#pragma once
#include "../core/image.h"
#pragma once

class Menu
{
private:
	Image playButton;
	Image title;
	SDL_Rect r1 = { 1280 / 2 - 300 / 2, 100 / 2 + 400, 300, 100 };
	SDL_Rect r2 = { 1280 / 2 - 640 / 2, 20, 640, 240 };
	bool isActive;
public:
	Menu(SDL_Renderer*);
	void print(SDL_Renderer*);
	bool getActive();
	void setInactive();
};

