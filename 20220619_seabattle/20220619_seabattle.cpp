#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <time.h>


enum isAction {
	play = 0,
	save = 1000,
	quit = 2000
};

const int rows{ 10 }, cols{ 10 };
int xBias1{ 3 }, xBias2{ cols * 2 + xBias1 } , xBias3{ cols * 4 + 10 };
int yBias1{ 3 }, yBias2{ rows + 3 + yBias1 };
const int countTypeShips{ 4 };
int countHumShips{}, countAiShips{};
char around{ '.' };
char sankDeck{ 'X' };
isAction action{ play };

struct BattleShip {
	int countShip{};
	int countDecks{};
	char deck{ 'D' };
};

struct BS {
	int countDecks{};
	bool direct{}; // 0 - vertical, 1 - horizontal
	int xStart{};
	int yStart{};
	int status{};
	char deckSymbol{};
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

void clearScreen(int pause = 0)
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

void errMes(std::string message, int x, int y, ConsoleColor color = White, int pause = 0)
{
	showMesPos(message, x, y, color, pause);
	showMesPos(message, x, y, Black, 0);
}

int random(int min = 0, int max = 9)
{
	return rand() % (max - min + 1) + min;
}

void printSubField(BS** arr, int xCoord = 0, int yCoord = 0)
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
				char t = arr[j - 1][i - 1].deckSymbol;
				t != 0 && j < cols + 1 ? std::cout << "|" << t : std::cout << "| ";
			}
			else if(i > 0 && j==cols+1)
				std::cout << "|";
			++s;
		}
	}
}

void printFields(BS*** arr)
{	
	//clearScreen();
	printSubField(arr[0]);
	printSubField(arr[1], cols * 2 + 5);
	printSubField(arr[2], 0, rows + 3);
	printSubField(arr[3], cols * 2 + 5, rows + 3);
}

BS** genBattleField()
{
	BS** arr = new BS* [cols];

	for (int i{}; i < cols; ++i)
		arr[i] = new BS[rows]{};
		
	return arr;
}

bool checkFreeCeel(BS** arr, int x, int y)
{
	return arr[x][y].deckSymbol == 0 ? true : false;
}

bool isFreeCells(BS** arr, const int& cDeck, const int& x, const int& y, const int& direct, bool avto = true)
{
	int coord = direct ? x : y;
	int sizeF = direct ? cols : rows;

	if ((coord + cDeck - 1) > (sizeF - 1) && cDeck > 1)
	{
		if (!avto)
			errMes("НЕ РАЗМЕСТИТЬ!!!", xBias3, yBias1 - 2, Red, 1500);
		return false;
	}
	for (int i{};i < cDeck; ++i)	
		if (!checkFreeCeel(arr, direct ? x + i : x, direct ? y : y + i))
		{
			if (!avto)
				errMes("НЕ РАЗМЕСТИТЬ!!!", xBias3, yBias1 - 2, Red, 1500);
			return false;
		}
	return true;
}

void aroundShip(BS** arr, int xS, int yS)
{
	int t = arr[xS][yS].direct ? (arr[xS][yS].xStart + arr[xS][yS].countDecks) : (arr[xS][yS].yStart + arr[xS][yS].countDecks);
	int x = arr[xS][yS].xStart;
	int y = arr[xS][yS].yStart;
	if (arr[xS][yS].direct)
	{
		for (int i{ x - 1 }; i < t + 1; ++i)
		{
			(y - 1) < 0 || i < 0 || i >= cols ? NULL : arr[i][y - 1].deckSymbol = around;
			(y + 1) >= rows || i < 0 || i >= cols ? NULL : arr[i][y + 1].deckSymbol = around;
		}
		(x - 1) < 0 ? NULL : arr[x - 1][y].deckSymbol = around;
		t >= cols ? NULL : arr[t][y].deckSymbol = around;
	}
	else 
	{
		for (int i{ y - 1 }; i < t + 1; ++i)
		{
			(x - 1) < 0 || i < 0 || i >= rows ? NULL : arr[x - 1][i].deckSymbol = around;
			(x + 1) >= cols || i < 0 || i >= rows ? NULL : arr[x + 1][i].deckSymbol = around;
		}
		(y - 1) < 0 ? NULL : arr[x][y - 1].deckSymbol = around;
		t >= rows ? NULL : arr[x][t].deckSymbol = around;
	}	
}

