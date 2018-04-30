#pragma once

#include<iostream>
#include"Board.h"

class MCTS
{
private:
	int player;
	int duration = 3;
	double C;


public:
	Board * root;
	MCTS(double p[4], char state[8][8], int player, double _C);

	Board* search();
	Board* treePolicy();
	void release();
	int defaultPolicy(Board* curState);


};
