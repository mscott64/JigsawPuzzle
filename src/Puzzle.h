#ifndef PUZZLE_H
#define PUZZLE_H

#include "Piece.h"
#include <vector>

enum {EASY = 7, MEDIUM = 10, HARD = 14};

class Puzzle {
public:
	Puzzle(int type);
	~Puzzle();
	void draw();
	Piece *getPiece(unsigned int id);
private:
	std::vector<Piece *> mPieces;
	unsigned int mTexture;
};

#endif /* Puzzle.h */