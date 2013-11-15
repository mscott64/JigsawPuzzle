#ifndef COORD_H
#define COORD_H

class Coord {
public:
	Coord() { setCoord(0.0f,0.0f,0.0f); }
	Coord(float x, float y, float z) { setCoord(x,y,z); }
	void setCoord(float x, float y, float z) { mx = x; my = y; mz = z; }
	float mx, my, mz;
};

#endif /* Coord.h */