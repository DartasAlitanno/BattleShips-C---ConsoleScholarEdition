#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <time.h>
#include <conio.h>
#include <thread>
#include <string>
#include <iomanip> 
using std::cin;
using std::cout;
using std::endl;
using std::string;
//���������� ���������� �������
bool win;//������� �����
bool placeTime;//������� ����������
bool viewGhosts;//������/�������� ���������� �����
bool showMenu;//������� �� ����
bool gameBegin = false;//������ �� ������ play
bool showAcceptMenu = false;//������������ �� ���� �������������
bool playerTurn;//��� ������
bool playerWin;//���� ������� �����
bool turnOff;//����� ����
//������� ����
const int width = 29, height = 13;
//���������� ���������
unsigned int arrowX, arrowY;//������
unsigned int coordX, coordY;//����������
unsigned int ghostX, ghostY;//���������� ����������
//������������ ������� ����
int selectShipList = 0;//��������� ���� ������ ��������
int selectMainMenuPoint = 1;//�������� ����
int acceptSelect = 1;//���� ������
int logListFlag = 1;//��������� � ����
//���������� ��������
int numberS1 = 0;
int numberS2 = 0;
int numberS3 = 0;
int numberS4 = 0;
int botS1 = 0;
int botS2 = 0;
int botS3 = 0;
int botS4 = 0;
//������� �������� ��� �������
int oneDeckShips[4][2];
int twoDeckShips[3][2][2];
int threeDeckShips[2][3][2];
int fourDeckShips[4][2];
//��
char numberS1_AI[4];
char numberS2_AI[3][2];
char numberS3_AI[2][3];
char numberS4_AI[4];
int lastTurnParam_AI[3];
bool lastTurnGet = false;
//����������� � ����������� �������
const char boardMarks[5] = { 'o', 's', 'x', '~', '-' };//����������� �� �����
const char leftCoordsY[10] = { '1','2','3','4','5','6','7','8','9','1' };//������� ���������
const char topCoordsX[10] = { '�','�','�','�','�','�','�','�','�','�' };//������� ���������
enum EnumKey { NONE = 0, A, D, W, S, ENTER, TAB, R, VGM, ESC, U } controlKey;//����������
//������ ��� ���������� �����
const int sizeOfGhostMap = 12;
int ghostMap[sizeOfGhostMap][sizeOfGhostMap] = { 0 };
//������ ��� ����� ����
const int sizeOfBotMap = 12;
int botMap[sizeOfBotMap][sizeOfBotMap] = { 0 };
//����� ��������
char missBoard[height][width];

//�������������
void setup() {
	system("color 0f");
	system("mode con cols=120 lines=60");
	win = false;
	placeTime = true;
	viewGhosts = false;
	showMenu = true;
	playerTurn = true;
	controlKey = NONE;
	turnOff = false;
	//����������� ��������� ���������
	arrowX = width / 4;
	arrowY = (height - 1) / 2;
}

//��� �������� ���������� X � �����
char toCharCoordX(int coordIntX) {
	for (int i = 0; i < 10; i++) {
		if (i == coordIntX)
			return topCoordsX[coordIntX];
	}
	return '0';
}

//���������� � ����
string logInfo() {
	string logList[10]{
		"",//0
		"����������� W, A, S, D ��� ����������� ������������� ��������.",//1
		"������������ ���������� ��������! ������� R, ����� ������� ��� �������.",//2
		"������������ �������� ��������.",//3
		"������� ����������� �������������.",//4
		"��� ���.",//5
		"��� ����������."//6
	};
	return logList[logListFlag];
}

//�������� ���������
void checkGhostMap(int x, int y, int deck, int path) {
	for (int i = 0; i < deck; i++) {
		if (ghostMap[x][y] == 1 ||
			ghostMap[x - 1][y] == 1 ||
			ghostMap[x + 1][y] == 1 ||
			ghostMap[x][y - 1] == 1 ||
			ghostMap[x][y + 1] == 1 ||
			ghostMap[x - 1][y - 1] == 1 ||
			ghostMap[x - 1][y + 1] == 1 ||
			ghostMap[x + 1][y - 1] == 1 ||
			ghostMap[x + 1][y + 1] == 1) {
			logListFlag = 2;
		}
		else if (x == 0 || x == 11 || y == 0 || y == 11) {
			logListFlag = 2;
		}
		switch (path) {
		case 1:
			if (y == sizeOfGhostMap)
				logListFlag = 2;
			y++;
			break;
		case 2:
			if (x == 0)
				logListFlag = 2;
			x--;
			break;
		case 3:
			if (x == sizeOfGhostMap)
				logListFlag = 2;
			x++;
			break;
		case 4:
			if (y == 0)
				logListFlag = 2;
			y--;
			break;
		}
	}
}

//����� ���������� �����
void ghostMapReset() {
	for (int i = 0; i < sizeOfGhostMap; i++) {
		for (int j = 0; j < sizeOfGhostMap; j++) {
			ghostMap[j][i] = 0;
		}
	}
}

