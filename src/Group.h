#ifndef GROUP_H
#define GROUP_H

#include <vector>
class Piece;

class Group {
	public:
	Group() {};
	Group(Piece *p) { addPiece(p); }
	void addPiece(Piece *p) { // don't add duplicates
		for(unsigned int i = 0; i < mPieces.size(); i++) {
			if(mPieces[i] == p)
				return;
		}
		mPieces.push_back(p); 
	}
	unsigned int getNumPieces() { return mPieces.size(); }
	Piece *getPiece(int i) { return mPieces[i]; }
	std::vector<Piece *>getPieces() { return mPieces; }

protected:
	std::vector<Piece *> mPieces;
};

#endif /* Group.h */