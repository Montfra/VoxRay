#pragma once
#include "../core/image.h"

class hud
{
private:
	Image score;
	Image a;
	Image aA;
	Image z;
	Image zZ;
	Image e;
	Image eE;
	bool ba;
	bool bz;
	bool be;
	SDL_Rect r1 = { 0, 0, 320, 240 };
	SDL_Rect ra = { (1280 / 2) - 129, 720 - 60, 129, 60 };
	SDL_Rect rz  = { (1280 / 2) - 129 / 2, 720 - 60, 129, 60 };
	SDL_Rect re = { (1280 / 2), 720 - 60, 129, 60 };

public:
	hud(SDL_Renderer*);
	void setA();
	void setZ();
	void setE();
	void dsetA();
	void dsetZ();
	void dsetE();
	void setScore(int);
	void print(SDL_Renderer*);
};

