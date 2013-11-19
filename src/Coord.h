#ifndef COORD_H
#define COORD_H

#include <math.h>

class Coord {
public:
	float mx, my, mz;
	Coord() { setCoord(0.0f,0.0f,0.0f); }
	Coord(float x, float y, float z) { setCoord(x,y,z); }
	void setCoord(float x, float y, float z) { mx = x; my = y; mz = z; }
	static bool areTogether(Coord *p1, Coord *p2) { return abs(p1->mx - p2->mx) < 1e-1 && abs(p1->mz - p2->mz) < 1e-1; }
};

#endif /* Coord.h */