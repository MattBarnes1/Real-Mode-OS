#include "malloc.h"

int SetRegister(char high, char low)
{
	int myReturn = 0;
	char *MyBase = &myReturn;
	MyBase[0] = low;
	MyBase[1] = high;
	return myReturn;
}


char IntConversion[7];
/* Converts ints to strings */
char * itostr(int myInt)
{
	//char *IntConversion = (char *)malloc(7);
	int anOldInt = myInt;
	int modulo = 10;
	int divReturn = 40;
	int i;
	char myVals[7];
	char myVal;
	int sizeCounter = 0;
	int lastChar = 0;
	char wasNeg = 0;
	if (myInt < 0)
	{
		wasNeg = 1;
		myInt *= -1;

	}


	while (divReturn != 0)
	{

		int returned = mod(anOldInt, modulo);
		IntConversion[lastChar++] = 0x30 + returned;
		divReturn = div(anOldInt, modulo);
		anOldInt = divReturn;
	}
	IntConversion[lastChar] = '\0';

	for (i = 0; i < lastChar; i++)
	{
		myVals[lastChar - i] = IntConversion[i];

	}
	for (i = 0; i < lastChar; i++)
	{
		IntConversion[i] = myVals[i + 1];
	}


	return IntConversion;
}


/* Stoi - String to Integer Conversion Function */
int stoi(char * myString)
{
	int myValue = 0;
	int Position = 0;
	char myChars[7];
	int i;
	int size;
	char c;
	for (size = 0; size < 7; size++)
	{
		if (myString[size] == '\0')
		{
			break;
		}
	}


	for (i = size - 1; i >= 0; i--)
	{
		myChars[size - 1 - i] = myString[i];

	}
	myChars[size] = '\0';

	while (myChars[Position] != '\0')
	{
		if (Position != 0)
		{
			myValue += (myChars[Position] - 0x30)*pow(10, Position);
		}
		else {
			myValue += (myChars[Position] - 0x30);
		}

		Position++;
	}

	return myValue;
}



void GetFarPointerData(int segment, int offset, char *fromRead, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		fromRead[i] = readFromMemory(segment, offset++);
	}
}
