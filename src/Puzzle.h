#ifndef PUZZLE_H
#define PUZZLE_H

#include "Piece.h"
#include <vector>
#define IMG_PATH "../images/"

class Group;

enum {EASY = 7, MEDIUM = 10, HARD = 14};

class Puzzle {
public:
	Puzzle(int type, int puz);
	~Puzzle();
	void draw();
	Piece *getPiece(unsigned int id);
	bool check(Piece *p);
	static int LoadBMP(const char* location, unsigned int &texture);
	void addGroup(Group *g);
	void deleteGroup(Group *g);
	Group *checkGroups(Piece *p);
	void solve(float cx, float cy, float cz);
private:
	void join(Piece *moving, Piece *fixed, int dir);
	bool connect(Piece *p);
	std::vector<Piece *> mPieces;
	std::vector<Group *> mGroups;
	unsigned int mTexture;
	float mPieceSize;
	int mType;
};

#endif /* Puzzle.h */