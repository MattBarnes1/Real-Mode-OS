/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:_____________________________________ Date:_____________        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2018.             */

#include "Math.h"
#include "DiskIO.h"
#include "malloc.h"
#include "string.h" 
#include "UtilityItems.h"
void switchVideoMode_Text(int x);
void setCursorPosition_Text(int row, int column);
void ScrollDown_Text();
void ClearScreen_Text(char Background, char Foreground);
void ScrollUp_Text();
void writeInt_Text(int Int, int location);
void SetScreenColor_Text(int background, int foreground);
void printString_Text(char* c, int printval);
void readString_Text(char* CharArray);
void readInt_Text(int *Int);
void writeCharacter_Text(char c);
void handleInterrupt21(int, int, int, int);
void printLogo();
//void KeyboardInterrupt();
void PrintRegisters(int, int, int, int);
void DebugPrintRegisters();
void error(int myErrorCode);
//https://courses.cs.washington.edu/courses/cse457/98a/tech/OpenGL/font.c
void runProgram(int, int);
void stop();

void (*setTextColor)(int, int);

void printString_Text(char *String, int Print);

void main()
{
    int i = 0;
    int g = 0;
	char *Test = "hello!\0";
 	makeInterrupt21();
	setTextColor = SetScreenColor_Text;
	switchVideoMode_Text(0x6A);
	//clearScreen(0x02, 0x0B);
	ClearScreen_Text(0x01, 0x0B);
	
	printLogo();	
	interrupt(33,4,"kitty1\0",3,0);

	
	stop();
}


void KeyboardInterrupt()
{
	//KeyboardChar = In(0x60);	
}


char Scrolling = 0;
int abackground = 0x0;
int aforeground = 0xA;
int ScreenTextColumns = 100;
int ScreenTextRows = 39;
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
    	int AX;
    	int DX;
	if(xCursor > ScreenTextRows)
	{
		xCursor = 0;
		yCursor++;
	} 
	if(yCursor > ScreenTextColumns)
	{
		yCursor--; //keep it on the right screen;
		xCursor = 0;		
		ScrollDown_Text();
	}
	AX = SetRegister(2, 0);
	DX = SetRegister(Y, X);
	xCursor = X;
	yCursor = Y;	
	interrupt(16, AX, 0, 0,DX);
}

void ScrollDown_Text()
{
	interrupt(0x10, SetRegister(0x06, 1), SetRegister(abackground, 0),0, 	SetRegister(ScreenTextRows, ScreenTextColumns));
}

void IncrementYCursorCheck_Text()
{
	
	setCursorPosition_Text(xCursor, yCursor);
}



