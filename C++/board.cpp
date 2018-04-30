#include<iostream>
#include<cstring>
#include<random>
#include<cmath>
#include<omp.h>
#include"Board.h"

using namespace std;

short dir[8][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,1 },{ 1,1 } ,{ 1,0 },{ 1,-1 },{ 0,-1 } };
char tmpState[8][8];

int check(char state[8][8], int player, short row, short col)
{
	short cur[2];
	int i, interval, point;

	for (i = 0; i < 8; ++i)
	{
		cur[0] = row + dir[i][0];
		cur[1] = col + dir[i][1];

		interval = point = 0;
		while (cur[0] >= 0 && !(cur[0] & 8) && cur[1] >= 0 && !(cur[1] & 8))
		{
			if (!state[cur[0]][cur[1]])
				break;
			if (state[cur[0]][cur[1]] == player)
			{
				point = 1;
				break;
			}
			interval = 1;
			cur[0] += dir[i][0];
			cur[1] += dir[i][1];
		}

		if (point == 1 && interval == 1)
			return 1;
	}
	return 0;
}


int getAvailable(Board* current, char state[8][8], int player, char *available)
{
	int index = 0, bad = 0;

	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
		{
			if (!state[i][j])
				if (check(state, player, i, j))
				{
					if ((i <= 1 || i >= 6) && (j <= 1 && j >= 6))
						if (i || j)
							++bad;
					available[index++] = (i << 3) | j;
				}
		}

	current->moveAbility = index - bad;
	return index;
}


int getAvailableNum(char state[8][8], int player)
{
	int index = 0;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
		{
			if (!state[i][j])
				if (check(state, player, i, j))
				{
					++index;
				}
		}

	return index;
}

void change(char state[8][8], short row, short col, int player)
{
	int i, interval, point, val;
	short pos[2];

	for (i = 0; i < 8; ++i)
	{

		pos[0] = row + dir[i][0];
		pos[1] = col + dir[i][1];

		point = interval = 0;
		while (pos[0] >= 0 && !(pos[0] & 8) && pos[1] >= 0 && !(pos[1] & 8))
		{
			if (!(state[pos[0]][pos[1]]))
				break;
			if (state[pos[0]][pos[1]] == player)
			{
				point = 1;
				break;
			}

			interval = 1;
			pos[0] += dir[i][0];
			pos[1] += dir[i][1];
		}

		if (point == 1 && interval == 1)
		{
			pos[0] -= dir[i][0];
			pos[1] -= dir[i][1];
			while (pos[0] != row || pos[1] != col)
			{
				state[pos[0]][pos[1]] = player;
				pos[0] -= dir[i][0];
				pos[1] -= dir[i][1];
			}
		}
	}

}

Board::Board()
{}

Board::Board(double *_p, char _state[8][8], int _player, Board* _parent, int real, int quiet)
{
	int i = 0, j = 0;
	p = _p;

	memcpy(state, _state, 64);
	player = _player;
	numChild = 0;
	N = Q = 0;
	step = -64;
	parent = _parent;
	if (parent == NULL)
	{
		step = 0;
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j)
				if (state[i][j])
					++step;
	}
	else
	{
		if (quiet == 1)
			step = parent->step;
		else
			step = parent->step + 1;
	}

	numAvailable = getAvailable(this, state, player, available);


	child[0] = nullptr;
	curScore = 0;
	stable = potential = stable = 0;

	/*
	if (!real && step > 45)
	getStable();*/

	if (real)
	{
		for (i = 0; i < numAvailable; ++i)
			unvisited.push_back(available[i]);
	}
	else
	{
		if (step < 30)
		{
			getPotential();
			curScore += moveAbility * p[0] + potential * p[1];
		}

	}

};


