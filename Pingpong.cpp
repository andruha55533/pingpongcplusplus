#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <fstream>

using namespace std;

class GameObject {
public:
	int x = 0;
	int y = 0;
	virtual void draw() = 0; 
	virtual void movement() = 0; 
};

void gotoxy(int x, int y) {
	COORD a;
	a.X = x;
	a.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void hidecurs() {
	CONSOLE_CURSOR_INFO cursoA;
	cursoA.dwSize = 100;
	cursoA.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursoA);
}

bool gameover;
int const screenH = 20;
int const screenW = 30;
int mapa[screenH][screenW];
int scorep1;
int scorep2;

class Paddle : public GameObject {
public:
	int speed = 0;
	char direct = ' ';
	int delay = 0;
	int count_delay = 0;

	void draw() override {
		for (int i = 0; i < 6; i++)
			mapa[y][x + i] = 1;
	}

	void movement() override {
		if (count_delay == delay && direct != 'S') {
			if (direct == 'L' && x - speed > 0) {
				x -= speed;
			}
			else if (direct == 'R' && x + speed < screenW - 6) {
				x += speed;
			}
			count_delay = 0;
		}
		count_delay++;
	}
};

void filescore() {
	ofstream file("scores.txt");
	if (!file.is_open()) {
		cout << "Error!." << endl;
		return;
	}

	file << "Players score:" << endl;
	file << "Player 1: " << scorep1 << endl;
	file << "Player 2: " << scorep2 << endl;

	file.close();
}

class Ball : public GameObject{
public:
	int speed;
	int direct;
	int delay;
	int count_delay;

	void draw() override{
		mapa[y][x] = 5;
	}
	
	void movement() override {
		if (count_delay == delay && direct != 9) {
			delay = 5;
			if (direct == 0 && !collision(x - speed, y - speed)) {
				x -= speed;
				y -= speed;
			}
			else if (direct == 1 && !collision(x + speed, y - speed)) {
				x += speed;
				y -= speed;
			}else if (direct == 2 && !collision(x - speed, y + speed)) {
				x -= speed;
				y += speed;
			}else if (direct == 3 && !collision(x + speed, y + speed)) {
				x += speed;
				y += speed;
			}
			count_delay = 0;
		}
		count_delay++;
	}

			bool collision(int fx, int fy) {
				if (mapa[fy][fx] == 9 || mapa[fy][fx] == 1 || mapa[fy][fx] == 7 || mapa[fy][fx] == 8) {
					bounce(fx, fy);
					return true;
				}
			 return false;
		}

		void bounce(int fx, int fy) {
			if (mapa[fy][fx] == 1) {
				if (direct == 0) direct = 2;
				else if (direct == 1) direct = 3;
				else if (direct == 2) direct = 0;
				else if (direct == 3) direct = 1;
			}
			else if (mapa[fy][fx] == 9) {
				if (direct == 0) direct = 2;
				else if (direct == 1) direct = 0;
				else if (direct == 2) direct = 3;
				else if (direct == 3) direct = 2;
			}
			else if (mapa[fy][fx] == 7 || mapa[fy][fx] == 8) {
				delay = 50;
				x = screenW / 2;
				y = screenH / 2;
				direct = rand() % 4;
				if (mapa[fy][fx] == 8) scorep2++;
				else if (mapa[fy][fx] == 7) scorep1++;
			}
		}
};

Paddle pl1;
Paddle pl2;
Ball myach;

void wall() {
	for (int i = 0; i < screenH; i++) {
		for (int j = 0; j < screenW; j++) {
			if (j == 0 || j == screenW - 1) mapa[i][j] = 9;
			else mapa[i][j] = 0;
		}
	}
}

void scoreWall() {
	for (int i = 0; i < screenH; i++) {
		for (int j = 0; j < screenW; j++) {
			if (i == 0) {
				mapa[i][j] = 8;
			}
			if (i == screenH - 1) {
				mapa[i][j] = 7;
			}
		}
	}
}

void setup() {
	srand(unsigned int(NULL));
	gameover = false;
	scorep1 = 0;
	scorep2 = 0;
	myach.x = screenW / 2;
	myach.y = screenH / 2;
	myach.speed = 2;
	myach.direct = rand() % 4;
	myach.delay = 5;
	pl1.x = screenW / 2 - 3;
	pl1.y = screenH - 2;
	pl1.speed = 2;
	pl1.delay = 3;
	pl2.x = screenW / 2 - 3;
	pl2.y = screenH / 7 - 1;
	pl2.speed = 2;
	pl2.delay = 3;
}

void display() {
	gotoxy(15, 0); cout << "SCORE:";
	gotoxy(2, 1); cout << "PLAYER 1:";
	gotoxy(24, 1); cout << "PLAYER 2:";
	gotoxy(5, 2); cout << scorep1;
	gotoxy(28, 2); cout << scorep2;

	for (int i = 0; i < screenH; i++) {
		for (int j = 0; j < screenW; j++) {
			gotoxy(j + 2, i + 3);
			if (mapa[i][j] == 9) cout << char(219);
			if (mapa[i][j] == 1) cout << char(219);
			if (mapa[i][j] == 7 || mapa[i][j] == 8) cout << '=';
			if (mapa[i][j] == 5) cout << char(254);
			if (mapa[i][j] == 0) cout << ' ';
		}
	}
}

void layout() {
	wall();
	scoreWall();
	pl1.draw();
	pl2.draw();
	myach.draw();
}

void input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':
			pl1.direct = 'L';
			break;
		case 'd':
			pl1.direct = 'R';
			break;
		case 75:
			pl2.direct = 'L';
			break;
		case 77:
			pl2.direct = 'R';
			break;
		case '1': 
			setup();
			break;
		case '2': 
			gameover = true;
			break;
		}
	}
}

void movs() {
	pl1.movement();
	pl2.movement();
	myach.movement();

	if (scorep1 >= 10) {
		gameover = true;
		system("cls");
		cout << "|==============|" << endl;
		cout << "|--------------|" << endl;
		cout << "|Player 1 wins!|" << endl;
		cout << "|--------------|" << endl;
		cout << "|==============|" << endl;
	}
	else if (scorep2 >= 10) {
		gameover = true;
		system("cls");
		cout << "|==============|" << endl;
		cout << "|--------------|" << endl;
		cout << "|Player 2 wins!|" << endl;
		cout << "|--------------|" << endl;
		cout << "|==============|" << endl;
	}
	filescore();
}

void showMenu() {
	system("cls"); 
	cout << "===== Menu =====\n";
	cout << "1. Start the game\n";
	cout << "2. Exit\n";
	cout << "Choose an option: ";
}

int main() {
	
	HWND s = GetConsoleWindow();
	MoveWindow(s, 300, 200, 300, 420, true);
	hidecurs();
	while (true) {
		showMenu();
		char choice;
		cin >> choice;
		if (choice == '1') {
			setup();
			system("cls");
			while (!gameover) {
				display();
				layout();
				input();
				movs();
				Sleep(50);
			}
			gameover = false; 
		}
		else if (choice == '2') {
			cout << "Thanks for visiting our game! Bye!";
			break;
		}
		else {
			cout << "Wrong choice. Please choose again\n";
			system("pause");
		}
	}

}