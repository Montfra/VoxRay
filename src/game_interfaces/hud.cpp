#include "hud.h"

hud::hud(SDL_Renderer* renderer) : score(renderer, resourcePath("score.bmp").c_str(), &r1),
a(renderer, resourcePath("a.bmp").c_str(), &ra),
aA(renderer, resourcePath("aA.bmp").c_str(), &ra),
e(renderer, resourcePath("e.bmp").c_str(), &re),
eE(renderer, resourcePath("eE.bmp").c_str(), &re),
z(renderer, resourcePath("z.bmp").c_str(), &rz),
zZ(renderer, resourcePath("zZ.bmp").c_str(), &rz),
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