//����������
void places() {
	bool readWay = true;
	switch (selectShipList) {
	case 0:
		if (numberS1 != 4) {
			checkGhostMap(ghostX, ghostY, 1, 0);
			ghostMap[ghostX][ghostY] = 1;
			oneDeckShips[numberS1][0] = arrowX;
			oneDeckShips[numberS1][1] = arrowY;
			numberS1++;
		}
		break;
	case 1:
		if (numberS2 != 3) {
			while (readWay) {
				switch (_getch()) {
				case 115:
					checkGhostMap(ghostX, ghostY, 2, 1);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX][ghostY + 1] = 1;
					twoDeckShips[numberS2][0][0] = arrowX;
					twoDeckShips[numberS2][0][1] = arrowY;
					twoDeckShips[numberS2][1][0] = arrowX;
					twoDeckShips[numberS2][1][1] = arrowY + 1;
					readWay = false;
					break;
				case 97:
					checkGhostMap(ghostX, ghostY, 2, 2);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX - 1][ghostY] = 1;
					twoDeckShips[numberS2][0][0] = arrowX;
					twoDeckShips[numberS2][0][1] = arrowY;
					twoDeckShips[numberS2][1][0] = arrowX - 1;
					twoDeckShips[numberS2][1][1] = arrowY;
					readWay = false;
					break;
				case 100:
					checkGhostMap(ghostX, ghostY, 2, 3);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX + 1][ghostY] = 1;
					twoDeckShips[numberS2][0][0] = arrowX;
					twoDeckShips[numberS2][0][1] = arrowY;
					twoDeckShips[numberS2][1][0] = arrowX + 1;
					twoDeckShips[numberS2][1][1] = arrowY;
					readWay = false;
					break;
				case 119:
					checkGhostMap(ghostX, ghostY, 2, 4);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX][ghostY - 1] = 1;
					twoDeckShips[numberS2][0][0] = arrowX;
					twoDeckShips[numberS2][0][1] = arrowY;
					twoDeckShips[numberS2][1][0] = arrowX;
					twoDeckShips[numberS2][1][1] = arrowY - 1;
					readWay = false;
					break;
				}
			}
			readWay = true;
			numberS2++;
		}
		break;
	case 2:
		if (numberS3 != 2) {
			while (readWay) {
				switch (_getch()) {
				case 115:
					checkGhostMap(ghostX, ghostY, 3, 1);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX][ghostY + 1] = 1;
					ghostMap[ghostX][ghostY + 2] = 1;
					threeDeckShips[numberS3][0][0] = arrowX;
					threeDeckShips[numberS3][0][1] = arrowY;
					threeDeckShips[numberS3][1][0] = arrowX;
					threeDeckShips[numberS3][1][1] = arrowY + 1;
					threeDeckShips[numberS3][2][0] = arrowX;
					threeDeckShips[numberS3][2][1] = arrowY + 2;
					readWay = false;
					break;
				case 97:
					checkGhostMap(ghostX, ghostY, 3, 2);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX - 1][ghostY] = 1;
					ghostMap[ghostX - 2][ghostY] = 1;
					threeDeckShips[numberS3][0][0] = arrowX;
					threeDeckShips[numberS3][0][1] = arrowY;
					threeDeckShips[numberS3][1][0] = arrowX - 1;
					threeDeckShips[numberS3][1][1] = arrowY;
					threeDeckShips[numberS3][2][0] = arrowX - 2;
					threeDeckShips[numberS3][2][1] = arrowY;
					readWay = false;
					break;
				case 100:
					checkGhostMap(ghostX, ghostY, 3, 3);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX + 1][ghostY] = 1;
					ghostMap[ghostX + 2][ghostY] = 1;
					threeDeckShips[numberS3][0][0] = arrowX;
					threeDeckShips[numberS3][0][1] = arrowY;
					threeDeckShips[numberS3][1][0] = arrowX + 1;
					threeDeckShips[numberS3][1][1] = arrowY;
					threeDeckShips[numberS3][2][0] = arrowX + 2;
					threeDeckShips[numberS3][2][1] = arrowY;
					readWay = false;
					break;
				case 119:
					checkGhostMap(ghostX, ghostY, 3, 4);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX][ghostY - 1] = 1;
					ghostMap[ghostX][ghostY - 2] = 1;
					threeDeckShips[numberS3][0][0] = arrowX;
					threeDeckShips[numberS3][0][1] = arrowY;
					threeDeckShips[numberS3][1][0] = arrowX;
					threeDeckShips[numberS3][1][1] = arrowY - 1;
					threeDeckShips[numberS3][2][0] = arrowX;
					threeDeckShips[numberS3][2][1] = arrowY - 2;
					readWay = false;
					break;
				}
			}
			readWay = true;
			numberS3++;
		}
		break;
	case 3:
		if (numberS4 != 1) {
			while (readWay) {
				switch (_getch()) {
				case 115:
					checkGhostMap(ghostX, ghostY, 4, 1);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX][ghostY + 1] = 1;
					ghostMap[ghostX][ghostY + 2] = 1;
					ghostMap[ghostX][ghostY + 3] = 1;
					fourDeckShips[numberS4][0] = arrowX;
					fourDeckShips[numberS4][1] = arrowY;
					fourDeckShips[1][0] = arrowX;
					fourDeckShips[1][1] = arrowY + 1;
					fourDeckShips[2][0] = arrowX;
					fourDeckShips[2][1] = arrowY + 2;
					fourDeckShips[3][0] = arrowX;
					fourDeckShips[3][1] = arrowY + 3;
					readWay = false;
					break;
				case 97:
					checkGhostMap(ghostX, ghostY, 4, 2);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX - 1][ghostY] = 1;
					ghostMap[ghostX - 2][ghostY] = 1;
					ghostMap[ghostX - 3][ghostY] = 1;
					fourDeckShips[numberS4][0] = arrowX;
					fourDeckShips[numberS4][1] = arrowY;
					fourDeckShips[1][0] = arrowX - 1;
					fourDeckShips[1][1] = arrowY;
					fourDeckShips[2][0] = arrowX - 2;
					fourDeckShips[2][1] = arrowY;
					fourDeckShips[3][0] = arrowX - 3;
					fourDeckShips[3][1] = arrowY;
					readWay = false;
					break;
				case 100:
					checkGhostMap(ghostX, ghostY, 4, 3);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX + 1][ghostY] = 1;
					ghostMap[ghostX + 2][ghostY] = 1;
					ghostMap[ghostX + 3][ghostY] = 1;
					fourDeckShips[numberS4][0] = arrowX;
					fourDeckShips[numberS4][1] = arrowY;
					fourDeckShips[1][0] = arrowX + 1;
					fourDeckShips[1][1] = arrowY;
					fourDeckShips[2][0] = arrowX + 2;
					fourDeckShips[2][1] = arrowY;
					fourDeckShips[3][0] = arrowX + 3;
					fourDeckShips[3][1] = arrowY;
					readWay = false;
					break;
				case 119:
					checkGhostMap(ghostX, ghostY, 4, 4);
					ghostMap[ghostX][ghostY] = 1;
					ghostMap[ghostX][ghostY - 1] = 1;
					ghostMap[ghostX][ghostY - 2] = 1;
					ghostMap[ghostX][ghostY - 3] = 1;
					fourDeckShips[numberS4][0] = arrowX;
					fourDeckShips[numberS4][1] = arrowY;
					fourDeckShips[1][0] = arrowX;
					fourDeckShips[1][1] = arrowY - 1;
					fourDeckShips[2][0] = arrowX;
					fourDeckShips[2][1] = arrowY - 2;
					fourDeckShips[3][0] = arrowX;
					fourDeckShips[3][1] = arrowY - 3;
					readWay = false;
					break;
				}
			}
			readWay = true;
			numberS4++;
		}
		break;
	}
}

