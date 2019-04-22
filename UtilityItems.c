#include "malloc.h"


char GetHexChar(int Char)
{
	switch(Char)
	{
		case 0:
		return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
		default:
			return 'X';


	}
}


int SetRegister(char high, char low)
{
	int myReturn = 0;
	char *MyBase = &myReturn;
	MyBase[0] = low;
	MyBase[1] = high;
	return myReturn;
}

char *GetHex(int myInt)
{
	int Value = myInt;
	int size = (2*sizeof(int)) + 2;
	char *myReturn = (char *)malloc(size);
	int position = size - 1;	
	myReturn[0]='0';
	myReturn[1]='x';
	while(Value > 15)
	{
	  	int result = mod(Value ,16);
	  	Value = div(Value,16);
		myReturn[position] = GetHexChar(result);
		position--;
	}	
	myReturn[position--] = GetHexChar(Value);
	while(position > 1)
	{
		myReturn[position--] = '0';
	}
	return myReturn;
}

void DumpToAscii(char *myDataStructure)
{
	int i = 0;
	int DSSize = sizeof(myDataStructure);
	int finish  = div(DSSize,sizeof(char));
	char *MyEmptyArray = (char *)malloc(DSSize + 1);
	finish++;
	interrupt(33, 13, finish, 0,0);
	for(; i < finish - 1; i++)
	{
		MyEmptyArray[i] = myDataStructure[i];
	}
	MyEmptyArray[i+1] = '\0';
	
}

//char IntConversion[7];
/* Converts ints to strings */
char * itostr(int myInt)
{
	char *IntConversion = (char *)malloc(7);
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
