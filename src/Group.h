#ifndef GROUP_H
#define GROUP_H

#include <vector>
class Piece;
class Coord;

class Group {
public:
	Group() {};
	Group(Piece *p);
	void addPiece(Piece *p);
	void removePiece(Piece *p);
	unsigned int getNumPieces() { return mPieces.size(); }
	Piece *getPiece(int i) { return mPieces[i]; }
	std::vector<Piece *>getPieces() { return mPieces; }
	Coord *getAxis() { return axis; }
	Coord *setAxis(Coord *c) { axis = c; }
	void fan();
	void stack();

protected:
	std::vector<Piece *> mPieces;
	Coord *axis;
	bool isStacked;
};

#endif /* Group.h */