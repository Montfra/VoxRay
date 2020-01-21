#include "Image.h"

Image::Image(SDL_Renderer* render, const char* imagePath, SDL_Rect rect) :
	dstrect(rect)
{
	image = SDL_LoadBMP(imagePath);
	texture = SDL_CreateTextureFromSurface(render, image);
}



void Image::print(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}