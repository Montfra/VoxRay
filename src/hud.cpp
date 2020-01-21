#include "hud.h"

hud::hud(SDL_Renderer* renderer) : score(renderer, "score.bmp", { 0, 0, 320, 240 }),
a(renderer, "a.bmp", { (1280 / 2) - 129, 720 - 60, 129, 60 }),
aA(renderer, "aA.bmp", { (1280 / 2) - 129, 720 - 60, 129, 60 }),
e(renderer, "e.bmp", { (1280 / 2), 720 - 60, 129, 60 }),
eE(renderer, "eE.bmp", { (1280 / 2), 720 - 60, 129, 60 }),
z(renderer, "z.bmp", { (1280 / 2) - 129 / 2, 720 - 60, 129, 60 }),
zZ(renderer, "zZ.bmp", { (1280 / 2) - 129 / 2, 720 - 60, 129, 60 }),
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