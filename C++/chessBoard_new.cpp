#pragma warning(disable : 4996)

#include "MCTS.h"
#include<winsgl.h>
#include<cstring>

enum MyEnum
{
	N, W, B, H
};
char chessboard[8][8];
int BP = 2, WP = 2;
int down = 0;
int make = 0;
int showOption = 1;
int USERCOLOR = 2, COMPUTERCOLOR = 1;
void testFunction(widgetObj *w, int x, int y, int status) {
	if (w->status & WIDGET_SELECTED) {
		int tmp = 0;
		tmp = USERCOLOR;
		USERCOLOR = COMPUTERCOLOR;
		COMPUTERCOLOR = tmp;
		make = 1;
		showOption = 0;
	}
}
void testFunction2(widgetObj *w, int x, int y, int status) {
	if (w->status & WIDGET_SELECTED) {
		showOption = 0;
	}
}
void layoutWidget() {
	widgetObj *first;
	widgetObj *second;
	first = newWidget(SG_BUTTON, (SGstring)"first");
	first->pos.x = 60;
	first->pos.y = 600;
	first->size.x = 60;
	first->size.y = 32;
	strcpy((char*)first->content, "先手");
	first->mouseClick = (mouseClickCall)testFunction2;
	registerWidget(first);
	free(first);
	second = newWidget(SG_BUTTON, (SGstring)"second");
	second->pos.x = 180;
	second->pos.y = 600;
	second->size.x = 60;
	second->size.y = 32;
	strcpy((char*)second->content, "后手");
	second->mouseClick = (mouseClickCall)testFunction;
	registerWidget(second);
	free(second);
}
void refreshBoard() {
	setColor(95, 47, 0);
	clearScreen();
	setColor(255, 255, 255);
	for (int i = 0; i < 9; i++) {
		putLine(60, 60 + 60 * i, 540, 60 + 60 * i, SOLID_LINE);
		putLine(60 + 60 * i, 60, 60 + 60 * i, 540, SOLID_LINE);
	}
	

}
void change2(char state[8][8], short action[2], int player)
{
	int i, interval, point, val;
	short direction[8][2] = { { -1,0 },{ 1,0 },{ 1,-1 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,1 } ,{ -1,-1 } };
	int pos[2];

	for (i = 0; i < 8; i++)
	{
		pos[0] = action[0];
		pos[1] = action[1];
		pos[0] += direction[i][0];
		pos[1] += direction[i][1];

		interval = point = 0;

		while (pos[0] >= 0 && pos[1] <= 7 && pos[0] <= 7 && pos[1] >= 0)
		{
			if (state[pos[0]][pos[1]] == 0)
				break;
			if (state[pos[0]][pos[1]] == player)
			{
				point = 1;
				break;
			}
			pos[0] += direction[i][0];
			pos[1] += direction[i][1];
			interval = 1;
		}

		if (point == 1 && interval == 1)
		{
			val = state[pos[0]][pos[1]];
			pos[0] -= direction[i][0];
			pos[1] -= direction[i][1];
			while (val != state[pos[0]][pos[1]])
			{
				state[pos[0]][pos[1]] = player;
				pos[0] -= direction[i][0];
				pos[1] -= direction[i][1];
			}
		}
	}
}
void putChess() {
	std::memset(chessboard, 0, 64 * sizeof(char));
	chessboard[3][3] = W;
	chessboard[3][4] = B;
	chessboard[4][3] = B;
	chessboard[4][4] = W;

}
void countPoint() {
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (chessboard[i][j] == 1) {//white
				WP++;
				}
			else if (chessboard[i][j] == 2) {//black
				BP++;
			}
		}
	}
	char* bscore = new char[20];
	char* wscore = new char[20];
	itoa(BP, bscore, 10);
	itoa(WP, wscore, 10);
	setColor(255, 255, 255);
	putString("Black Score", 300, 600);
	putString(bscore, 420, 600);
	putString("White Score", 300, 630);
	putString((itoa(WP, wscore, 10)), 420, 630);
	WP = 0;
	BP = 0;
}
void drawChess() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {//color
			if (chessboard[i][j] == 1) {//white
				setColor(255, 255, 255);
				putCircle(90 + 60 * j, 90 + 60 * i, 27, SOLID_FILL);
			}
			else if (chessboard[i][j] == 2) {//black
				setColor(0, 0, 0);
				putCircle(90 + 60 * j, 90 + 60 * i, 27, SOLID_FILL);
			}
			else if (chessboard[i][j] == 3) {//hint
				setColor(240, 0, 0);
				for (int k = 0; k < 5; k++) {
					putCircle(90 + 60 * j, 90 + 60 * i, 15-k, SOLID_FILL);
				}
			}
		}
	}
}

void sgSetup() {
	initWindow(600, 660, "黑白棋", BIT_MAP);
	initMouse(SG_COORDINATE);
	layoutWidget();
	initKey();
	putChess();
	return;
}

void sgLoop() 
{
	if (showOption) {
		
	}
	else {
		double p[] = { -0.04,-0.024,0.08,0.08 };
		if (biosMouse(1).m) {
			vecThree mouse = biosMouse(0);
			if (mouse.m == SG_LEFT_BUTTON | SG_BUTTON_DOWN) {
				if (mouse.x > 60 && mouse.x < 540 && mouse.y < 540 && mouse.y > 60) {
					int k = int((mouse.x - 60) / 60);
					int v = int((mouse.y - 60) / 60);
					down = 1;
					chessboard[v][k] = USERCOLOR;
					short tmp[2];
					tmp[0] = v;
					tmp[1] = k;
					change2(chessboard, tmp, USERCOLOR);
					refreshBoard();
					countPoint();
					drawChess();

				}

			}
		}


		if (make == 1)
		{
			MCTS a = MCTS(p, chessboard, COMPUTERCOLOR, 1.7);
			Board* now = a.search();
			if (now) {
				chessboard[now->action[0]][now->action[1]] = COMPUTERCOLOR;
				change2(chessboard, now->action, COMPUTERCOLOR);
			}

			down = 0;
			make = 0;
		}

		refreshBoard();
		countPoint();
		drawChess();

		if (down == 1)
			make = 1;
	}
}


