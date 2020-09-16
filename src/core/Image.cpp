#include "image.h"

Image::Image(SDL_Renderer* render, const char* imagePath, SDL_Rect* rect) :
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

    digit.x = 420;
    digit.y = 0;
    digit.w = 66;
    digit.h = 63;

    dix.x = 530;
    dix.y = 0;
    dix.w = 66;
    dix.h = 63;

    cent.x = 640;
    cent.y = 0;
    cent.w = 66;
    cent.h = 63;

    mille.x = 750;
    mille.y = 0;
    mille.w = 66;
    mille.h = 63;

    dixMille.x = 860;
    dixMille.y = 0;
    dixMille.w = 66;
    dixMille.h = 63;

    centMille.x = 970;
    centMille.y = 0;
    centMille.w = 66;
    centMille.h = 63;

    d1.x = 420;
    d1.y = 0;
    d1.w = 66;
    d1.h = 63;

    d10.x = 530;
    d10.y = 0;
    d10.w = 66;
    d10.h = 63;

    d100.x = 640;
    d100.y = 0;
    d100.w = 66;
    d100.h = 63;

    d1000.x = 750;
    d1000.y = 0;
    d1000.w = 66;
    d1000.h = 63;

    d10000.x = 860;
    d10000.y = 0;
    d10000.w = 66;
    d10000.h = 63;

    d100000.x = 970;
    d100000.y = 0;
    d100000.w = 66;
    d100000.h = 63;
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
		SDL_RenderCopy(renderer, texture, NULL, dstrect);
	}
}

void Image::setValue(int val) {
	isDigit = true;
	int decal = 0;
	if (val / 100000 == 0) {
        digit.x = 170;
        digit.y = 0;
        digit.w = 66;
        digit.h = 63;
	}
	else {
		decal = ((val / 100000) - 1) * 110 + 420;
        digit.x = decal;
        digit.y = 0;
        digit.w = 66;
        digit.h = 63;
		val = val - 100000 * (val / 100000);
	}

	if (val / 10000 == 0) {
        dix.x = 170;
        dix.y = 0;
        dix.w = 66;
        dix.h = 63;
	}
	else {
		decal = ((val / 10000) - 1) * 110 + 420;
        dix.x = decal;
        dix.y = 0;
        dix.w = 66;
        dix.h = 63;
		val = val - 10000 * (val / 10000);
	}

	if (val / 1000 == 0) {
        cent.x = 170;
        cent.y = 0;
        cent.w = 66;
        cent.h = 63;
	}
	else {
		decal = ((val / 1000) - 1) * 110 + 420;
        cent.x = decal;
        cent.y = 0;
        cent.w = 66;
        cent.h = 63;
		val = val - 1000 * (val / 1000);
	}

	if (val / 100 == 0) {
        mille.x = 170;
        mille.y = 0;
        mille.w = 66;
        mille.h = 63;
	}
	else {
		decal = ((val / 100) - 1) * 110 + 420;
        mille.x = decal;
        mille.y = 0;
        mille.w = 66;
        mille.h = 63;
		val = val - 100 * (val / 100);
	}

	if (val / 10 == 0) {
        dixMille.x = 170;
        dixMille.y = 0;
        dixMille.w = 66;
        dixMille.h = 63;
	}
	else {
		decal = ((val / 10) - 1) * 110 + 420;
        dixMille.x = decal;
        dixMille.y = 0;
        dixMille.w = 66;
        dixMille.h = 63;
		val = val - 10 * (val / 10);
	}

	if (val / 1 == 0) {
        centMille.x = 170;
        centMille.y = 0;
        centMille.w = 66;
        centMille.h = 63;
	}
	else {
		decal = ((val / 1) - 1)  * 110 + 420;
        centMille.x = decal;
        centMille.y = 0;
        centMille.w = 66;
        centMille.h = 63;
	}

}