void genXYcoord(int& x, int& y)
{
	x = random(0, cols - 1);
	y = random(0, rows - 1);
}

void aiArrangeCoord(BS** arr, int& cDeck, int& selX, int& selY, int& direct)
{
	direct = random(0, 1); // 0 - vertical, 1 - horizontal	
	do {
		genXYcoord(selX, selY);
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
			clearScreen();
	}
	return inp;
}

bool isIterrupt(std::string coord)
{
	if (coord == "save")
	{
		action = save;
		return true;
	}
	else if (coord == "exit")
	{
		action == quit;
		return true;
	}
	return false;
}

void manualInputCoord(int& x, int& y, int xBias, int yBias, std::string message )
{	
	std::string coord;
	do {		
		do {			
			showMesPos(message, xBias, yBias);			
			showMesPos("      ", xBias + message.length(), yBias);
			setPosition(xBias + message.length(), yBias);
			std::cin >> coord;
			if (isIterrupt(coord))
				return;
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

void battleShipArrange(BS*** arr , int& sel, int& cDeck, bool avto)
{
	int direct{}; // 0 - vertical, 1 - horizontal	
	int selX{}, selY{}, x{}, y{};
	if (avto)	
		aiArrangeCoord(arr[sel], cDeck, selX, selY, direct);		
	else
	{
		do {
			clearScreen();
			printSubField(arr[sel]);
			std::string text = "Координаты [XY] для " + std::to_string(cDeck) + (cDeck == 1 ? "-но" : "-x") + " палубного: ";
			manualInputCoord(selX, selY, xBias3, yBias1, text);
			direct = cDeck == 1 ? 0 : isYesNo(xBias3, (yBias1 + 2), "Расположение [0 - верт. / 1 - гориз.] корабля?: ", 0);
		} while (!isFreeCells(arr[sel], cDeck, selX, selY, direct, avto));		
	}
	
	for (int i{};i < cDeck; ++i)
	{		
		x = direct ? selX + i : selX;
		y = direct ? selY : selY + i;
		arr[sel][x][y].deckSymbol = arrShips->deck;
		arr[sel][x][y].countDecks = cDeck;
		arr[sel][x][y].direct = direct;
		arr[sel][x][y].xStart = selX; 
		arr[sel][x][y].yStart = selY; 		
		arr[sel][x][y].status = cDeck;
	}	
	aroundShip(arr[sel], selX, selY);
}

void removeAround(BS** arr)
{
	for (int i{}; i < cols; ++i)
		for (int j{}; j < rows; ++j)
			arr[i][j].deckSymbol != around ? NULL : arr[i][j].deckSymbol = 0;
}

int fillField(BS*** arr ,int sel, bool avto)
{
	int counter{};
	for (int i{}; i < countTypeShips; ++i)
		for (int j{}; j < arrShips[i].countShip; ++j)
		{
			battleShipArrange(arr, sel, arrShips[i].countDecks, avto);
			++counter;
		}
	removeAround(arr[sel]);
	return counter;
}

bool isSankShip(BS** arr, const int x, const int y)
{		
	--arr[x][y].status;
	if (arr[x][y].status == 0)		
		return true;
	
	if (arr[x][y].direct)	
		for (int i{ arr[x][y].xStart }; i < (arr[x][y].xStart + arr[x][y].countDecks); ++i)
		{
			if (arr[i][y].deckSymbol == sankDeck)
				continue;
			--arr[i][y].status;
		}	
	else if(!arr[x][y].direct)
		for (int i{ arr[x][y].yStart }; i < (arr[x][y].yStart + arr[x][y].countDecks); ++i)
		{
			if (arr[x][i].deckSymbol == sankDeck)
				continue;
			--arr[x][i].status;
		}	
	return false;
}

bool isWinner()
{
	if (countAiShips == 0)
	{
		showMesPos("ПОБЕДИЛ ИГРОК!!!", xBias3, yBias1 - 2, Red, 1500);
		clearScreen();
		return true;
	}
	if (countHumShips == 0)
	{
		showMesPos("ПОБЕДИЛ ИИ!!!", xBias3, yBias1 - 2, Red, 1500);
		clearScreen();
		return true;
	}
	return false;
}

void saveCoord(const int& x, const int& y, const int& sanked)
{
	if (sanked)
		return;
}

void aiGenXY(int& x, int& y)
{
	genXYcoord(x, y);

}

bool attackMove(BS*** arr, const int& defField, const int& attField)
{	
	bool sanked{ false };
	bool move{ true };

	std::string text = "Координаты пуска торпеды: ";
	
	do {		
		int x{}, y{};
		attField == 0 ? manualInputCoord(x, y, xBias3, yBias1, text) : aiGenXY(x,y);
		if (action != 0)
			return true;

		if (checkFreeCeel(arr[defField], x, y) && arr[attField + 1][x][y].deckSymbol != around)
		{
			arr[attField + 1][x][y].deckSymbol = around;
			arr[defField][x][y].deckSymbol = around;
			printFields(arr);
			errMes("Не попал!!!", xBias3 + text.length(), yBias1, Red, 1500);
			move = false;
		}
		else
		{
			if (arr[attField + 1][x][y].deckSymbol == around)
			{				
				errMes("Сюда пуск был!!!", xBias3 + text.length(), yBias1, Red, 1500);				
				continue;
			}
			else
			{				
				arr[defField][x][y].deckSymbol = sankDeck;
				arr[attField + 1][x][y] = arr[defField][x][y];
				printFields(arr);
				errMes("Попал!!!", xBias3 + text.length(), yBias1, Red, 1500);					
				sanked = isSankShip(arr[defField], x, y);
				attField == 2 ? saveCoord(x, y, sanked) : srand(time(0));
			}
			if (sanked)
			{
				aroundShip(arr[attField + 1], x, y);
				aroundShip(arr[defField], x, y);
				printFields(arr);
				errMes("Затопил!!!", xBias3 + text.length(), yBias1, Red, 1500);
				attField == 0 ? --countAiShips : --countHumShips;
				if (isWinner()) 					
					return true;
				sanked = false;
			}
		}
	} while (move);
	return false;
}

void fighting(BS*** arr)
{
	int hFd{ 0 }, aFd{ 2 };
	int i{2};
	while (true)
	{		
		if (attackMove(arr, (i % 2) ? hFd : aFd, (i % 2) ? aFd : hFd))
			break;
		++i;		
	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(0));
	rand();

	action = play;
	
	BS** humanFieldSelf = genBattleField();
	BS** humanFieldBattle = genBattleField();
	BS** aiFieldSelf = genBattleField();
	BS** aiFieldBattle = genBattleField();

	BS*** battleField = new BS** [4]{ humanFieldSelf, humanFieldBattle, aiFieldSelf, aiFieldBattle };
	
	bool avto = isYesNo(xBias1, yBias1, "Размещение кораблей в [0 - ручн / 1 - авто ] режиме?:  ");

	countHumShips =  fillField(battleField, 0, avto);
	countAiShips = fillField(battleField, 2, true);
		
	printFields(battleField);
	
	fighting(battleField);


	//setPosition(0, rows*2);
	std::cout << "\n\n";
	return 0;
}
