#include "hud.h"

hud::hud(SDL_Renderer* renderer) : score(renderer, "../ressources/score.bmp", &r1),
a(renderer, "../ressources/a.bmp", &ra),
aA(renderer, "../ressources/aA.bmp", &ra),
e(renderer, "../ressources/e.bmp", &re),
eE(renderer, "../ressources/eE.bmp", &re),
z(renderer, "../ressources/z.bmp", &rz),
zZ(renderer, "../ressources/zZ.bmp", &rz),
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