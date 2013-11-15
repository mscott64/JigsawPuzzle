#ifndef JOINED_H
#define JOINED_H

#include "Piece.h"
#include <vector>

class Joined {
public:
	Joined(Piece *p) { addPiece(p); }
	void addPiece(Piece *p) { // don't add duplicates
		for(unsigned int i = 0; i < mPieces.size(); i++) {
			if(mPieces[i] == p)
				return;
		}
		mPieces.push_back(p); 
	}
	unsigned int getNumPieces() { return mPieces.size(); }
	Piece *getPiece(int i) { return mPieces[i]; }

private:
	std::vector<Piece *> mPieces;
};

#endif /* Joined.h */