#include <iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;

// game variables
bool GameOver;
int x, y, fruitX, fruitY, bonusX, bonusY, score, bonusMode;
int tailX[100], tailY[100];
int tailNum;
const int width = 50, height = 25;
enum eDirection {STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;

// moves console cursor to desired position
void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

// assigns x and y values to bonus object and returns false if they already belong to fruit object, otherwise returns false
bool checkRand() {

	bonusY = (rand() % (height - 2)) + 1;
	bonusX = (rand() % (width - 2)) + 1;

	if (bonusX == fruitX && bonusY == fruitY)
		return false;
	
	return true;
}

// function responsible to set up the game
void Setup() {

	srand(time(NULL));
	x = width / 2;
	y = height / 2;
	fruitX = (rand() % (width - 2)) + 1;
	fruitY = (rand() % (height - 2)) + 1;
	while (!checkRand())
	bonusMode = 0;
	GameOver = false;
	dir = STOP;
	tailNum = 0;
}

// draw borders used only once in the main function
void DrawMap() {
	for (int i = 0; i < width; i++) {
		cout << "#";
	}
	cout << endl;

	for (int i = 1; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width - 1)
				cout << "#";
			else							
				cout << " ";
			}
			cout << endl;
		}
	for (int i = 0; i < width; i++) {
		cout << "#";
	}
	cout << endl;
	cout << "Score: " << score;
}

// draw version2 (testing) draws objects to the screen
void Draw2() {

	setCursorPosition(1, 1);
	for (int i = 1; i < height; i++) {

		for (int j = 1; j < width - 1; j++) {	

			if (j == x && i == y) // render head
				cout << "@";
			else if (j == fruitX && i == fruitY) // render fruit
				cout << "F";
			else if (j == bonusX && i == bonusY) // render bonus
				cout << "B";
			else
			{
				bool print = false;
				if (tailNum > 0)
				{
					for (int k = 0; k < tailNum; k++) {
						if (j == tailX[k] && i == tailY[k]) {
							cout << "0";
							print = true;
						}
					}

				}
				if (!print)
					cout << " ";
			}
		}
		if (i == 1)
			cout << "#  Bonus mode: " << bonusMode << " ";
		cout << "\n#";
		
	}

	setCursorPosition(0, height+1);
	cout << "Score: " << score;

}
// draw version 1, draws all objects to the screen
void Draw() {

	//system("cls");
	setCursorPosition(0, 0);
	for (int i = 0; i < width; i++) {
		cout << "#";
	}
	cout << endl;

	for (int i = 1; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0  || j == width - 1)
				cout << "#";
			else if (j == x && i == y)
				cout << "@";
			else if (j == fruitX && i == fruitY)
				cout << "F";
			else
			{
				bool print = false;
				if (tailNum > 0)
				{
					for (int k = 0; k < tailNum; k++) {
						if (j == tailX[k] && i == tailY[k]) {
							cout << "0";
							print = true;
						}
					}

				}
				if (!print)
					cout << " ";
			}
		}
		cout << endl;
	}
		
	
	for (int i = 0; i < width; i++) {
		cout << "#";
	}
	cout << endl;
	cout << "Score: " << score;

}

// handles user input
void Input() {

	// simple input based on 4 keys w, a, s and d
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
			if(dir != DOWN)
				dir = UP;			
			break;
		case 's':
			if (dir != UP)
				dir = DOWN;		
			break;
		case 'a':
			if (dir != RIGHT)
				dir = LEFT;			
			break;
		case 'd':
			if (dir != LEFT)
				dir = RIGHT;			
			break;
		case 'p':
			GameOver = true;
		}
		_getch();
	}
}

// handles all the logic of the game
void Logic() {


	// tail positions
	int prevX = x, prevY = y;
	int prev2X, prev2Y;
	for (int i = 0; i < tailNum; i++) { 
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	// actions after input
	switch (dir) {
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	}

/*
	######
	#    #
	#	 #
	#	 #
	######

*/
	 
	if (bonusMode <= 0) { // touching walls
		
		if (y <= 0 || y >= height)
			GameOver = true;
		if (x <= 0 || x >= width - 1)
			GameOver = true;

		if (5 == rand() % 100 && bonusX <= 0 && bonusY <= 0) // set new bonus in the map only if it is not there already
			while (!checkRand()) {

			}

	}
	else { // going through walls !

		if (y <= 0 || y >= height)
			y = y == 0 ? (height - 1) : 1;
		if (x <= 0 || x >= width - 1)
			x = x == 0 ? width - 1 : 1;
		bonusMode -= 1;
	}	
	

	// touching tail
	for (int i = 0; i < tailNum; i++) {
		if (x == tailX[i] && y == tailY[i])
			GameOver = true;
	}

	// touching fruit + gaining tail
	if (x == fruitX && y == fruitY) {
		score += 10;
		fruitX = (rand() % (width - 2)) + 1;
		fruitY = (rand() % (height - 2)) + 1;
		tailNum++;
	}
	// touching bonus
	if (x == bonusX && y == bonusY) {
		bonusMode = 200;
		bonusX = -1;
		bonusY = -1;
	}
}

// a function that disables console cursor
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void main()
{
	Setup(); // set up all game variables
	DrawMap(); // draw map only once
	ShowConsoleCursor(false); // disable console cursor
	while (!GameOver) // game loop
	{
		Draw2();
		Input();
		Logic();
		Sleep(41);
	}
	cout << endl;
	cout << "GAME OVER";
}
