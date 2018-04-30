#include<winsgl.h>
enum MyEnum
{
	N, W, B, H
};
int board[8][8];
int BP = 2, WP = 2;
int USERCOLOR = 1, COMPUTERCOLOR;
void testFunction(widgetObj *w, int x, int y, int status) {
	if (w->status & WIDGET_SELECTED) {
		exit(0);
	}
}
void layoutWidget() {
	widgetObj *first, *second;
	first = newWidget(SG_BUTTON, (SGstring)"first");
	first->pos.x = 60;
	first->pos.y = 600;
	first->size.x = 60;
	first->size.y = 32;
	strcpy((char*)first->content, "先手");
	first->mouseClick = (mouseClickCall)testFunction;
	registerWidget(first);
	free(first);
	second = newWidget(SG_BUTTON, (SGstring)"second");
	second->pos.x = 180;
	second->pos.y = 600;
	second->size.x = 60;
	second->size.y = 32;
	strcpy((char*)second->content, "后手");
	registerWidget(second);
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

void putChess() {
	board[3][3] = W;
	board[3][4] = B;
	board[4][3] = B;
	board[4][4] = W;

}
void countPoint() {
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == 1) {//white
				WP++;
				}
			else if (board[i][j] == 2) {//black
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
			if (board[i][j] == 1) {//white
				setColor(255, 255, 255);
				putCircle(90 + 60 * j, 90 + 60 * i, 27, SOLID_FILL);
			}
			else if (board[i][j] == 2) {//black
				setColor(0, 0, 0);
				putCircle(90 + 60 * j, 90 + 60 * i, 27, SOLID_FILL);
			}
			else if (board[i][j] == 3) {//hint
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
	initKey();
	layoutWidget();
	putChess();
	return;
}
void sgLoop() {
	if (biosMouse(1).m) {
		vecThree mouse = biosMouse(0);
		if (mouse.m == SG_LEFT_BUTTON | SG_BUTTON_DOWN) {
			if (mouse.x > 60 && mouse.x < 540 && mouse.y < 540 && mouse.y > 60) {
				int k = int((mouse.x - 60) / 60);
				int v = int((mouse.y - 60) / 60);
				if (board[v][k] == 0) {
					board[v][k] = USERCOLOR;
				}
				
			}
			
		}
	}
	refreshBoard();
	countPoint();
	drawChess();
}
