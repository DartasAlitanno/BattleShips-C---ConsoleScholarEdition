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
//логические переменные условий
bool win;//условие жизни
bool placeTime;//условие размещения
bool viewGhosts;//скрыть/показать призрачную карту
bool showMenu;//открыто ли меню
bool gameBegin = false;//нажата ли кнопка play
bool showAcceptMenu = false;//показывается ли меню подтверждения
bool playerTurn;//ход игрока
bool playerWin;//если выиграл игрок
bool turnOff;//конец хода
//размеры поля
const int width = 29, height = 13;
//переменные координат
unsigned int arrowX, arrowY;//курсор
unsigned int coordX, coordY;//координаты
unsigned int ghostX, ghostY;//призрачные координаты
//переключение пунктов меню
int selectShipList = 0;//стартовое меню выбора кораблей
int selectMainMenuPoint = 1;//главного меню
int acceptSelect = 1;//меню выбора
int logListFlag = 1;//сообщение в логе
//количество кораблей
int numberS1 = 0;
int numberS2 = 0;
int numberS3 = 0;
int numberS4 = 0;
int botS1 = 0;
int botS2 = 0;
int botS3 = 0;
int botS4 = 0;
//массивы кораблей для графики
int oneDeckShips[4][2];
int twoDeckShips[3][2][2];
int threeDeckShips[2][3][2];
int fourDeckShips[4][2];
//ИИ
char numberS1_AI[4];
char numberS2_AI[3][2];
char numberS3_AI[2][3];
char numberS4_AI[4];
int lastTurnParam_AI[3];
bool lastTurnGet = false;
//графические и управляющие массивы
const char boardMarks[5] = { 'o', 's', 'x', '~', '-' };//обозначения на доске
const char leftCoordsY[10] = { '1','2','3','4','5','6','7','8','9','1' };//боковая навигация
const char topCoordsX[10] = { 'А','Б','В','Г','Д','Е','Ж','З','И','К' };//верхняя навигация
enum EnumKey { NONE = 0, A, D, W, S, ENTER, TAB, R, VGM, ESC, U } controlKey;//клавиатура
//массив для призрачной карты
const int sizeOfGhostMap = 12;
int ghostMap[sizeOfGhostMap][sizeOfGhostMap] = { 0 };
//массив для карты бота
const int sizeOfBotMap = 12;
int botMap[sizeOfBotMap][sizeOfBotMap] = { 0 };
//доска промахов
char missBoard[height][width];

//преднастройки
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
	//изначальное положение указателя
	arrowX = width / 4;
	arrowY = (height - 1) / 2;
}

//для перевода координаты X в букву
char toCharCoordX(int coordIntX) {
	for (int i = 0; i < 10; i++) {
		if (i == coordIntX)
			return topCoordsX[coordIntX];
	}
	return '0';
}

//информация в чате
string logInfo() {
	string logList[10]{
		"",//0
		"Используйте W, A, S, D для направления многопалубных кораблей.",//1
		"Недопустимое размещение кораблей! Нажмите R, чтобы стереть все корабли.",//2
		"Расположение кораблей сброшено.",//3
		"Корабли расположены автоматически.",//4
		"Ваш ход.",//5
		"Ход противника."//6
	};
	return logList[logListFlag];
}

//проверка корабелей
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

//сброс призрачной карты
void ghostMapReset() {
	for (int i = 0; i < sizeOfGhostMap; i++) {
		for (int j = 0; j < sizeOfGhostMap; j++) {
			ghostMap[j][i] = 0;
		}
	}
}

//размещение
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

//авторасстановка
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

//проверка расстановки для бота
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

//расстановка кораблей бота
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

//стреляет игрок
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

