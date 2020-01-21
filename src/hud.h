#pragma once
#include "image.h"

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

