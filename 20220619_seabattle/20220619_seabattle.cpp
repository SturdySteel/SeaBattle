#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <time.h>



const int rows{10},cols{ 10 };



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

int random(int min =0, int max = 9)
{
	return rand() % (max - min + 1) + min;
}


void printSubField(int** arr, int xCoord = 0, int yCoord = 0)
{
	char s = 65;
	short maxSymbol = s + cols;

	for (int i{}; i < rows + 1; ++i)
	{
		setPosition(xCoord, yCoord + i);
		for (int j{}; j < cols + 2; ++j)
		{
			if (i == 0 && s < maxSymbol)
				j == 0 ? std::cout << "   " << s : std::cout << ' ' << s;
			else if (j == 0)
				i < 10 ? std::cout << ' ' << i : std::cout << i;
			else if (i != 0)
			{
				char t = arr[i - 1][j - 1];
				t != 0 && j < cols + 1 ? std::cout << "|" << t : std::cout << "| ";
			}
			++s;
		}
	}
}

void printRules()
{
	setPosition(cols * 3 + 10,1);

}

void printBattleShip()
{

}

void printFields(int*** arr )
{	
	printSubField(arr[0]);
	printSubField(arr[1], cols * 2 + 5);
	printSubField(arr[2], 0, rows + 3);
	printSubField(arr[3], cols * 2 + 5, rows + 3);
}

int** genBattleField()
{
	int** arr = new int* [rows];

	for (int i{}; i < rows; ++i)
		arr[i] = new int[cols] {};
		
	return arr;
}


int main()
{
	setlocale(LC_ALL, "ru");
		
	int** humanFieldSelf = genBattleField();
	int** humanFieldBattle = genBattleField();
	int** aiFieldSelf = genBattleField();
	int** aiFieldBattle = genBattleField();

	int*** battleField = new int**[4]{ humanFieldSelf, humanFieldBattle, aiFieldSelf, aiFieldBattle };

	humanFieldSelf[0][2] = 72;
	humanFieldBattle[3][3] = 72;
	aiFieldSelf[5][5] = 72;
	aiFieldBattle[9][9] = 72;

	printFields(battleField);
	

	std::cout << "\n\n";
	return 0;
}