//���������������
void autoPlaces() {
	numberS1 = 0;
	numberS2 = 0;
	numberS3 = 0;
	numberS4 = 0;
	ghostMapReset();
	int x, y;
	int path;
	while (numberS1 != 4 && numberS2 != 3 && numberS3 != 2 && numberS4 != 1) {
		while (numberS1 != 4) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			checkGhostMap(x, y, 1, 0);
			if (logListFlag == 2) {
				logListFlag = 1;
			}
			else {
				ghostMap[x][y] = 1;
				x += 2;
				y += 1;
				oneDeckShips[numberS1][0] = x;
				oneDeckShips[numberS1][1] = y;
				numberS1++;
			}
		}
		while (numberS2 != 3) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			path = 1 + rand() % 4;
			checkGhostMap(x, y, 2, path);
			if (logListFlag == 2) {
				logListFlag = 1;
			}
			else {
				switch (path) {
				case 1:
					ghostMap[x][y] = 1;
					ghostMap[x][y + 1] = 1;
					x += 2;
					y += 1;
					twoDeckShips[numberS2][0][0] = x;
					twoDeckShips[numberS2][0][1] = y;
					twoDeckShips[numberS2][1][0] = x;
					twoDeckShips[numberS2][1][1] = y + 1;
					break;
				case 2:
					ghostMap[x][y] = 1;
					ghostMap[x - 1][y] = 1;
					x += 2;
					y += 1;
					twoDeckShips[numberS2][0][0] = x;
					twoDeckShips[numberS2][0][1] = y;
					twoDeckShips[numberS2][1][0] = x - 1;
					twoDeckShips[numberS2][1][1] = y;
					break;
				case 3:
					ghostMap[x][y] = 1;
					ghostMap[x + 1][y] = 1;
					x += 2;
					y += 1;
					twoDeckShips[numberS2][0][0] = x;
					twoDeckShips[numberS2][0][1] = y;
					twoDeckShips[numberS2][1][0] = x + 1;
					twoDeckShips[numberS2][1][1] = y;
					break;
				case 4:
					ghostMap[x][y] = 1;
					ghostMap[x][y - 1] = 1;
					x += 2;
					y += 1;
					twoDeckShips[numberS2][0][0] = x;
					twoDeckShips[numberS2][0][1] = y;
					twoDeckShips[numberS2][1][0] = x;
					twoDeckShips[numberS2][1][1] = y - 1;
					break;
				}
				numberS2++;
			}
		}
		while (numberS3 != 2) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			path = 1 + rand() % 4;
			checkGhostMap(x, y, 3, path);
			if (logListFlag == 2) {
				logListFlag = 1;
			}
			else {
				switch (path) {
				case 1:
					ghostMap[x][y] = 1;
					ghostMap[x][y + 1] = 1;
					ghostMap[x][y + 2] = 1;
					x += 2;
					y += 1;
					threeDeckShips[numberS3][0][0] = x;
					threeDeckShips[numberS3][0][1] = y;
					threeDeckShips[numberS3][1][0] = x;
					threeDeckShips[numberS3][1][1] = y + 1;
					threeDeckShips[numberS3][2][0] = x;
					threeDeckShips[numberS3][2][1] = y + 2;
					break;
				case 2:
					ghostMap[x][y] = 1;
					ghostMap[x - 1][y] = 1;
					ghostMap[x - 2][y] = 1;
					x += 2;
					y += 1;
					threeDeckShips[numberS3][0][0] = x;
					threeDeckShips[numberS3][0][1] = y;
					threeDeckShips[numberS3][1][0] = x - 1;
					threeDeckShips[numberS3][1][1] = y;
					threeDeckShips[numberS3][2][0] = x - 2;
					threeDeckShips[numberS3][2][1] = y;
					break;
				case 3:
					ghostMap[x][y] = 1;
					ghostMap[x + 1][y] = 1;
					ghostMap[x + 2][y] = 1;
					x += 2;
					y += 1;
					threeDeckShips[numberS3][0][0] = x;
					threeDeckShips[numberS3][0][1] = y;
					threeDeckShips[numberS3][1][0] = x + 1;
					threeDeckShips[numberS3][1][1] = y;
					threeDeckShips[numberS3][2][0] = x + 2;
					threeDeckShips[numberS3][2][1] = y;
					break;
				case 4:
					ghostMap[x][y] = 1;
					ghostMap[x][y - 1] = 1;
					ghostMap[x][y - 2] = 1;
					x += 2;
					y += 1;
					threeDeckShips[numberS3][0][0] = x;
					threeDeckShips[numberS3][0][1] = y;
					threeDeckShips[numberS3][1][0] = x;
					threeDeckShips[numberS3][1][1] = y - 1;
					threeDeckShips[numberS3][2][0] = x;
					threeDeckShips[numberS3][2][1] = y - 2;
					break;
				}
				numberS3++;
			}
		}
		while (numberS4 != 1) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			path = 1 + rand() % 4;
			checkGhostMap(x, y, 4, path);
			if (logListFlag == 2) {
				logListFlag = 1;
			}
			else {
				switch (path) {
				case 1:
					ghostMap[x][y] = 1;
					ghostMap[x][y + 1] = 1;
					ghostMap[x][y + 2] = 1;
					ghostMap[x][y + 3] = 1;
					x += 2;
					y += 1;
					fourDeckShips[0][0] = x;
					fourDeckShips[0][1] = y;
					fourDeckShips[1][0] = x;
					fourDeckShips[1][1] = y + 1;
					fourDeckShips[2][0] = x;
					fourDeckShips[2][1] = y + 2;
					fourDeckShips[3][0] = x;
					fourDeckShips[3][1] = y + 3;
					break;
				case 2:
					ghostMap[x][y] = 1;
					ghostMap[x - 1][y] = 1;
					ghostMap[x - 2][y] = 1;
					ghostMap[x - 3][y] = 1;
					x += 2;
					y += 1;
					fourDeckShips[0][0] = x;
					fourDeckShips[0][1] = y;
					fourDeckShips[1][0] = x - 1;
					fourDeckShips[1][1] = y;
					fourDeckShips[2][0] = x - 2;
					fourDeckShips[2][1] = y;
					fourDeckShips[3][0] = x - 3;
					fourDeckShips[3][1] = y;
					break;
				case 3:
					ghostMap[x][y] = 1;
					ghostMap[x + 1][y] = 1;
					ghostMap[x + 2][y] = 1;
					ghostMap[x + 3][y] = 1;
					x += 2;
					y += 1;
					fourDeckShips[0][0] = x;
					fourDeckShips[0][1] = y;
					fourDeckShips[1][0] = x + 1;
					fourDeckShips[1][1] = y;
					fourDeckShips[2][0] = x + 2;
					fourDeckShips[2][1] = y;
					fourDeckShips[3][0] = x + 3;
					fourDeckShips[3][1] = y;
					break;
				case 4:
					ghostMap[x][y] = 1;
					ghostMap[x][y - 1] = 1;
					ghostMap[x][y - 2] = 1;
					ghostMap[x][y - 3] = 1;
					x += 2;
					y += 1;
					fourDeckShips[0][0] = x;
					fourDeckShips[0][1] = y;
					fourDeckShips[1][0] = x;
					fourDeckShips[1][1] = y - 1;
					fourDeckShips[2][0] = x;
					fourDeckShips[2][1] = y - 2;
					fourDeckShips[3][0] = x;
					fourDeckShips[3][1] = y - 3;
					break;
				}
				numberS4++;
			}
		}
	}
	logListFlag = 4;
}

