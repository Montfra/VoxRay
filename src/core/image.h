#pragma once
#include <SDL.h>
class Image
{

private:
	SDL_Surface* image;
	SDL_Texture* texture;
	SDL_Rect* dstrect;

	SDL_Texture* score;
	SDL_Texture* score1;
	SDL_Texture* score10;
	SDL_Texture* score100;
	SDL_Texture* score1000;
	SDL_Texture* score10000;
	SDL_Texture* score100000;

	SDL_Rect digit;
	SDL_Rect dix;
	SDL_Rect cent;
	SDL_Rect mille;
	SDL_Rect dixMille;
	SDL_Rect centMille;

	SDL_Rect d1;
	SDL_Rect d10;
	SDL_Rect d100;
	SDL_Rect d1000;
	SDL_Rect d10000;
	SDL_Rect d100000;

	bool isDigit;
	

public:
	Image(SDL_Renderer* render, const char* imagePath, SDL_Rect* rect);
	void print(SDL_Renderer*);
	void setValue(int);
};

