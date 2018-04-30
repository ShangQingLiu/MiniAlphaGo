#include<iostream>
#include<time.h>
#include"MCTS.h"



MCTS::MCTS(double p[4], char state[8][8], int _player, double _C)
{
	player = _player;
	C = _C;
	root = new Board(p, state, player, NULL, 2);
}

Board* MCTS::search()
{
	Board* nextBoard;
	Board* curState = root;
	int i;
	double reward;
	time_t beg;
	if (root->numAvailable == 0)
	{
		return NULL;
	}

	int k = 1;
	if (root->step > 50)
		k *= 6;
	else
		if (root->step > 35)
			k *= 2;
	//beg = clock();
	while (1)
	{
		/*
		#pragma omp parrallel sections
		{
		#pragma omp section
		{
		for (int i = 0; i < 10000; i++)
		{
		nextBoard = treePolicy();
		reward = defaultPolicy(nextBoard);
		nextBoard->backup(reward);
		}		pos = action;
		}
		}*/

		for (int i = 0; i < 20000; i++)
		{
			nextBoard = treePolicy();
			reward = defaultPolicy(nextBoard);
			nextBoard->backup(reward);
		}
		break;
		//		if ((clock() - beg) > 25000)
		//		break;
	}

	return root->bestChild(0, 2);

}


void MCTS::release()
{
	root->release();
}


Board* MCTS::treePolicy()
{
	Board *curState = root;

	while (1)
	{
		if (curState->isTerminal())
			break;
		if (curState->needExpand())
			return curState->expand();
		else
			curState = curState->bestChild(C, 1);
	}

	return curState;
}

int MCTS::defaultPolicy(Board* start)
{
	Board* curState = start;
	Board* next;
	while (!(curState->isTerminal()))
	{
		next = curState->randomGo();
		if (curState != start)
		{
			curState->release();
		}
		curState = next;
	}

	int result;
	if (start->player == curState->player)
		result = curState->isWinner();
	else
		result = 1 - curState->isWinner();

	if (curState != start)
	{
		curState->unvisited.clear();
		curState->release();
	}

	return result;
}

