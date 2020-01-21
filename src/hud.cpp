#include "hud.h"

hud::hud(SDL_Renderer* renderer) : score(renderer, "score.bmp", &r1),
a(renderer, "a.bmp", &ra),
aA(renderer, "aA.bmp", &ra),
e(renderer, "e.bmp", &re),
eE(renderer, "eE.bmp", &re),
z(renderer, "z.bmp", &rz),
zZ(renderer, "zZ.bmp", &rz),
ba(false), bz(false), be(false) {
	score.setValue(0);
}

void hud::setA() { ba = true; }
void hud::setZ() { bz = true; }
void hud::setE() { be = true; }

void hud::dsetA() { ba = false; }
void hud::dsetZ() { bz = false; }
void hud::dsetE() { be = false; }

void hud::setScore(int s) { score.setValue(s); }

void hud::print(SDL_Renderer* renderer) {
	score.print(renderer);

	if (ba){
		aA.print(renderer);
	}
	else {
		a.print(renderer);
	}

	if (bz) {
		zZ.print(renderer);
	}
	else {
		z.print(renderer);
	}

	if (be) {
		eE.print(renderer);
	}
	else {
		e.print(renderer);
	}
}