/*
	Console Input/Output written by Nicholas Tan.
	Last update : 14/11/2016
	conio_nt : version 1.0.1
*/

#include "conio_nt.h"


void SetFontColor(COLOR color)
{
	foregroundColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (backgroundColor << 4));
}

void SetBackgroundColor(COLOR color)
{
	backgroundColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foregroundColor + (color << 4));
}

void GoToXY(int x, int y)
{
	COORD coordinate;

	coordinate.X = x;
	coordinate.Y = y ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

int GetCursorPositionX()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition.X;
}

int GetCursorPositionY()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition.Y;
}

void SetCursorType(int type)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = type;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ShowCursor()
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = true;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ClrScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	COORD coordinate;
	DWORD consoleSize;
	DWORD written;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	coordinate.X = 0;
	coordinate.Y = 0;
	consoleSize = info.dwSize.X * info.dwSize.Y;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', consoleSize, coordinate, &written);
	FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info.wAttributes, consoleSize, coordinate, &written);
	GoToXY(0, 0);
}

void ClrLine()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	COORD coordinate;
	DWORD written;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	coordinate.X = info.dwCursorPosition.X;
	coordinate.Y = info.dwCursorPosition.Y;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', info.dwSize.X * info.dwCursorPosition.Y, coordinate, &written);
	GoToXY(info.dwCursorPosition.X, info.dwCursorPosition.Y);
}

void SetDisplayMode(int mode)
{
	COORD coordinate;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), 2, &coordinate);
}

void SetWindowSize(short width, short height)
{
	SMALL_RECT windowSize;
	COORD largestSize;
	COORD coordinate;

	largestSize = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

	if (largestSize.X < width) width = largestSize.X - 2;
	if (largestSize.Y < height) height = largestSize.Y - 2;

	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = width - 1;
	windowSize.Bottom = height - 1;
	
	coordinate.X = width;
	coordinate.Y = height;

	if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordinate)) OutputDebugString("Failed to set screen buffer size");
	else OutputDebugString("Buffer Size Success");

	if(!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize)) OutputDebugString("Failed to set window size");
	else OutputDebugString("Window Size Success");

}

void SetQuickEditMode(bool b)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hIn, &dwMode);
	if(!b)
		dwMode &= ~ENABLE_QUICK_EDIT_MODE;
	else
		dwMode |= ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hIn, dwMode | ENABLE_EXTENDED_FLAGS);
}

int hexToInt(char hex)
{
	if(hex == '0') return 0;
	if(hex == '1') return 1;
	if(hex == '2') return 2;
	if(hex == '3') return 3;
	if(hex == '4') return 4;
	if(hex == '5') return 5;
	if(hex == '6') return 6;
	if(hex == '7') return 7;
	if(hex == '8') return 8;
	if(hex == '9') return 9;
	if(hex == 'A') return 10;
	if(hex == 'B') return 11;
	if(hex == 'C') return 12;
	if(hex == 'D') return 13;
	if(hex == 'E') return 14;
	if(hex == 'F') return 15;
}

char intToHex(int i)
{
	if(i == 0) return '0';
	if(i == 1) return '1';
	if(i == 2) return '2';
	if(i == 3) return '3';
	if(i == 4) return '4';
	if(i == 5) return '5';
	if(i == 6) return '6';
	if(i == 7) return '7';
	if(i == 8) return '8';
	if(i == 9) return '9';
	if(i == 10) return 'A';
	if(i == 11) return 'B';
	if(i == 12) return 'C';
	if(i == 13) return 'D';
	if(i == 14) return 'E';
	if(i == 15) return 'F';
}

int rand_int(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}
