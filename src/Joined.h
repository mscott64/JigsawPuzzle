#ifndef JOINED_H
#define JOINED_H

#include <vector>
#include "Group.h"
class Piece;
class Coord;

class Joined : public Group {
public:
	Joined(Piece *p);
	void addPiece(Piece *p);
	Coord *getCenter() { return mCenter; }
	float getRotateAngle() { return mRotateAngle; }
	float getFlipAngleX() { return mFlipAngleX; }
	float getFlipAngleY() { return mFlipAngleY; }
	void rotate(float angle);
	void computeCenter();
	void flipX();
	void flipY();

private:
	Coord *mCenter;
	float mRotateAngle;
	float mFlipAngleX;
	float mFlipAngleY;
};

#endif /* Joined.h */