void writeCharacter_Text(char c)
{	
	int BX;
	int AX = SetRegister(0xE, c);
	if(c == '\0') return;
	if(c == '\r') return;
	if(c == '\n')
	{	
		xCursor = 1;
		yCursor++;
	}
	else {
		xCursor++;
		interrupt(16, AX, SetRegister(abackground | aforeground, abackground | aforeground), 0, 0);
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
	interrupt(0x10, SetRegister(0x07, 1),SetRegister(abackground, 0),0, SetRegister(ScreenTextRows, ScreenTextColumns));
	setCursorPosition_Text(xCursor, yCursor);	
}


void SetScreenColor_Text(char background, char foreground)
{
	abackground = background;
	aforeground = foreground;
	
}


void ClearScreen_Text(char Background, char Foreground)
{	
    int BX;
    if(Background <= 8 && Foreground <= 16 && Background > 0 && Foreground > 0)
    {  
        BX = SetRegister(Background-1, Foreground-1);   
        SetScreenColor_Text(Background-1,Foreground-1);
		setCursorPosition_Text(0,0);
        interrupt(0x10, SetRegister(0x06, 0),BX,0, SetRegister(ScreenTextRows, ScreenTextColumns));
        setCursorPosition_Text(0,0);	   
    }
}

void printString_Text(char* c, int printval)
{	
	if (printval == 0)
	{

		while (*c != '\0')
		{
			writeCharacter_Text(*c);
			c++;
		}
	}
	else 
    {
		while (*c != '\0')
		{
		//	writeCharacter_Text(*c);
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
    //free(myReturn);
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


void printLogo()
{
	printString_Text("       ___   `._   ____  _            _    _____   ____   _____ \r\n\0", 0);
	printString_Text("      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0", 0);
	printString_Text("     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0", 0);
	printString_Text("    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0", 0);
	printString_Text("   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0", 0);
	printString_Text("._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0", 0);
	printString_Text(" BlackDOS2020 v. 0.00.0.1, c. 2018. Based on a project by M. Black. \r\n\0", 0);
	printString_Text(" Author(s): Matthew Barnes.\r\n\r\n\0", 0);
}

/* MAKE FUTURE UPDATES HERE */
/* VVVVVVVVVVVVVVVVVVVVVVVV */


/*void HandleMouse(int X, int Y, int ButtonsPressed)
{


}*/

void error(int myErrorCode)
{
	ClearScreen_Text(0x2, 0x8);
	printString_Text("Fatal Error Detected:\n\0", 0);
	switch(myErrorCode)
	{
		case 0:
			printString_Text("File Not Found!\0", 0); 
		break;
		case 1:
			printString_Text("Bad File Name!\0", 0);
		break;
		case 2:
			printString_Text("Disk full!\0", 0);
		break;		
		case 3:
			printString_Text("File Exists!\0", 0);
		break;

		default:
			printString_Text("General Error!\0", 0);
		break;
	}
	stop();
}


/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

	

void runProgram(int bx, int cx)
{
	char *Data = (char *)malloc(2000);
	char *Test = Data;
	int i = 0, g = 0;
	int size = 0;
	readFile(bx, Data,&size);
	/*for(i = 0; i < (size * 512); i++)
	{
		putInMemory(0x1000*cx, i, Data[i]);
	}*/
	free(Data);
	while(1);
	launchProgram(&Data);

}

void stop()
{

	while(1);

}



void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	switch (ax) {
	case 0:
    printString_Text("Print String Called\n\0", 1);
		printString_Text(bx, cx);
		break;
	case 1:
printString_Text("Read String Called\n\0", 1);
		readString_Text(bx);
		break;
	case 2:
printString_Text("readSector Called\n\0", 1);
		readSector(bx, cx);
		break;
	case 3:
printString_Text("readFile Called\n\0", 1);
		readFile(bx, cx, dx);
		break;
	case 4:

printString_Text("runProgram Called\n\0", 1);
		runProgram(bx, cx);
		break;
	case 5:
printString_Text("stop Called\n\0", 1);
		stop();
		break;
	case 6:
printString_Text("writeSector Called\n\0", 1);
		writeSector(bx, cx);
		break;
	case 7:
printString_Text("deleteFile Called\n\0", 1);
		deleteFile(bx);
		break;

 	case 8:
        printString_Text("writeFile Called\n\0", 1);
		writeFile(bx,cx,dx);
		break;
	 case 9:
        printString_Text("Int 9 called\n\0", 1);
		break; 
case 10:
        printString_Text("Int 10 called\n\0", 1);
		break;
	case 11:
        printString_Text("Int 11 called\n\0", 1);
		break;
	case 12:
		printString_Text("Stuff happened", 1);
		ClearScreen_Text(bx, cx);
		break;
	case 13:
	//	printString_Text("writeInt_Text Called\n\0", 1);
        	writeInt_Text(bx, cx);
		break;
	case 14:
        printString_Text("readInt Called\n\0", 1);	
		readInt_Text(bx);
		break;
	case 15:
		printString_Text("error Called\n\0", 1);
		error(bx);
		break;
	case 16:
printString_Text("Int 16\n\0", 1);
		break;
	case 17:
printString_Text("writeCharacter_Text Called\n\0", 1);
		writeCharacter_Text(bx);
		break;
	default: 
printString_Text("Empty Called\n\0", 1);
		break;
	}
}