//�������� ����������� ��� ����
bool checkBotMap(int x, int y, int deck, int path) {
	for (int i = 0; i < deck; i++) {
		if (botMap[x][y] == 1 ||
			botMap[x - 1][y] == 1 ||
			botMap[x + 1][y] == 1 ||
			botMap[x][y - 1] == 1 ||
			botMap[x][y + 1] == 1 ||
			botMap[x - 1][y - 1] == 1 ||
			botMap[x - 1][y + 1] == 1 ||
			botMap[x + 1][y - 1] == 1 ||
			botMap[x + 1][y + 1] == 1) {
			return 1;
		}
		else if (x == 0 || x == 11 || y == 0 || y == 11) {
			return 1;
		}
		switch (path) {
		case 1:
			if (y == sizeOfGhostMap)
				return 1;
			y++;
			break;
		case 2:
			if (x == 0)
				return 1;
			x--;
			break;
		case 3:
			if (x == sizeOfGhostMap)
				return 1;
			x++;
			break;
		case 4:
			if (y == 0)
				return 1;
			y--;
			break;
		}
	}
	return 0;
}

//����������� �������� ����
void botPlace() {
	int x, y;
	int path;
	while (botS1 != 4 && botS2 != 3 && botS3 != 2 && botS4 != 1) {
		while (botS1 != 4) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			if (!checkBotMap(x, y, 1, 0)) {
				botMap[x][y] = 1;
				botS1++;
			}
		}
		while (botS2 != 3) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			path = 1 + rand() % 4;
			if (!checkBotMap(x, y, 2, path)) {
				switch (path) {
				case 1:
					botMap[x][y] = 1;
					botMap[x][y + 1] = 1;
					break;
				case 2:
					botMap[x][y] = 1;
					botMap[x - 1][y] = 1;
					break;
				case 3:
					botMap[x][y] = 1;
					botMap[x + 1][y] = 1;
					break;
				case 4:
					botMap[x][y] = 1;
					botMap[x][y - 1] = 1;
					break;
				}
				botS2++;
			}
		}
		while (botS3 != 2) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			path = 1 + rand() % 4;
			if (!checkBotMap(x, y, 3, path)) {
				switch (path) {
				case 1:
					botMap[x][y] = 1;
					botMap[x][y + 1] = 1;
					botMap[x][y + 2] = 1;
					break;
				case 2:
					botMap[x][y] = 1;
					botMap[x - 1][y] = 1;
					botMap[x - 2][y] = 1;
					break;
				case 3:
					botMap[x][y] = 1;
					botMap[x + 1][y] = 1;
					botMap[x + 2][y] = 1;
					break;
				case 4:
					botMap[x][y] = 1;
					botMap[x][y - 1] = 1;
					botMap[x][y - 2] = 1;
					break;
				}
				botS3++;
			}
		}
		while (botS4 != 1) {
			x = 1 + rand() % 10;
			y = 1 + rand() % 10;
			path = 1 + rand() % 4;
			if (!checkBotMap(x, y, 4, path)) {
				switch (path) {
				case 1:
					botMap[x][y] = 1;
					botMap[x][y + 1] = 1;
					botMap[x][y + 2] = 1;
					botMap[x][y + 3] = 1;
					break;
				case 2:
					botMap[x][y] = 1;
					botMap[x - 1][y] = 1;
					botMap[x - 2][y] = 1;
					botMap[x - 3][y] = 1;
					break;
				case 3:
					botMap[x][y] = 1;
					botMap[x + 1][y] = 1;
					botMap[x + 2][y] = 1;
					botMap[x + 3][y] = 1;
					break;
				case 4:
					botMap[x][y] = 1;
					botMap[x][y - 1] = 1;
					botMap[x][y - 2] = 1;
					botMap[x][y - 3] = 1;
					break;
				}
				botS4++;
			}
		}
	}
}