//логика бота
void brainAI(int x, int y) {
	int S1_isAlive = 4;
	int S2_isAlive = 6;
	int S3_isAlive = 6;
	int S4_isAlive = 4;
	//направление
	int path = rand() % 3;
	//анализ оставшихся кораблей игрока для ИИ
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
	//если запомнил прошлый ходы
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
		case 0://вправо
			x++;
			break;
		case 1://влево
			x--;
			break;
		case 2://вверх
			y--;
			break;
		case 3://вниз
			y++;
			break;
		}
		//визуальные координаты
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
				//завершение и передача хода
				turnOff = true;
				logListFlag = 5;
				//вкл обстрел на некст ходу
				lastTurnParam_AI[0] = x;
				lastTurnParam_AI[1] = y;
				lastTurnParam_AI[2] = path;
				lastTurnGet = true;
			}
		}
	}
}

//стреляет бот
void fireBot() {
	while (!turnOff) {
		int x = 1 + rand() % 10;
		int y = 1 + rand() % 10;
		int vx = x + 2;
		int vy = y + 1;
		//если вкл обстрел на некст ходу
		if (lastTurnGet) {
			//вкл обдумывание выстрела
			brainAI(x, y);
		}
		else if (ghostMap[x][y] == 1 && missBoard[vx][vy] != 'x' && missBoard[vx][vy] != '-') {
			missBoard[vx][vy] = 'x';
			ghostMap[x][y] = 0;
			//вкл обдумывание выстрела
			brainAI(x, y);
		}
		else if (botMap[x][y] != 1 && missBoard[vx][vy] != '-' && missBoard[vx][vy] != 'x') {
			missBoard[vx][vy] = '-';
			//завершение и передача хода
			turnOff = true;
			logListFlag = 5;
			system("cls");
		}
	}
	Sleep(1000);
}

//управление главным меню
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
		cout << "\n\n\n\t\t\t\t\t\tИгра завершена.\n\n" << endl;
		win = true;
		break;
	}
}

//меню размещения кораблей
void showStartedShipMenu() {
	const int showStartedMenu = 4;//количество пунктов меню
	int maxCountShip[4] = { 4,3,2,1 };//максимальное количество кораблей каждого типа
	int thisCountShip[4] = { numberS1, numberS2, numberS3, numberS4 };//текущее количество размещенных кораблей
	string shipsPlacedMenu[showStartedMenu] = {//имена кораблей
		"Однопалубный",
		"Двухпалубный",
		"Трехпалубный",
		"Четырехпалубный"
	};
	//вывод меню
	for (int i = 0; i < showStartedMenu; i++) {
		if (selectShipList == i) {
			cout << "\n\t\t\t\t\t-> " << shipsPlacedMenu[i] << " " << thisCountShip[i] << "/" << maxCountShip[i] << " <-";
		}
		else {
			cout << "\n\t\t\t\t\t" << shipsPlacedMenu[i] << " " << thisCountShip[i] << "/" << maxCountShip[i];
		}
	}
	cout << "\n\t\tTab - переключение пунктов меню. R - очистить поле. U - авторасстановка.";
	cout << "\n\t\t\tПо завершению расстановки кораблей, нажмите Enter.";
}

//показывать состояние кораблей
void showShipsOnBoard() {
	int ns1 = 4, ns2 = 3, ns2p = 2, ns3 = 2, ns3p = 3, ns4p = 4;
	cout << "\n\t\t\t\t\tОднопалубные: ";
	for (int i = 0; i < ns1; i++) {
		if (missBoard[oneDeckShips[i][0]][oneDeckShips[i][1]] != 'x') {
			cout << "S ";
		}
		else {
			cout << "X ";
		}
	}
	cout << "\n\t\t\t\t\tДвухпалубный: ";
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
	cout << "\n\t\t\t\t\tТрехпалубный: ";
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
	cout << "\n\t\t\t\t\tЧетырехпалубный: ";
	for (int i = 0; i < ns4p; i++) {
		if (missBoard[fourDeckShips[i][0]][fourDeckShips[i][1]] != 'x') {
			cout << "S";
		}
		else {
			cout << "X";
		}
	}
}

