#include "Image.h"

Image::Image(SDL_Renderer* render, const char* imagePath, SDL_Rect rect) :
	dstrect(rect), isDigit(false)
{
	image = SDL_LoadBMP(imagePath);
	texture = SDL_CreateTextureFromSurface(render, image);
	score = SDL_CreateTextureFromSurface(render, image);
	score1 = SDL_CreateTextureFromSurface(render, image);
	score10 = SDL_CreateTextureFromSurface(render, image);
	score100 = SDL_CreateTextureFromSurface(render, image);
	score1000 = SDL_CreateTextureFromSurface(render, image);
	score10000 = SDL_CreateTextureFromSurface(render, image);
	score100000 = SDL_CreateTextureFromSurface(render, image);


	SDL_Texture* score;
	SDL_Texture* score1;
	SDL_Texture* score10;
	SDL_Texture* score100;
	SDL_Texture* score1000;
	SDL_Texture* score10000;
	SDL_Texture* score100000;

	digit = {420, 0, 66, 63};
	dix = { 530, 0, 66, 63 };
	cent = { 640, 0, 66, 63 };
	mille = { 750, 0, 66, 63 };
	dixMille = { 860, 0, 66, 63 };
	centMille = { 970, 0, 66, 63 };

	d1 = { 420, 0, 66, 63 };
	d10 = { 530, 0, 66, 63 };
	d100 = { 640, 0, 66, 63 };
	d1000 = { 750, 0, 66, 63 };
	d10000 = { 860, 0, 66, 63 };
	d100000 = { 970, 0, 66, 63 };

	bool isDigit;
}



void Image::print(SDL_Renderer* renderer) {
	if (isDigit) {
		SDL_Rect c = { 0, 0, 420, 63 };
		SDL_Rect c1 = { 0, 0, 420, 63 };
		SDL_RenderCopy(renderer, score, &c, &c1);
		SDL_RenderCopy(renderer, score1, &digit, &d1);
		SDL_RenderCopy(renderer, score10, &dix, &d10);
		SDL_RenderCopy(renderer, score100, &cent, &d100);
		SDL_RenderCopy(renderer, score1000, &mille, &d1000);
		SDL_RenderCopy(renderer, score10000, &dixMille, &d10000);
		SDL_RenderCopy(renderer, score100000, &centMille, &d100000);
	}
	else {
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	}
}

void Image::setValue(int val) {
	isDigit = true;
	int decal = 0;
	if (val / 100000 == 0) {
		digit = { 170, 0, 66, 63 };
	}
	else {
		decal = ((val / 100000) - 1) * 110 + 420;
		digit = { decal, 0, 66, 63 };
		val = val - 100000 * (val / 100000);
	}

	if (val / 10000 == 0) {
		dix = { 170, 0, 66, 63 };
	}
	else {
		decal = ((val / 10000) - 1) * 110 + 420;
		dix = { decal, 0, 66, 63 };
		val = val - 10000 * (val / 10000);
	}

	if (val / 1000 == 0) {
		cent = { 170, 0, 66, 63 };
	}
	else {
		decal = ((val / 1000) - 1) * 110 + 420;
		cent = { decal, 0, 66, 63 };
		val = val - 1000 * (val / 1000);
	}

	if (val / 100 == 0) {
		mille = { 170, 0, 66, 63 };
	}
	else {
		decal = ((val / 100) - 1) * 110 + 420;
		mille = { decal, 0, 66, 63 };
		val = val - 100 * (val / 100);
	}

	if (val / 10 == 0) {
		dixMille = { 170, 0, 66, 63 };
	}
	else {
		decal = ((val / 10) - 1) * 110 + 420;
		dixMille = { decal, 0, 66, 63 };
		val = val - 10 * (val / 10);
	}

	if (val / 1 == 0) {
		centMille = { 170, 0, 66, 63 };
	}
	else {
		decal = ((val / 1) - 1)  * 110 + 420;
		centMille = {decal, 0, 66, 63};
	}

}