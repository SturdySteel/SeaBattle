#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <time.h>



int rows{10},cols{ 10 };

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

void printSubField(int xCoord)
{
	char s = 65;
	short maxSymbol = s + cols;

	for (int i{}; i < rows + 1; ++i)
	{
		setPosition(xCoord, i + 1);
		for (int j{}; j < cols + 2; ++j)
		{
			if (i == 0 && s < maxSymbol)
				j == 0 ? std::cout << "   " << s : std::cout << ' ' << s;
			else if (j == 0)
				i < 10 ? std::cout << ' ' << i : std::cout << i;
			else if (i != 0)
				std::cout << "| ";
			++s;
		}
	}
}

void printFields()
{	
	printSubField(0);
	printSubField(cols*2 + 5);

}


int main()
{
	setlocale(LC_ALL, "ru");

	//setPosition(0, 5);

    //showMessage("Hello World!", Red, 1000);
	printFields();

	std::cout << "\n\n";

    return 0;
}
