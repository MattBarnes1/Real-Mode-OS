#include "Math.h"
#include "UtilityItems.h"
char Scrolling = 0;
int abackground = 0x0;
int aforeground = 0xA;
int ScreenTextColumns = 800/8;
int ScreenTextRows = 36;
int xCursor = 0;
int yCursor = 0;

void ShowCursor()
{


}


void HideCursor()
{


}

void setCursorPosition_Text(int X, int Y)
{	
	int AX = SetRegister(2, 0);
	int DX = SetRegister(Y, X);
	xCursor = X;
	yCursor = Y;	
	interrupt(16, AX, 0, 0,DX);
}

void ScrollDown_Text()
{
		interrupt(0x10, SetRegister(0x06, 1),SetRegister(abackground >> 4, 0),0, SetRegister(ScreenTextRows, ScreenTextColumns));
}

void IncrementYCursorCheck_Text()
{
	if(yCursor > ScreenTextRows)
	{
		yCursor--; //keep it on the right screen;
		xCursor = 0;		
		ScrollDown_Text();
	}	
	setCursorPosition_Text(xCursor, yCursor);
}



void writeCharacter_Text(char c)
{	
	int BX;
	int AX = SetRegister(9, c);
	if(c == '\0') return;
	if(c == '\r') return;
	if(c == '\n')
	{	
		xCursor = 0;
		yCursor++;
		IncrementYCursorCheck_Text();
		return;
	}
	interrupt(16, AX, SetRegister(0, abackground | aforeground), 1, 0);
	xCursor++;
	if(xCursor > ScreenTextColumns)
	{
		xCursor = 0;
		yCursor++;
		IncrementYCursorCheck_Text();
	} 
	setCursorPosition_Text(xCursor, yCursor);
}


void switchVideoMode_Text(int x)
{
	int AX = SetRegister(0x00,x);
	interrupt(0x10, AX, 0 , 0, 0);
}



void ScrollUp_Text()
{
	interrupt(0x10, SetRegister(0x07, 1),SetRegister(abackground >> 4, 0),0, SetRegister(ScreenTextRows, ScreenTextColumns));
	setCursorPosition_Text(xCursor, yCursor);	
}


void SetScreenColor_Text(char background, char foreground)
{
	abackground = background << 4;
	aforeground = foreground;
	
}


void ClearScreen_Text(char Background, char Foreground)
{	
	SetScreenColor_Text(Background,Foreground);
	interrupt(0x10, SetRegister(0x06, 0),SetRegister(abackground >> 4, 0),0, SetRegister(ScreenTextRows, ScreenTextColumns));
	setCursorPosition_Text(0,0);	
}

void printString_Text(char* c, int printval)
{	
	if (printval != 1)
	{

		while (*c != '\0')
		{
			writeCharacter_Text(*c);
			c++;
		}
	}
	else {
		while (*c != '\0')
		{
			interrupt(23, *c, 0, 0, 0);
			c++;
		}
	}
	return;
}


void writeInt_Text(int Int, int location)
{

	char *myReturn = itostr(Int);
	printString_Text(myReturn, location);

}

void readString_Text(char* CharArray)
{
	char * c = 0x0;
	int PointerPosition = 0;
	*c = interrupt(22, 0, 0, 0, 0);
	ShowCursor();
	if (*c == 0xD) {
		CharArray[0] = '\0';
		return;
	}
	while (*c != 0xD)
	{

		if (*c != 0x8)
		{
			CharArray[PointerPosition] = *c;
			PointerPosition++;
			writeCharacter_Text(*c);
		}
		else
		{
			writeCharacter_Text(*c);
			PointerPosition--;
			if (PointerPosition < 0)
			{
				PointerPosition = 0;
			}
		}
		*c = interrupt(22, 0, 0, 0, 0);
	}
	CharArray[PointerPosition] = '\0';
	HideCursor();
}

void readInt_Text(int *Int)
{
	char String[80];
	int returned;
	interrupt(33, 1, String, 0, 0);
	returned = stoi(String);
	*Int = returned;
}

void printScreenInformation()
{
	interrupt(33,0, "Screen Width:\0",0,0);
	interrupt(33,13, ScreenTextColumns,0,0);
	interrupt(33,0, "Screen Height:\0",0,0);
		interrupt(33,13, ScreenTextRows,0,0);


}
