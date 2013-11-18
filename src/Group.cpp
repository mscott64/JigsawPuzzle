#include "Group.h"
#include "Coord.h"
#include "Piece.h"
#include "Joined.h"

#define CONV (3.14159265f / 180.0f)

Group::Group(Piece *p) {
	axis = new Coord();
	Coord *pos;
	if(p->mJoined == NULL) 
		pos = p->getPos(false);
	else 
		pos = p->mJoined->getCenter();
	
	axis->mx = pos->mx;
	axis->mz = pos->mz;
	isStacked = true;

	addPiece(p); 
}

void Group::addPiece(Piece *p) { // don't add duplicates
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		if(mPieces[i] == p)
			return;
	}
	mPieces.push_back(p); 
	Coord *c;
	if(p->mJoined == NULL) {
		c = p->getPos(false);
		p->mGroup = this;
	} else {
		Joined *join = p->mJoined;
		c = join->getCenter();
		for(unsigned int i = 0; i < join->getNumPieces(); i++)
			join->getPiece(i)->mGroup = this;
	}
	
	Coord *d = new Coord(axis->mx - c->mx, 1.0f + mPieces.size() * 0.05f - c->my, axis->mz - c->mz);
	p->moveAnimated(d->mx, d->my, d->mz);
}

void Group::removePiece(Piece *p) {
	unsigned int index;
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		if(mPieces[i] == p) {
			mPieces[i]->mGroup = NULL;
			index = i;
			mPieces.erase(mPieces.begin()+i);
			break;
		}
	}

	// move everything else down
	for(unsigned int i = index; i < mPieces.size(); i++) {
		mPieces[i]->move(0.0f, -0.05f, 0.0f);
	}
}

void Group::fan() {
	if(!isStacked) {
		stack();
		return;
	}
	float deg = 360.0f / mPieces.size();
	float x,y,tx,ty;
	x = 0.0f; y = -1.0f;
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		mPieces[i]->moveAnimated(x, 0.0f, y);
		tx = x; ty = y;
		x = tx * cos(deg * CONV) - ty * sin(deg * CONV);
		y = tx * sin(deg * CONV) + ty * cos(deg * CONV);
	}
	isStacked = false;
}

void Group::stack() {
	Piece *p;
	Coord *c, *d;
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		p = mPieces[i];
		if(p->mJoined == NULL) {
			c = p->getPos(false);
		} else {
			c = p->mJoined->getCenter();
		}
		d = new Coord(axis->mx - c->mx, 1.05f + i * 0.05f - c->my, axis->mz - c->mz);
		p->moveAnimated(d->mx, d->my, d->mz);
	}
	isStacked = true;
}
