#pragma once
#include "image.h"
#pragma once

class Menu
{
private:
	Image playButton;
	Image title;
	bool isActive;
public:
	Menu(SDL_Renderer*);
	void print(SDL_Renderer*);
	bool getActive();
	void setInactive();
};

