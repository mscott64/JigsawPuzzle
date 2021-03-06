#include "Joined.h"
#include "Piece.h"

Joined::Joined(Piece *p) {
	mCenter = new Coord();
	mRotateAngle = p->getRotateAngle();
	mFlipAngleX = p->getFlipAngleX();
	mFlipAngleY = p->getFlipAngleY();
	addPiece(p);
	computeCenter();
}

void Joined::computeCenter() {
	float total_x=0.0f, total_z=0.0f; // already share a y
	unsigned int num_pieces = mPieces.size();
	for(unsigned int i = 0; i < num_pieces; i++) {
		Coord *pos = mPieces[i]->getPos(false);
		total_x += pos->mx;
		total_z += pos->mz;
	}
	mCenter->mx = total_x / num_pieces;
	mCenter->my = mPieces[0]->getPos(false)->my;
	mCenter->mz = total_z / num_pieces;
}

void Joined::addPiece(Piece *p) { // don't add duplicates
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		if(mPieces[i] == p)
			return;
		}
	mPieces.push_back(p); 
}

void Joined::flipX() {
	if(mFlipAngleX > 0.0f)
		mFlipAngleX -= 180.0f;
	else
		mFlipAngleX += 180.0f;
}

void Joined::flipY() {
	if(mFlipAngleY > 0.0f)
		mFlipAngleY -= 180.0f;
	else
		mFlipAngleY += 180.0f;
}

void Joined::rotate(float angle) {
	mRotateAngle += angle;
	// Keep angles in range [0, 360)
	if(mRotateAngle >= 360.0f)
		mRotateAngle -= 360.0f;
	if(mRotateAngle < 0)
		mRotateAngle += 360.0f;
}