void Board::getPotential()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (state[i][j] == -player)
			{
				for (int k = 0; k < 8; k++)
				{
					int x = i + dir[k][0];
					int y = j + dir[k][1];
					if (x >= 0 && x <= 7 && y >= 0 && y <= 7 && state[x][y] == 0)
					{
						potential++;
					}
				}

				if (state[i][j] == player)
				{
					for (int k = 0; k < 8; k++)
					{
						int x = i + dir[k][0];
						int y = j + dir[k][1];
						if (x >= 0 && x <= 7 && y >= 0 && y <= 7 && state[x][y] == 0)
						{
							potential--;
						}
					}
				}
			}
		}
}

/*
void Board::getStable()
{
int i, j, k, m, n, s;
int inc, sum = 0;
int color = 0;

for (m = 0; m < 14; m += 7)
{
i = 0;
for (s = 0; s < 8; s++)
if (state[m][s] == 0)
break;
else
if (state[m][s] == -player)
sum++;
else
sum--;
if (s == 8)
{
sideStable += sum;
continue;
}
sum = i = 0;
color = state[m][0];
inc = state[m][0] * (-player);
if (color != 0)
{
while (i <= 7 && state[m][i] == state[m][0])
{
sum += inc;
i++;
}
}

n = i;
i = 7;
color = state[m][7];
inc = state[m][7] * (-player);
if (color)
{
while (i >= n && state[m][i] == color)
{
sum += inc;
i--;
}
}

sideStable += sum;
}


for (m = 0; m < 14; m += 7)
{
i = 0;
for (s = 0; s < 8; s++)
if (state[s][m] == 0)
break;
else
if (state[s][m] == -player)
sum++;
else
sum--;
if (s == 8)
{
sideStable += sum;
continue;
}
sum = i = 0;
color = state[0][m];
inc = state[0][m] * (-player);
if (color != 0)
{
while (i <= 7 && state[i][m] == state[0][m])
{
sum += inc;
i++;
}
}

n = i;
i = 7;
color = state[7][m];
inc = state[7][m] * (-player);
if (color)
{
while (i >= n && state[i][m] == color)
{
sum += inc;
i--;
}
}

sideStable += sum;
}

stable = sideStable;
}


*/

int Board::needExpand()
{
	return unvisited.size();
}

void Board::setAction(short pos)
{
	action[0] = pos >> 3;
	action[1] = pos & 7;
}

Board* Board::expand()
{
	char newPos;

	char choice = int(rand() % int(unvisited.size()));
	newPos = unvisited[choice];
	unvisited.erase(unvisited.begin() + choice);
	memcpy(tmpState, state, 64);
	char row = newPos >> 3;
	char col = newPos & 7;
	tmpState[row][col] = player;
	change(tmpState, row, col, player);
	Board *newChild = new Board(p, tmpState, 3 - player, this, 1);
	newChild->setAction(newPos);
	child[numChild++] = newChild;
	return newChild;
}

void Board::release()
{
	int i;

	if (numChild == 0 && child[0] != NULL)
		child[0]->release();
	else
	{
		for (i = 0; i < numChild; i++)
			child[i]->release();
	}

	unvisited.clear();
	delete this;
}

Board* Board::randomGo()
{
	int newPos;
	if (numAvailable == 0)
		return new Board(p, state, 3 - player, this, 0, 1);

	int choice = int(rand() % numAvailable);
	newPos = available[choice];

	memcpy(tmpState, state, 64);
	char row = newPos >> 3;
	char col = newPos & 7;
	tmpState[row][col] = player;
	change(tmpState, row, col, player);
	return new Board(p, tmpState, 3 - player, this, 0);
}

double Board::isWinner()
{
	int score = 0;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (state[i][j])
			{
				score += (state[i][j] == player) ? 1 : -1;
			}


	double ret = 0.5;

	if (score > 0)
		return 1;

	if (score < 0)
		return 0;

	return 0.5;
}


void Board::backup(double reward)
{
	Board* curState = this;
	Board* next = NULL;
	while (curState != NULL)
	{
		curState->N += 1;
		curState->Q += (curState->player == player) ? reward : (1 - reward);
		curState = curState->parent;
	}
}

