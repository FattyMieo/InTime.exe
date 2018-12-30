/* 
	Console Input/Output written by Nicholas Tan.
	Last update : 14/11/2016
	conio_nt : version 1.0.1
*/

#include <windows.h>

enum COLOR
{
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKCYAN,
	DARKRED,
	DARKMAGENTA,
	DARKYELLOW,
	GRAY,
	DARKGRAY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	YELLOW,
	WHITE
};

static COLOR foregroundColor = GRAY;
static COLOR backgroundColor = BLACK;

void SetFontColor(COLOR color);						// Changes the color of the text.
void SetBackgroundColor(COLOR color);				// Changes the color behind the text.
void GoToXY(int x, int y);							// Sets the cursor to the position you want.
int GetCursorPositionX();							// Get the cursors current X position.
int GetCursorPositionY();							// Get the cursors current Y position.
void SetCursorType(int type);						// Sets the size and visibility of the cursor
void HideCursor();									// Hides the cursor.
void ShowCursor();									// Shows the cursor.
void ClrScreen();									// Clear the entire screen.
void ClrLine();										// Clear the current line the cursor is on.
void SetDisplayMode(int mode);						// Full screen - 1 / Window mode - 2
void SetWindowSize(short width, short height);		// Sets the size of the console.
void SetQuickEditMode(bool b);						// Quick Edit Mode: Disable to prevent mouse clicks from interrupting the program
int hexToInt(char hex);								// Change hex to int
char intToHex(int i);                               // Change int to hex
int rand_int(int min, int max);                     // Generate a random integer between two numbers
