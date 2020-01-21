#pragma once
#include "image.h"

class hud
{
private:
	Image score;
	Image keyboard;

public:
	hud(SDL_Renderer*);
	void print(SDL_Renderer*);
};

