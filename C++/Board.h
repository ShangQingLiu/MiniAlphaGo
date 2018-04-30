#pragma once

#include<iostream>
#include<cstring>
#include<vector>
#include<bitset>


using namespace std;


class Board
{
private:
	int type;
	char available[30];
	Board* parent;
	Board* child[30];
	void getPotential();
	int checkCorner(short pos[2]);

public:
	int  numChild;
	double *p;
	int moveAbility, inStable, sideStable, stable, potential, frontNum;
	double edge;
	char state[8][8];

	vector<short> unvisited;
	double N, Q;
	int numAvailable, chessNum;
	int player, step;
	short action[2];
	double curScore;
	Board();
	Board(double *_p, char _state[8][8], int _player, Board* _parent, int real = 1, int quiet = 0);
	int checkWin();
	int checkEdge();
	void getStable();
	void getInStable();
	int needExpand();
	void release();
	Board* expand();
	Board* randomGo();
	void setAction(short action);
	double isWinner();
	void backup(double reward);
	double getScore(Board& obj, double C, int final);
	Board* finalChild();
	Board* bestChild(double C, int final);
	bool isTerminal();
	int corner();
};

