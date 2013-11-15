#ifndef PIECE_H
#define PIECE_H

#include "Coord.h"
#include <vector>

class Joined;

enum {ABOVE, BELOW, LEFT, RIGHT};

class Piece {
public:
	Piece(int id);
	~Piece();
	void draw();
	void setPos(float x, float y, float z);
	void setTextureBounds(float lx, float ly, float hx, float hy);
	Coord *getPos() { return mPos; };
	void setSize(float w, float h) { mw = w/2.0f; mh = h/2.0f; }
	float getWidth() { return mw * 2.0f; }
	float getHeight() { return mh * 2.0f; }
	int getID() { return mId; }
	void move(float dx, float dy, float dz);
	void addNeighbor(Piece *p) { mNeighbors.push_back(p); }
	void removeNeighbor(int i) { mNeighbors.erase(mNeighbors.begin()+i); mDirections.erase(mDirections.begin()+i);}
	void removeNeighbor(Piece *p);
	void addDirection(int dir) { mDirections.push_back(dir); }
	unsigned int getNumNeighbors() { return mNeighbors.size(); }
	Piece *getNeighbor(int i) { return mNeighbors[i]; }
	int getDirection(int i) { return mDirections[i]; }
	Joined *mJoined;

private:
	int mId;
	float mw, mh; // half of width and height
	Coord *mPos;
	Coord *mLow, *mHigh;
	std::vector<Piece *> mNeighbors;
	std::vector<int> mDirections;
};

#endif /* Piece.h */