//�������� �����
void firePlayer() {
	if (botMap[ghostX][ghostY] == 1 && missBoard[arrowX][arrowY] != 'x' && missBoard[arrowX][arrowY] != '-') {
		missBoard[arrowX][arrowY] = 'x';
		botMap[ghostX][ghostY] = 0;
	}
	else if (botMap[ghostX][ghostY] != 1 && missBoard[arrowX][arrowY] != '-' && missBoard[arrowX][arrowY] != 'x') {
		missBoard[arrowX][arrowY] = '-';
		turnOff = true;
		logListFlag = 6;
	}
}

//������ ����
void brainAI(int x, int y) {
	int S1_isAlive = 4;
	int S2_isAlive = 6;
	int S3_isAlive = 6;
	int S4_isAlive = 4;
	//�����������
	int path = rand() % 3;
	//������ ���������� �������� ������ ��� ��
	for (int i = 0; i < 4; i++) {
		if (missBoard[oneDeckShips[i][0]][oneDeckShips[i][1]] == 'x') {
			numberS1_AI[i] = 'X';
			S1_isAlive--;
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (missBoard[twoDeckShips[i][j][0]][twoDeckShips[i][j][1]] == 'x') {
				numberS2_AI[i][j] = 'X';
				S2_isAlive--;
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (missBoard[threeDeckShips[i][j][0]][threeDeckShips[i][j][1]] == 'x') {
				numberS3_AI[i][j] = 'X';
				S3_isAlive--;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (missBoard[fourDeckShips[i][0]][fourDeckShips[i][1]] == 'x') {
			numberS4_AI[i] = 'X';
			S4_isAlive--;
		}
	}
	//���� �������� ������� ����
	if (lastTurnGet) {
		lastTurnGet = false;
		x = lastTurnParam_AI[0];
		y = lastTurnParam_AI[1];
		path = lastTurnParam_AI[2];
		if (path > 0)
			path--;
		else if (path < 3)
			path++;
	}
	while (!turnOff) {
		switch (path) {
		case 0://������
			x++;
			break;
		case 1://�����
			x--;
			break;
		case 2://�����
			y--;
			break;
		case 3://����
			y++;
			break;
		}
		//���������� ����������
		int vx = x + 2;
		int vy = y + 1;
		if (S4_isAlive > 0) {
			if (ghostMap[x][y] == 1 && missBoard[vx][vy] != 'x' && missBoard[vx][vy] != '-') {
				missBoard[vx][vy] = 'x';
				ghostMap[x][y] = 0;
				S4_isAlive--;
			}
			else if (botMap[x][y] != 1 && missBoard[vx][vy] != '-' && missBoard[vx][vy] != 'x') {
				missBoard[vx][vy] = '-';
				//���������� � �������� ����
				turnOff = true;
				logListFlag = 5;
				//��� ������� �� ����� ����
				lastTurnParam_AI[0] = x;
				lastTurnParam_AI[1] = y;
				lastTurnParam_AI[2] = path;
				lastTurnGet = true;
			}
		}
	}
}

//�������� ���
void fireBot() {
	while (!turnOff) {
		int x = 1 + rand() % 10;
		int y = 1 + rand() % 10;
		int vx = x + 2;
		int vy = y + 1;
		//���� ��� ������� �� ����� ����
		if (lastTurnGet) {
			//��� ����������� ��������
			brainAI(x, y);
		}
		else if (ghostMap[x][y] == 1 && missBoard[vx][vy] != 'x' && missBoard[vx][vy] != '-') {
			missBoard[vx][vy] = 'x';
			ghostMap[x][y] = 0;
			//��� ����������� ��������
			brainAI(x, y);
		}
		else if (botMap[x][y] != 1 && missBoard[vx][vy] != '-' && missBoard[vx][vy] != 'x') {
			missBoard[vx][vy] = '-';
			//���������� � �������� ����
			turnOff = true;
			logListFlag = 5;
			system("cls");
		}
	}
	Sleep(1000);
}

//���������� ������� ����
void selectMainMenuList() {
	switch (selectMainMenuPoint) {
	case 1:
		if (!gameBegin) {
			system("color 70");
			gameBegin = true;
			showMenu = false;
		}
		else {
			system("color 70");
			showMenu = false;
		}
		break;
	case 2:
		system("cls");
		cout << "\n\n\n\t\t\t\t\t\t���� ���������.\n\n" << endl;
		win = true;
		break;
	}
}

//���� ���������� ��������
void showStartedShipMenu() {
	const int showStartedMenu = 4;//���������� ������� ����
	int maxCountShip[4] = { 4,3,2,1 };//������������ ���������� �������� ������� ����
	int thisCountShip[4] = { numberS1, numberS2, numberS3, numberS4 };//������� ���������� ����������� ��������
	string shipsPlacedMenu[showStartedMenu] = {//����� ��������
		"������������",
		"������������",
		"������������",
		"���������������"
	};
	//����� ����
	for (int i = 0; i < showStartedMenu; i++) {
		if (selectShipList == i) {
			cout << "\n\t\t\t\t\t-> " << shipsPlacedMenu[i] << " " << thisCountShip[i] << "/" << maxCountShip[i] << " <-";
		}
		else {
			cout << "\n\t\t\t\t\t" << shipsPlacedMenu[i] << " " << thisCountShip[i] << "/" << maxCountShip[i];
		}
	}
	cout << "\n\t\tTab - ������������ ������� ����. R - �������� ����. U - ���������������.";
	cout << "\n\t\t\t�� ���������� ����������� ��������, ������� Enter.";
}

//���������� ��������� ��������
void showShipsOnBoard() {
	int ns1 = 4, ns2 = 3, ns2p = 2, ns3 = 2, ns3p = 3, ns4p = 4;
	cout << "\n\t\t\t\t\t������������: ";
	for (int i = 0; i < ns1; i++) {
		if (missBoard[oneDeckShips[i][0]][oneDeckShips[i][1]] != 'x') {
			cout << "S ";
		}
		else {
			cout << "X ";
		}
	}
	cout << "\n\t\t\t\t\t������������: ";
	for (int i = 0; i < ns2; i++) {
		for (int j = 0; j < ns2p; j++) {
			if (missBoard[twoDeckShips[i][j][0]][twoDeckShips[i][j][1]] != 'x') {
				cout << "S";
			}
			else {
				cout << "X";
			}
		}
		cout << " ";
	}
	cout << "\n\t\t\t\t\t������������: ";
	for (int i = 0; i < ns3; i++) {
		for (int j = 0; j < ns3p; j++) {
			if (missBoard[threeDeckShips[i][j][0]][threeDeckShips[i][j][1]] != 'x') {
				cout << "S";
			}
			else {
				cout << "X";
			}
		}
		cout << " ";
	}
	cout << "\n\t\t\t\t\t���������������: ";
	for (int i = 0; i < ns4p; i++) {
		if (missBoard[fourDeckShips[i][0]][fourDeckShips[i][1]] != 'x') {
			cout << "S";
		}
		else {
			cout << "X";
		}
	}
}

//������ ��������
void bottomLegendShow() {
	cout << "\n\t\t\t\t\t����������:" << " x:" << toCharCoordX(coordX) << " y:" << coordY << endl;
	cout << "\t\t\t\t\t\t���� �������: ";
	(placeTime) ? showStartedShipMenu() : showShipsOnBoard();
	cout << "\n\n\t\t\t\t\tW/A/S/D - �����������." << endl;
	cout << "\t\t\t\t\tEsc - ������� ����." << endl;
	cout << "\t\t\t\t\to - ���������." << endl;
	//���������� ������� ����
	if (placeTime) {
		if (logListFlag != 0 && logListFlag != 2)
			logListFlag = 1;
	}
}

//���� �������������
void accept() {
	system("cls");
	cout << "\n\n\n\t\t\t\t\t\t  �� �������?\n" << endl;
	string acceptMenu[4] = {
		"",
		"��",
		"���",
		""
	};
	cout << "\t\t\t\t\t";
	for (int i = 1; i < 3; i++) {
		if (i == acceptSelect)
			cout << "\t->" << acceptMenu[i] << "<-";
		else
			cout << "\t  " << acceptMenu[i];
	}
	cout << "\n\n\n";
}

//������� ����
void mainMenu() {
	const int numberPointMainMenu = 3;
	string mainMenuPoints[numberPointMainMenu] = {
		"",
		"������/����������",
		"�����"
	};
	system("cls");
	cout << "\t(i)��� ���������� ������ ��������� ���������� ������������ \"Eng\" ��������� ���������� � ����������� CapsLock.\n";
	cout << "\n\n\n\n\n";
	for (int i = 1; i < numberPointMainMenu; i++) {
		if (selectMainMenuPoint == i) {
			cout << "\n\t\t\t\t  =---> " << mainMenuPoints[i];
		}
		else {
			cout << "\n\t\t\t\t\t" << mainMenuPoints[i];
		}
	}
	cout << "\n\n\t\t\t\t\tTab ��� W/S ��� ������������ ������� ����.\n\t\t\t\t\tEnter - �����." << endl;
	(gameBegin) ? cout << "\t\t\t\t\tEsc - ������� ����." << endl : cout << "\n\t\t\t\t\t�������� \"������\", ����� ������." << endl;
}

//�������
void graphic() {
	system("cls");
	//������������ "�����������" ������ 0.2
	char gameZone[height][width];//������� �� ���������� ������ � ������ -_-
	int leftLegends = 0;//������� �� �������� �� � �������� ���� -_-
	for (int i = 0; i < height; i++) {
		int topLegends = 0;//������� �������� ������� ���������
		for (int j = 0; j < width; j++) {
			//���������� ����������
			if (i == 0 && j > 2 && topLegends < 10) {
				gameZone[i][j] = topCoordsX[topLegends];
				topLegends++;
			}
			else if (i == 0 && j == 17) {
				gameZone[i][j] = ' ';
				topLegends = 0;
			}
			//�������� ����������
			else if (i > 1 && j < 1 && i < 12) {
				gameZone[i][j] = leftCoordsY[leftLegends];
				leftLegends++;
			}
			else if (i == 11 && j == 1) {
				leftLegends = 0;
				gameZone[i][1] = '0';
			}
			else if (i > 1 && j == 15 && i < 12) {//�������� ��� ������� ����
				gameZone[i][j] = leftCoordsY[leftLegends - 1];
				if (i == 11) {
					gameZone[i][j] = '1';
				}
			}
			else if (i == 11 && j == 16) {//������� ��� ����
				gameZone[i][j] = '0';
			}
			//������������ �����
			else if ((i > 1 && i < 12) && (j == 2 || j == 13 || j == 17 || j == 28)) {
				gameZone[i][j] = '|';
			}
			//�������������� �����
			else if ((i == 1 || i == 12) && (j >= 2 && j <= 13 || j >= 17 && j <= 28)) {
				gameZone[i][j] = '=';
			}
			//�����
			else if (missBoard[j][i] == 'x') {
				gameZone[i][j] = boardMarks[2];
			}
			//���������
			else if (i == arrowY && j == arrowX) {
				gameZone[i][j] = boardMarks[0];
			}
			//����
			else if (missBoard[j][i] == '-') {
				gameZone[i][j] = boardMarks[4];
			}
			//������ ��������
			else if (numberS1 > 0 || numberS2 > 0 || numberS3 > 0 || numberS4 > 0) {//���� �������� ������ 0
				if (numberS4 != 0) {//���������������
					for (int nd = 0; nd < 4; nd++) {
						if (missBoard[fourDeckShips[nd][0]][fourDeckShips[nd][1]] == 'x') {
							gameZone[fourDeckShips[nd][1]][fourDeckShips[nd][0]] = boardMarks[2];
						}
						else
							gameZone[fourDeckShips[nd][1]][fourDeckShips[nd][0]] = boardMarks[1];
					}
				}
				//������������
				for (int ns = 0; ns < numberS3; ns++) {
					for (int nd = 0; nd < 3; nd++) {
						if (missBoard[threeDeckShips[ns][nd][0]][threeDeckShips[ns][nd][1]] == 'x') {
							gameZone[threeDeckShips[ns][nd][1]][threeDeckShips[ns][nd][0]] = boardMarks[2];
						}
						else
							gameZone[threeDeckShips[ns][nd][1]][threeDeckShips[ns][nd][0]] = boardMarks[1];
					}
				}
				//������������
				for (int ns = 0; ns < numberS2; ns++) {
					for (int nd = 0; nd < 2; nd++) {
						if (missBoard[twoDeckShips[ns][nd][0]][twoDeckShips[ns][nd][1]] == 'x') {
							gameZone[twoDeckShips[ns][nd][1]][twoDeckShips[ns][nd][0]] = boardMarks[2];
						}
						else
							gameZone[twoDeckShips[ns][nd][1]][twoDeckShips[ns][nd][0]] = boardMarks[1];
					}
				}
				//������������
				for (int ns = 0; ns < numberS1; ns++) {
					if (missBoard[oneDeckShips[ns][0]][oneDeckShips[ns][1]] == 'x') {
						gameZone[oneDeckShips[ns][1]][oneDeckShips[ns][0]] = boardMarks[2];
					}
					else
						gameZone[oneDeckShips[ns][1]][oneDeckShips[ns][0]] = boardMarks[1];
				}
				gameZone[i][j] = ' ';
			}
			//������� ��� ����
			else {
				gameZone[i][j] = ' ';
			}
		}
	}
	//����� "�����������"
	cout << "\t(i)��� ���������� ������ ��������� ���������� ������������ \"Eng\" ��������� ���������� � ����������� CapsLock.\n";
	cout << "\n\t\t\t\t\t  BATTLE SHIPS\n" << endl;
	for (int i = 0; i < height; i++) {
		cout << "\t\t\t\t";
		for (int j = 0; j < width; j++) {
			cout << gameZone[i][j];
		}
		cout << endl;
	}
	//�������
	cout << "\t\tLog: " << logInfo();
	bottomLegendShow();
	//������������ ���������� �����
	cout << endl;
	if (viewGhosts) {
		for (int i = 0; i < sizeOfGhostMap; i++) {
			cout << "\t\t";
			for (int j = 0; j < sizeOfGhostMap; j++) {
				if ((i == 0 || i == 11) || (j == 0 || j == 11))
					cout << '#';
				else if (ghostMap[j][i] == 1)
					cout << "S";
				else
					cout << boardMarks[3];
			}
			cout << endl;
		}
		cout << "\t    GhostX: " << ghostX << " GhostY: " << ghostY << endl;
		for (int i = 0; i < sizeOfBotMap; i++) {
			cout << "\t\t";
			for (int j = 0; j < sizeOfBotMap; j++) {
				if ((i == 0 || i == 11) || (j == 0 || j == 11))
					cout << '#';
				else if (botMap[j][i] == 1)
					cout << "S";
				else
					cout << boardMarks[3];
			}
			cout << endl;
		}
	}
}

//������ ����������
void input() {
	switch (_getch()) {
	case 27:
		controlKey = ESC;
		break;
	case 119:
		controlKey = W;
		break;
	case 115:
		controlKey = S;
		break;
	case 97:
		controlKey = A;
		break;
	case 100:
		controlKey = D;
		break;
	case 13:
		controlKey = ENTER;
		break;
	case 9:
		controlKey = TAB;
		break;
	case 114:
		controlKey = R;
		break;
	case 93:
		controlKey = VGM;
		break;
	case 117:
		controlKey = U;
		break;
	default:
		controlKey = NONE;
		break;
	}
}

//������
void logic() {
	//������������� ������ ����������
	switch (controlKey) {
	case ESC:
		//���� ���� ������
		if (gameBegin) {
			showMenu = !showMenu;
		}
		//���� ���� �������
		if (showMenu) {
			system("color 0f");
		}
		else {
			system("color 70");
		}
		break;
	case W:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			arrowY--;
		}
		//���� ������� ����
		else if (showMenu) {
			selectMainMenuPoint--;
			if (selectMainMenuPoint == 0)//����������� ����� ������ ���� �����
				selectMainMenuPoint = 2;
		}
		break;
	case S:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			arrowY++;
		}
		//���� ���� ������������
		else if (showMenu) {
			selectMainMenuPoint++;
			if (selectMainMenuPoint == 3)//����������� ����� ������ ���� ����
				selectMainMenuPoint = 1;
		}
		break;
	case A:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			arrowX--;
		}
		//���� ������� ���� �������������
		else if (showAcceptMenu) {
			acceptSelect--;
			if (acceptSelect == 0)
				acceptSelect = 2;
		}
		break;
	case D:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			arrowX++;
		}
		//���� ������� ���� �������������
		else if (showAcceptMenu) {
			acceptSelect++;
			if (acceptSelect == 3)
				acceptSelect = 1;
		}
		break;
	case VGM:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			viewGhosts = !viewGhosts;
		}
		break;
	case ENTER:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			//���� enter ����� �� ����� ����� ����������
			if (placeTime) {
				if (logListFlag != 2)//���� ���������� �� ����� ������
					places();
				if (numberS1 == 4 && numberS2 == 3 && numberS3 == 2 && numberS4 == 1) {//���� ��� ������� �����
					showAcceptMenu = true;//�������� �������������
				}
			}
			//���� �� ���� ����������
			else if (!placeTime) {
				//���� ��� ������
				if (playerTurn) {
					firePlayer();
				}
			}
		}
		//���� ���� ������ ������������
		else if (showAcceptMenu) {
			if (acceptSelect == 1) {//���� "��"
				//�� ����� �����������
				if (placeTime) {
					placeTime = false;
					botPlace();
					playerTurn = rand() % 2;
					//���� ��� ������
					if (playerTurn) {
						logListFlag = 5;
					}
					else if (!playerTurn) {
						logListFlag = 6;
					}
					arrowX = (width - 7);
					arrowY = (height - 1) / 2;
				}
				showAcceptMenu = false;
			}
			else if (acceptSelect == 2) {//���� "���"
				showAcceptMenu = false;
			}
		}
		//���� ������������ ������� ����
		else {
			selectMainMenuList();
		}
		break;
	case TAB:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			//���� tab ����� �� ����� ����� ����������
			if (placeTime) {
				selectShipList++;
				if (selectShipList > 3)
					selectShipList = 0;
			}
		}
		//���� ������� ���� �������
		else {
			selectMainMenuPoint++;
			if (selectMainMenuPoint > 2)//������������ ����
				selectMainMenuPoint = 1;
		}
		break;
	case R:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			//�����
			if (placeTime) {
				numberS1 = 0;
				numberS2 = 0;
				numberS3 = 0;
				numberS4 = 0;
				ghostMapReset();
				logListFlag = 3;
			}
		}
		break;
	case U:
		//���� ���� � ����� �� ������������
		if (!showMenu && !showAcceptMenu) {
			//�� ����� ����� ����������
			if (placeTime) {
				autoPlaces();
			}
		}
		break;
	}
	//�� ������� ���� X
	if (!placeTime) {
		if (arrowX == 17) {
			arrowX = width - 2;
		}
		else if (arrowX == width - 1) {
			arrowX = 18;
		}
	}
	//�� ������ ���� X
	else {
		if (arrowX == 2)
			arrowX = width / 2 - 2;
		if (arrowX == width / 2 - 1)
			arrowX = 3;
	}
	//����������� �� Y
	if (arrowY == 1) {
		arrowY = height - 2;
	}
	else if (arrowY == height - 1) {
		arrowY = 2;
	}
	//��������� ���������� ��������� �� ������ ���� X
	if (arrowX < 13) {
		coordX = arrowX - 3;
		coordY = arrowY - 1;
	}
	//��������� ���������� ��������� �� ������ ���� X
	else if (arrowX > 12) {
		coordX = arrowX - 18;
		coordY = arrowY - 1;
	}
	//��������� ���������� ���������
	ghostX = coordX + 1;
	ghostY = coordY;
	//���� ��� �����
	if (!placeTime) {
		//������������ ���� ��� ����
		if (!playerTurn) {
			fireBot();
		}
		//������������ ����
		if (turnOff) {
			playerTurn = !playerTurn;
			turnOff = false;
		}
		bool botHaveShips = false;
		bool playerHaveShips = false;
		//����� ��������
		for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 11; j++) {
				if (!botHaveShips && botMap[i][j] == 1) {
					botHaveShips = true;
				}
				if (!playerHaveShips && ghostMap[i][j] == 1) {
					playerHaveShips = true;
				}
			}
		}
		//������� ������ ����
		if (!playerHaveShips) {
			playerWin = false;
			win = true;
			system("cls");
			cout << "\n\n\n\t\t\t\t\t�� ���������!\n" << endl;
		}
		//������� ������ ������
		if (!botHaveShips) {
			playerWin = true;
			win = true;
			system("cls");
			cout << "\n\n\n\t\t\t\t\t����������! �� ��������!\n" << endl;
		}
	}
}

//������
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	setup();//��������� ��������� ����������
	logic();//��������� ������
	while (!win) {//���� �� �������
		if (showMenu) {
			mainMenu();//������� ����
			input();
			logic();
		}
		else if (showAcceptMenu) {
			accept();
			input();
			logic();
		}
		else {
			graphic();//�������
			input();//����
			logic();//�����
		}
	}
}