#pragma once
#include <SDL.h>
class Image
{

private:
	SDL_Surface* image;
	SDL_Texture* texture;
	SDL_Rect dstrect;

public:
	Image(SDL_Renderer* render, const char* imagePath, SDL_Rect rect);
	void print(SDL_Renderer*);
};