int Board::checkCorner(short pos[2])
{

	if (pos[0] == 1)
	{
		if (pos[1] == 1)
			return state[0][0] ? 0 : 1;
		return state[0][7] ? 0 : 1;
	}

	if (pos[1] == 0)
		return state[7][0] ? 0 : 1;
	return state[7][7] ? 0 : 1;
}


//p[0],p[1]: action ability 
//p[2]: potential   p[3],p[4]: stable
double Board::getScore(Board& obj, double C, int final = 0)
{
	double k = 0;

	if (obj.corner() != 0)
	{
		if (step < 35)
			k -= 0.3;
		else
			k -= -0.15;
	}

	if (final)
	{
		int row = obj.action[0];
		int col = obj.action[1];
		if (row % 7 == 0 && col % 7 == 0)
		{
			if (step < 40)
				k += 0.3;
			else
				k += 0.2;
		}
		else
		{
			if ((row == 6 || row == 1) && (col == 6 || col == 1))
			{
				if (final == 2)
					k -= checkCorner(obj.action) * 0.3;
				else
					k -= checkCorner(obj.action) * 0.2;
			}

		}
	}

	if (!final)
		return k + curScore - obj.Q / obj.N + C * sqrt(2 * log(N) / obj.N);

	return k - obj.Q / obj.N + C * sqrt(2 * log(N) / obj.N);

}

Board* Board::finalChild()
{
	double score, punish, maxScore = -100, minScore = 100;
	Board* best = child[0];
	Board* tmp;
	Board* tmp2;
	int limit, i = 0;
	limit = (step < 35) ? 1000 : 800;
	maxScore = getScore(*best, 0, 2);
	while (i < numChild && child[i]->N < limit)
	{
		++i;
	}

	if (i == numChild)
	{
		i = 0;
		limit *= 0.5;
	}
	for (; i < numChild; ++i)
	{
		tmp = child[i];
		if (tmp->N < limit)
			continue;
		score = getScore(*tmp, 0, 2);

		punish = 0;
		for (int j = 0; j < tmp->numChild; ++j)
		{
			tmp2 = tmp->child[j];

			if (tmp2->N > limit && ((tmp2->Q / tmp2->N) < minScore))
			{
				minScore = tmp2->Q / tmp2->N;
			}
		}
		if (minScore < 0.3)
			punish = (1 - minScore) / 2;
		score -= punish;

		if (score > maxScore)
		{
			maxScore = score;
			best = child[i];
		}
	}
	cout << endl << "****************" << endl;

	cout << best->N << " " << maxScore << endl;
	for (int i = 0; i < best->numChild; ++i)
		cout << best->child[i]->N << "  " << best->child[i]->Q / best->child[i]->N << endl;

	return best;
}

Board* Board::bestChild(double C, int final)
{
	int i;
	double score;

	if (final == 2)
		return finalChild();

	if (numAvailable == 0)
	{
		if (child[0] != NULL)
			return child[0];
		child[0] = new Board(p, state, 3 - player, this, 1, 1);
		return child[0];
	}

	double maxScore = getScore(*(child[0]), C, final);
	Board *best = child[0];
	for (i = 1; i < numChild; ++i)
	{
		score = getScore(*(child[i]), C, final);
		if (score > maxScore)
		{
			maxScore = score;
			best = child[i];
		}
	}

	return best;
}

bool Board::isTerminal()
{
	if (step == 64)
		return true;
	if (numAvailable == 0 && getAvailableNum(state, 3 - player) == 0)
		return true;
	return false;
}

int Board::corner()
{
	if (check(state, player, 0, 0))
		return 1;
	if (check(state, player, 0, 7))
		return 2;
	if (check(state, player, 7, 0))
		return 3;
	if (check(state, player, 7, 7))
		return 4;

	return 0;
}




