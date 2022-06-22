#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <time.h>



const int rows{ 10 }, cols{ 10 };
int xBias1{ 3 }, xBias2{ cols * 2 + xBias1 } , xBias3{ cols * 4 + 10 };
int yBias1{ 3 }, yBias2{ rows + 3 + yBias1 };
const int countTypeShips = 4;
char around = '.';

struct BattleShip
{
	int countShip = 0;
	int countDecks = 0;
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

void showMesPos(std::string message, int x, int y, ConsoleColor color = White, int pause = 0)
{
	setPosition(x, y);
	showMessage(message, color, pause);
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
	//system("cls");
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

void aroundShip(int** arr, const int& cDeck, const int& x, const int& y, const int& direct)
{
	int t = direct ? (x + cDeck) : (y + cDeck);
	if (direct)
	{
		for (int i{ x - 1 }; i < t + 1; ++i)
		{
			(y - 1) < 0 || i < 0 || i >= cols ? NULL : arr[i][y - 1] = around;
			(y + 1) >= rows || i < 0 || i >= cols ? NULL : arr[i][y + 1] = around;
		}
		(x - 1) < 0 ? NULL : arr[x - 1][y] = around;
		t >= cols ? NULL : arr[t][y] = around;
	}
	else 
	{
		for (int i{ y - 1 }; i < t + 1; ++i)
		{
			(x - 1) < 0 || i < 0 || i >= rows ? NULL : arr[x - 1][i] = around;
			(x + 1) >= cols || i < 0 || i >= rows ? NULL : arr[x + 1][i] = around;
		}
		(y - 1) < 0 ? NULL : arr[x][y - 1] = around;
		t >= rows ? NULL : arr[x][t] = around;
	}
	//return arr;
}

void aiArrangeCoord(int** arr, int& cDeck, int& selX, int& selY, int& direct)
{
	direct = random(0, 1); // 0 - vertical, 1 - horizontal
	
	do {
		selX = random(0, cols - 1);
		selY = random(0, rows - 1);

	} while (!isFreeCells(arr, cDeck, selX, selY, direct));
}

bool isYesNo(int xBias, int yBias, std::string message, bool cls = 1)
{
	int inp{ -1 };

	while (inp < 0 || inp > 1)
	{
		showMesPos("      ", xBias + message.length(), yBias);
		showMesPos(message, xBias, yBias);
		std::cin >> inp;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			inp = -1;
			continue;
		}
		if (cls)
			system("cls");
	}
	return inp;
}

void manualInputCoord(int& x, int& y, int xBias, int yBias, std::string message )
{
	//x = -1,	y = -1;
	std::string coord;
	//std::string text1 = ;
	do {
		do {			
			showMesPos(message, xBias, yBias);			
			showMesPos("      ", xBias + message.length(), yBias);
			setPosition(xBias + message.length(), yBias);
			std::cin >> coord;
			std::cin.ignore(32767, '\n');
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
	}while (x < 0 || y < 0 || x >= cols || y >= rows);	   
}

void battleShipArrange(int*** arr , int& sel, int& cDeck, bool avto, bool ai)
{
	int direct{};
	int selX{}, selY{}, x{}, y{};
	if (avto)
	{
		aiArrangeCoord(arr[sel], cDeck, selX, selY, direct);
		if(ai)
			printFields(arr);
	}
	else
	{
		do {
			system("cls");
			printFields(arr);
			std::string text = "Координаты [XY] для " + std::to_string(cDeck) + (cDeck == 1 ? "-но" : "-x") + " палубного: ";
			manualInputCoord(selX, selY, xBias3, yBias1, text);
			direct = cDeck == 1 ? 0 : isYesNo(xBias3, (yBias1 + 2), "Расположение [0 - верт. / 1 - гориз.] корабля?: ", 0);
		} while (!isFreeCells(arr[sel], cDeck, selX, selY, direct));		
	}
	
	for (int i{};i < cDeck; ++i)
	{		
		x = direct ? selX + i : selX;
		y = direct ? selY : selY + i;
		arr[sel][x][y] = arrShips->deck;				
	}	
	aroundShip(arr[sel], cDeck, selX, selY, direct);
}

void removeAround(int** arr)
{
	for (int i{}; i < cols; ++i)
		for (int j{}; j < rows; ++j)
			arr[i][j] != around ? NULL : arr[i][j] = 0;
}


void fillField(int*** arr ,int sel, bool avto, bool ai = false)
{
	for (int i{}; i < countTypeShips; ++i)
		for (int j{}; j < arrShips[i].countShip; ++j)
			battleShipArrange(arr, sel, arrShips[i].countDecks, avto, ai);
	removeAround(arr[sel]);
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
	
	bool avto = isYesNo(xBias1, yBias1, "Размещение кораблей в [0 - ручн / 1 - авто ] режиме?:  ");

	fillField(battleField, 0, avto);
	fillField(battleField, 2, true, true);
		
	//printFields(battleField);
	//manualInputCoord();
	

	setPosition(0, rows*2);
	std::cout << "\n\n";
	return 0;
}