//нижнее описание
void bottomLegendShow() {
	cout << "\n\t\t\t\t\tКоординаты:" << " x:" << toCharCoordX(coordX) << " y:" << coordY << endl;
	cout << "\t\t\t\t\t\tВаши корабли: ";
	(placeTime) ? showStartedShipMenu() : showShipsOnBoard();
	cout << "\n\n\t\t\t\t\tW/A/S/D - перемещение." << endl;
	cout << "\t\t\t\t\tEsc - главное меню." << endl;
	cout << "\t\t\t\t\to - указатель." << endl;
	//выключение очистка лога
	if (placeTime) {
		if (logListFlag != 0 && logListFlag != 2)
			logListFlag = 1;
	}
}

//окно подтверждения
void accept() {
	system("cls");
	cout << "\n\n\n\t\t\t\t\t\t  Вы уверены?\n" << endl;
	string acceptMenu[4] = {
		"",
		"Да",
		"Нет",
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

//главное меню
void mainMenu() {
	const int numberPointMainMenu = 3;
	string mainMenuPoints[numberPointMainMenu] = {
		"",
		"Играть/Продолжить",
		"Выход"
	};
	system("cls");
	cout << "\t(i)Для корректной работы программы необходимо использовать \"Eng\" раскладку клавиатуры с выключенным CapsLock.\n";
	cout << "\n\n\n\n\n";
	for (int i = 1; i < numberPointMainMenu; i++) {
		if (selectMainMenuPoint == i) {
			cout << "\n\t\t\t\t  =---> " << mainMenuPoints[i];
		}
		else {
			cout << "\n\t\t\t\t\t" << mainMenuPoints[i];
		}
	}
	cout << "\n\n\t\t\t\t\tTab или W/S для переключения пунктов меню.\n\t\t\t\t\tEnter - выбор." << endl;
	(gameBegin) ? cout << "\t\t\t\t\tEsc - закрыть меню." << endl : cout << "\n\t\t\t\t\tВыберите \"Играть\", чтобы начать." << endl;
}

//графика
void graphic() {
	system("cls");
	//формирвоание "изображения" версия 0.2
	char gameZone[height][width];//главное не перепутать ширину и высоту -_-
	int leftLegends = 0;//главное не помещать ее в ненужный цикл -_-
	for (int i = 0; i < height; i++) {
		int topLegends = 0;//счетчик символов верхних координат
		for (int j = 0; j < width; j++) {
			//символьные координаты
			if (i == 0 && j > 2 && topLegends < 10) {
				gameZone[i][j] = topCoordsX[topLegends];
				topLegends++;
			}
			else if (i == 0 && j == 17) {
				gameZone[i][j] = ' ';
				topLegends = 0;
			}
			//цифровые координаты
			else if (i > 1 && j < 1 && i < 12) {
				gameZone[i][j] = leftCoordsY[leftLegends];
				leftLegends++;
			}
			else if (i == 11 && j == 1) {
				leftLegends = 0;
				gameZone[i][1] = '0';
			}
			else if (i > 1 && j == 15 && i < 12) {//разметка для правого поля
				gameZone[i][j] = leftCoordsY[leftLegends - 1];
				if (i == 11) {
					gameZone[i][j] = '1';
				}
			}
			else if (i == 11 && j == 16) {//костыль для нуля
				gameZone[i][j] = '0';
			}
			//вертикалньые рамки
			else if ((i > 1 && i < 12) && (j == 2 || j == 13 || j == 17 || j == 28)) {
				gameZone[i][j] = '|';
			}
			//горизонтальные рамки
			else if ((i == 1 || i == 12) && (j >= 2 && j <= 13 || j >= 17 && j <= 28)) {
				gameZone[i][j] = '=';
			}
			//попал
			else if (missBoard[j][i] == 'x') {
				gameZone[i][j] = boardMarks[2];
			}
			//указатель
			else if (i == arrowY && j == arrowX) {
				gameZone[i][j] = boardMarks[0];
			}
			//мимо
			else if (missBoard[j][i] == '-') {
				gameZone[i][j] = boardMarks[4];
			}
			//запись кораблей
			else if (numberS1 > 0 || numberS2 > 0 || numberS3 > 0 || numberS4 > 0) {//если кораблей больше 0
				if (numberS4 != 0) {//четырехпалубный
					for (int nd = 0; nd < 4; nd++) {
						if (missBoard[fourDeckShips[nd][0]][fourDeckShips[nd][1]] == 'x') {
							gameZone[fourDeckShips[nd][1]][fourDeckShips[nd][0]] = boardMarks[2];
						}
						else
							gameZone[fourDeckShips[nd][1]][fourDeckShips[nd][0]] = boardMarks[1];
					}
				}
				//трехпалубные
				for (int ns = 0; ns < numberS3; ns++) {
					for (int nd = 0; nd < 3; nd++) {
						if (missBoard[threeDeckShips[ns][nd][0]][threeDeckShips[ns][nd][1]] == 'x') {
							gameZone[threeDeckShips[ns][nd][1]][threeDeckShips[ns][nd][0]] = boardMarks[2];
						}
						else
							gameZone[threeDeckShips[ns][nd][1]][threeDeckShips[ns][nd][0]] = boardMarks[1];
					}
				}
				//двухпалубные
				for (int ns = 0; ns < numberS2; ns++) {
					for (int nd = 0; nd < 2; nd++) {
						if (missBoard[twoDeckShips[ns][nd][0]][twoDeckShips[ns][nd][1]] == 'x') {
							gameZone[twoDeckShips[ns][nd][1]][twoDeckShips[ns][nd][0]] = boardMarks[2];
						}
						else
							gameZone[twoDeckShips[ns][nd][1]][twoDeckShips[ns][nd][0]] = boardMarks[1];
					}
				}
				//однопалубные
				for (int ns = 0; ns < numberS1; ns++) {
					if (missBoard[oneDeckShips[ns][0]][oneDeckShips[ns][1]] == 'x') {
						gameZone[oneDeckShips[ns][1]][oneDeckShips[ns][0]] = boardMarks[2];
					}
					else
						gameZone[oneDeckShips[ns][1]][oneDeckShips[ns][0]] = boardMarks[1];
				}
				gameZone[i][j] = ' ';
			}
			//пустота или вода
			else {
				gameZone[i][j] = ' ';
			}
		}
	}
	//вывод "изображения"
	cout << "\t(i)Для корректной работы программы необходимо использовать \"Eng\" раскладку клавиатуры с выключенным CapsLock.\n";
	cout << "\n\t\t\t\t\t  BATTLE SHIPS\n" << endl;
	for (int i = 0; i < height; i++) {
		cout << "\t\t\t\t";
		for (int j = 0; j < width; j++) {
			cout << gameZone[i][j];
		}
		cout << endl;
	}
	//легенда
	cout << "\t\tLog: " << logInfo();
	bottomLegendShow();
	//визуализация призрачной карты
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

//чтение клавиатуры
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

//логика
void logic() {
	//распознование команд клавиатуры
	switch (controlKey) {
	case ESC:
		//если игра начата
		if (gameBegin) {
			showMenu = !showMenu;
		}
		//если меню вызвано
		if (showMenu) {
			system("color 0f");
		}
		else {
			system("color 70");
		}
		break;
	case W:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			arrowY--;
		}
		//если вызвано меню
		else if (showMenu) {
			selectMainMenuPoint--;
			if (selectMainMenuPoint == 0)//перемещение через пункты меню вверх
				selectMainMenuPoint = 2;
		}
		break;
	case S:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			arrowY++;
		}
		//если меню показывается
		else if (showMenu) {
			selectMainMenuPoint++;
			if (selectMainMenuPoint == 3)//перемещение через пункты меню вниз
				selectMainMenuPoint = 1;
		}
		break;
	case A:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			arrowX--;
		}
		//если открыто меню подтверждения
		else if (showAcceptMenu) {
			acceptSelect--;
			if (acceptSelect == 0)
				acceptSelect = 2;
		}
		break;
	case D:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			arrowX++;
		}
		//если открыто меню подтверждения
		else if (showAcceptMenu) {
			acceptSelect++;
			if (acceptSelect == 3)
				acceptSelect = 1;
		}
		break;
	case VGM:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			viewGhosts = !viewGhosts;
		}
		break;
	case ENTER:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			//если enter нажат во время этапа размещения
			if (placeTime) {
				if (logListFlag != 2)//если размещение не равно ошибке
					places();
				if (numberS1 == 4 && numberS2 == 3 && numberS3 == 2 && numberS4 == 1) {//если все корабли стоят
					showAcceptMenu = true;//спросить подтверждение
				}
			}
			//если не этап размещения
			else if (!placeTime) {
				//если ход игрока
				if (playerTurn) {
					firePlayer();
				}
			}
		}
		//если меню выбора показывается
		else if (showAcceptMenu) {
			if (acceptSelect == 1) {//если "Да"
				//во время расстановки
				if (placeTime) {
					placeTime = false;
					botPlace();
					playerTurn = rand() % 2;
					//если ход игрока
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
			else if (acceptSelect == 2) {//если "Нет"
				showAcceptMenu = false;
			}
		}
		//если показывается главное меню
		else {
			selectMainMenuList();
		}
		break;
	case TAB:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			//если tab нажат во время этапа размещения
			if (placeTime) {
				selectShipList++;
				if (selectShipList > 3)
					selectShipList = 0;
			}
		}
		//если главное меню вызвано
		else {
			selectMainMenuPoint++;
			if (selectMainMenuPoint > 2)//переключение меню
				selectMainMenuPoint = 1;
		}
		break;
	case R:
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			//сброс
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
		//если меню и выбор не показывается
		if (!showMenu && !showAcceptMenu) {
			//во время этапа размещения
			if (placeTime) {
				autoPlaces();
			}
		}
		break;
	}
	//по правому полю X
	if (!placeTime) {
		if (arrowX == 17) {
			arrowX = width - 2;
		}
		else if (arrowX == width - 1) {
			arrowX = 18;
		}
	}
	//по левому полю X
	else {
		if (arrowX == 2)
			arrowX = width / 2 - 2;
		if (arrowX == width / 2 - 1)
			arrowX = 3;
	}
	//перемещение по Y
	if (arrowY == 1) {
		arrowY = height - 2;
	}
	else if (arrowY == height - 1) {
		arrowY = 2;
	}
	//получение правильных координат на первом поле X
	if (arrowX < 13) {
		coordX = arrowX - 3;
		coordY = arrowY - 1;
	}
	//получение правильных координат на втором поле X
	else if (arrowX > 12) {
		coordX = arrowX - 18;
		coordY = arrowY - 1;
	}
	//коррекция призрачных координат
	ghostX = coordX + 1;
	ghostY = coordY;
	//если бой начат
	if (!placeTime) {
		//переключение хода для бота
		if (!playerTurn) {
			fireBot();
		}
		//переключение хода
		if (turnOff) {
			playerTurn = !playerTurn;
			turnOff = false;
		}
		bool botHaveShips = false;
		bool playerHaveShips = false;
		//поиск кораблей
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
		//условие победы бота
		if (!playerHaveShips) {
			playerWin = false;
			win = true;
			system("cls");
			cout << "\n\n\n\t\t\t\t\tВы проиграли!\n" << endl;
		}
		//условие победы игрока
		if (!botHaveShips) {
			playerWin = true;
			win = true;
			system("cls");
			cout << "\n\n\n\t\t\t\t\tПоздравляю! Вы победили!\n" << endl;
		}
	}
}

//начало
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	setup();//установка стартовых параметров
	logic();//стартовый анализ
	while (!win) {//пока не победил
		if (showMenu) {
			mainMenu();//главное меню
			input();
			logic();
		}
		else if (showAcceptMenu) {
			accept();
			input();
			logic();
		}
		else {
			graphic();//графика
			input();//ввод
			logic();//мозги
		}
	}
}