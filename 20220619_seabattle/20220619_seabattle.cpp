#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <time.h>



const int rows{ 10 }, cols{ 10 };
int xBias1{ 3 }, xBias2{ cols * 2 + xBias1 };
int yBias1{ 1 }, yBias2{ rows + 3 + yBias1 };
const int countTypeShips = 4;


struct BattleShip
{
	int countShip;
	int countDecks;
	char deck = 'D';
};

BattleShip* arrShips = new BattleShip[countTypeShips]
{
	{ 1, 4 },	
	{ 2, 3 },
	{ 3, 2 },
	{ 4, 1 }	
};

enum ConsoleColor
{
	Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray,
	LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};

void setColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void setPosition(int x, int y)
{
	HANDLE hConsole;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	SetConsoleCursorPosition(hStdOut, coord);
}

void clearScreen(int pause)
{
	Sleep(pause);
	system("cls");
}

void showMessage(std::string message, ConsoleColor color = White, int pause = 0)
{
	setColor(color, Black);
	std::cout << message;
	setColor(White, Black);
	Sleep(pause);
}

int random(int min = 0, int max = 9)
{
	return rand() % (max - min + 1) + min;
}


void printSubField(int** arr, int xCoord = 0, int yCoord = 0)
{
	char s = 65;
	int maxSymbol = s + cols;

	for (int i{}; i < rows + 1; ++i)
	{
		setPosition(xCoord, yCoord + i );
		for (int j{}; j < cols + 2; ++j)
		{
			if (i == 0 && s < maxSymbol)
				j == 0 ? std::cout << "   " << s : std::cout << ' ' << s;
			else if (j == 0)
				i < 10 ? std::cout << ' ' << i : std::cout << i;
			else if (i > 0 && j < cols + 1)
			{
				char t = arr[j - 1][i - 1];
				t != 0 && j < cols + 1 ? std::cout << "|" << t : std::cout << "| ";
			}
			else if(i > 0 && j==cols+1)
				std::cout << "|";
			++s;
		}
	}
}

void printRules()
{
	setPosition(cols * 3 + 10,1);

}

void printFields(int*** arr)
{	
	printSubField(arr[0]);
	printSubField(arr[1], cols * 2 + 5);
	//printSubField(arr[2], 0, rows + 3);
	//printSubField(arr[3], cols * 2 + 5, rows + 3);
}

int** genBattleField()
{
	int** arr = new int* [cols];

	for (int i{}; i < cols; ++i)
		arr[i] = new int[rows]{};
		
	return arr;
}

bool checkFreeCeel(int** arr, int x, int y)
{
	return arr[x][y] == 0 ? true : false;
}

bool isFreeCells(int** arr, const int& cDeck, const int& x, const int& y, const int& direct)
{
	int coord = direct ? x : y;
	int sizeF = direct ? cols : rows;

	if ((coord + cDeck - 1) > (sizeF - 1) && cDeck > 1)
		return false;

	for (int i{};i < cDeck; ++i)	
		if (!checkFreeCeel(arr, direct ? x + i : x , direct ? y : y + i))
			return false;

	return true;
}

int** aroundShip(int** arr, const int& cDeck, const int& x, const int& y, const int& direct)
{
	int t = direct ? (x + cDeck) : (y + cDeck);
	if (direct)
	{
		for (int i{ x - 1 }; i < t + 1; ++i)
		{
			(y - 1) < 0 || i < 0 || i >= cols ? NULL : arr[i][y - 1] = '.';
			(y + 1) >= rows || i < 0 || i >= cols ? NULL : arr[i][y + 1] = '.';
		}
		(x - 1) < 0 ? NULL : arr[x - 1][y] = '.';
		t >= cols ? NULL : arr[t][y] = '.';
	}
	else 
	{
		for (int i{ y - 1 }; i < t + 1; ++i)
		{
			(x - 1) < 0 || i < 0 || i >= rows ? NULL : arr[x - 1][i] = '.';
			(x + 1) >= cols || i < 0 || i >= rows ? NULL : arr[x + 1][i] = '.';
		}
		(y - 1) < 0 ? NULL : arr[x][y - 1] = '.';
		t >= rows ? NULL : arr[x][t] = '.';
	}
	return arr;
}

void aiArrangeCoord(int** arr, int& cDeck, int& selCoordX, int& selCoordY, int& direct)
{
	direct = random(0, 1); // 0 - vertical, 1 - horizontal
	
	do {
		selCoordX = random(0, cols - 1);
		selCoordY = random(0, rows - 1);

	} while (!isFreeCells(arr, cDeck, selCoordX, selCoordY, direct));
}

void manualArrangeCoord()
{
	int x{-1}, y{-1};
	std::string coord;
	do {
		do {
			setPosition(cols * 4 + 10, 1);
			std::cout << "¬ведите координаты [XY]: ";
			std::cin >> coord;
		} while (coord.length() > 3);

		x = static_cast<int>(coord[0]);
		x = x < 97 ? x - 65 : x - 97;
		coord.erase(0, 1);
		if (static_cast<int>(coord[0]) < 48 || static_cast<int>(coord[0]) > 57)
			continue;
		if (coord.length() > 1)
			if (static_cast<int>(coord[1]) < 48 || static_cast<int>(coord[1]) > 57)
				continue;
		y = std::stoi(coord) - 1;
	}while (x < 0 || y < 0 || x > cols || y > rows);

    //setPosition(cols * 4 + 10, 3);
	//std::cout << x << ' ' << y; // << '\n';
}

void battleShipArrange(int** arr , int cDeck)
{
	int direct{};
	int selCoordX{}, selCoordY{}, x{}, y{};
	aiArrangeCoord(arr, cDeck, selCoordX, selCoordY, direct);
	
	for (int i{};i < cDeck; ++i)
	{		
		x = direct ? selCoordX + i : selCoordX;
		y = direct ? selCoordY : selCoordY + i;
		arr[x][y] = arrShips->deck;				
	}	
	arr = aroundShip(arr, cDeck, selCoordX, selCoordY, direct);
}

void removeAround(int** arr)
{
	for (int i{}; i < cols; ++i)
		for (int j{}; j < rows; ++j)
			arr[i][j] != 46 ? NULL : arr[i][j] = 0;
}

void fillField(int** arr)
{
	for (int i{}; i < countTypeShips; ++i)
		for (int j{}; j < arrShips[i].countShip; ++j)
			battleShipArrange(arr, arrShips[i].countDecks);
	removeAround(arr);
}


int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(0));
	rand();
	
	int** humanFieldSelf = genBattleField();
	int** humanFieldBattle = genBattleField();
	int** aiFieldSelf = genBattleField();
	int** aiFieldBattle = genBattleField();

	int*** battleField = new int**[4]{ humanFieldSelf, humanFieldBattle, aiFieldSelf, aiFieldBattle };
		
	fillField(humanFieldSelf);
	fillField(aiFieldSelf);
		
	printFields(battleField);
	manualArrangeCoord();
	

	setPosition(0, rows*2);
	std::cout << "\n\n";
	return 0;
}
