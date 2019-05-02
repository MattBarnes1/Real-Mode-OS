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

void switchVideoMode_Text(int x);
void setCursorPosition_Text(int row, int column);
void ScrollDown_Text();
void clearScreen(char Background, char Foreground);
void ScrollUp_Text();
void writeInt_Text(int Int, int location);
void SetScreenColor_Text(int background, int foreground);
void printString_Text(char* c, int printval);
void readString_Text(char* CharArray);
void readInt_Text(int *Int);
void writeCharacter_Text(char c);
void handleInterrupt21(int, int, int, int);
void printLogo();
void PrintRegisters(int, int, int, int);
void error(int myErrorCode);
void runProgram(int, int);
void stop();
void readSector(char * array, int sector);
void writeSector(char *data, int sector);
void writeFile(char *FileName, char *FileContents, int SectorsToWrite);
void readFile(char *FileToReadFrom, char *FileStorageArea);
void deleteFile(char *FileNameToDelete);
char ** GetFiles();
int mod(int a, int b);
int div(int a, int b);
int pow(int base, int exponent);
unsigned int divu(unsigned int a, unsigned int b);
int strlen(char *String);
char *strcpy(char *dest, char *src);
void memset(void *myArray, char myValue, int amount);
int SetRegister(char high, char low);
/* Converts ints to strings */
char * itostr(int myInt);
/* Stoi - String to Integer Conversion Function */
int stoi(char * myString);
void GetFarPointerData(int segment, int offset, char *fromRead, int size);
typedef struct
{
	unsigned int Address;
	unsigned int Segment;
} FARPTR;

void main()
{
	int i;
	//xCursor = 0;
//yCursor = 0;
 	makeInterrupt21();
	switchVideoMode_Text(0x6A);
clearScreen(0x01, 0x0B);
		/*for(i = 0; i < 100; i++)
		{
			if(i != 99)
			{
				printString_Text("k\0", 0);
			} else {
				printString_Text("x\0", 0);

			}

		}
		for(i = 0; i < 37; i++)
		{
				printString_Text("\nk\0", 0);
		}*/

	//printLogo();	
	interrupt(33,4,"Shell\0",2,0);
	interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);
	while (1);
	stop();
}


void KeyboardInterrupt()
{
	//KeyboardChar = In(0x60);	
}


 int abackground = 0x0;
int aforeground = 0xA;
int ScreenTextColumns = 99;
int ScreenTextRows = 36;
int xCursor = 0;
int yCursor = 1;

void ShowCursor()
{


}

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

void HideCursor()
{


}

void setCursorPosition_Text(int X, int Y)
{	
    	int AX;
    	int DX;	
	if(X == ScreenTextColumns)
	{
		X = 0;
		Y++;
	} 
	if(Y == ScreenTextRows && Y > 0)
	{
		Y--; //keep it on the right screen;
		X = 0;		
		ScrollDown_Text();
	}
	
	xCursor = X;
	yCursor = Y;
	AX = SetRegister(2, 0);
	DX = SetRegister(Y, X);
	interrupt(16, AX, 0, 0,DX);
}

void ScrollDown_Text()
{
	writeInt_Text(SetRegister(abackground, aforeground),1);
	interrupt(0x10, SetRegister(0x06, 1), SetRegister(abackground, aforeground),0, SetRegister(ScreenTextRows, ScreenTextColumns));
}







/*Calculates the length of string*/

int strlen(char *String)
{
	int count = 0;
	while(*String != 0) 
	{
		count++;
		String++;
	}
	return count;
}
/*Copies a string to another string*/	
char *strcpy(char *dest, char *src)
{
	char *OldDes = dest;
	int i = 0;		
	for(i; i < strlen(src) + 1; i++)
	{			
		OldDes[i] = src[i];			
	}
	return OldDes;
}
/*
/*Concatenates(joins) two strings*/	
/*char *strcat(char *dest, char *src)
{		
	int i = strlen(dest);
	int DestSize = sizeof(dest);
	int SourceSize = sizeof(src);
	char *RetVal = malloc(DestSize + SourceSize);
	strcpy(RetVal, dest);
	for(i; i < strlen(src) + 1; i++)
	{
		dest[strlen(dest) + i] = src[i];	
	}
	dest = RetVal; 
	return dest;
}*/
/*Compares two string*/	
/*
if Return value > 1 then it indicates str1 is less than str2.

if Return value > 0 then it indicates str2 is less than str1.

if Return value = 0 then it indicates str1 is equal to str2.
*/

int strcmp(char *str1, char *str2)
{
	int i = 0;
	int g = 0;
	char nomatch = 0;
	int counter;
	int str2Counter = 0;
	int str1Len = strlen(str1);
	int str2Len = strlen(str2);
	if(str1Len >= str2Len) //find smalles
		counter = str2Len;
	else 
		counter = str1Len;

	if(counter == 0) return 1;
	//interrupt(33,13,counter,1,0);
	for(i; i < counter; i++)
	{
		if(str1[i] != str2[i])
		{
			nomatch  = 1;
			break;
		}
	}
	if(nomatch == 0)
	{
		if(str1Len == str2Len)
		{
			return 0;
		} 
		else if(str1Len < str2Len)
		{
			return 1;
		} else {
			return 2;
		}
	} 
	else {
		return 1;
	}

}
/*
char *strstr(char *str1, char *str2)
{


}
*/

void memset(void *myArray, char myValue, int amount)
{
	char *myPtr = (char *)myArray;
	int i;
	for(i= 0; i < amount; i++)
	{
		myPtr[i] = myValue;
	}
}





/* Modulo Operator */
int mod(int a, int b)
{	
	int x = a;
	while (x >= b) x = x - b;
	return x;
}

int div(int a, int b)
{
	int q = 0;	
	if(a < b) return 0;
	while (q*b <= a) q++;
	return q - 1;
}

unsigned int divu(unsigned int a, unsigned int b)
{
	unsigned int q = 0;	
	if(a < b) return 0;
	while (q*b <= a) q++;
	return q - 1;
}


/* Pow - takes the base to the power of the exponent*/
int pow(int base, int exponent)
{
	int myReturn = 1;
	int i = 0;
	for (i; i < exponent; i++)
	{
		myReturn = myReturn * base;
	}
	return myReturn;

}


struct DiskMap
{
	char Sectors[512];
};

struct DirectoryEntry
{
	char EntryName[8];
	char ResidentSectors[24];
};


struct DiskDirectory
{
	struct DirectoryEntry Entries[16];
};

void readSector(char * array, int sector)
{
	int read;
	int AX = SetRegister(0x2, 1);
	int headNumber = mod(div(sector, 18), 2);
	int relativesector = mod(sector, 18) + 1;
	int trackNumber = div(sector, 36);
	read = interrupt(19, AX, array, trackNumber * 256 + relativesector, headNumber * 256);//SetRegister(trackNumber, relativesector), SetRegister(headNumber, 0));

	if(read != 1)
	{
		interrupt(33, 15,22,0,0);
	}
}




void writeSector(char *data, int sector)
{
	int AX = 0x301;
	int relativesector = mod(sector, 18) + 1;
	int headNumber = mod(div(sector, 18), 2);
	int trackNumber = div(sector, 36);
	interrupt(19, AX, data, trackNumber * 256 + relativesector, headNumber * 256);
}



int FindNextAvailableSector(struct DiskMap *myDiskMap, int startat)
{
	int i = startat;
	for(; i < 256; i++)
	{
		if(myDiskMap->Sectors[i] == 0x00)
		{
			interrupt(33,13,myDiskMap->Sectors[i],1,0);
			return i;
		}
	}
	return 0;
}

/*
Write a disk file. bx 8 bxAddress of
character array
containing the
file name.
cxAddress of
character array
where file contents
will be stored.
dx Tdotal
number of
sectors to
be written.
Call writeFile(bx,cx,dx)
*/
void writeFile(char* name, char* buffer, int numberOfSectors)
{
	int i;
	int g;
	int zeroOffset = 0;
	char Sectors2[512];
  	char Sectors[512];
	char *BufferCopyFrom = buffer;

	int lastSector = 0;	
	struct DiskMap *myDisk;
	struct DiskDirectory *DiskDir;	
	readSector(Sectors2, 256);
	myDisk = (struct DiskMap *)Sectors2;
	readSector(Sectors, 257);
	DiskDir = (struct DiskDirectory*)Sectors;


	if(name == 0 || name[0] == '\0' || strlen(name) > 8) interrupt(33,15,1, 0,0); //Bad File Name


	for(i = 0; i < 16; i++)
	{
		if(DiskDir->Entries[i].EntryName[0] != 0)
		{
			if(strcmp(DiskDir->Entries[i].EntryName, name) == 0)
			{
				interrupt(33,15,3,0,0);				//easy case, search for sectors;
			}
		} 
		else if (zeroOffset == 0)
		{
			zeroOffset = i; //the offset we know is empty;
		}
	}
	if(zeroOffset == 0) interrupt(33,15,2,0,0);
	memset(DiskDir->Entries[zeroOffset].EntryName, '\0', 7);
	strcpy(DiskDir->Entries[zeroOffset].EntryName, name);	
	for(g = 0; g < numberOfSectors; g++)
	{
		lastSector = FindNextAvailableSector(myDisk, lastSector);
		if(lastSector == 0)
		{
			interrupt(33,15,2,0,0);
		} else {
			myDisk->Sectors[lastSector] = 0xFF;
			DiskDir->Entries[zeroOffset].ResidentSectors[g] = lastSector;
			writeSector(BufferCopyFrom,lastSector);	
			BufferCopyFrom += 512;
		}
	}	
		
	writeSector( DiskDir , 257);
	writeSector(myDisk, 256);

}


/*Read a disk file. ax 3 
bx = Address of
character array
containing the
file name.
cx = Address of
character array
where file contents
will be stored.
dx = Total
number of
sectors to
read.*/
void readFile(char* fname, char* buffer, int* size)
{
	int i;
	int g;
	char Sectors[512];	
	struct DiskDirectory *MyData;// = GetDiskDirectory();
	readSector(Sectors, 257);
	MyData = (struct DiskDirectory*)Sectors;

	for(i = 0; i < 16; i++)
	{
		if(strcmp(MyData->Entries[i].EntryName, fname) == 0)
		{
			for(g= 0; g < 24; g++)
			{
				if(MyData->Entries[i].ResidentSectors[g] == 0)
				{		
					return;
				} else {
					readSector(buffer, MyData->Entries[i].ResidentSectors[g]);
					buffer += 512;
					(*size)++;
				}
			}


			return;	
		} 
	}

	interrupt(33, 15, 0, 0,0);
}

	char StringsInner[16][9];// = (char **)malloc(16 * sizeof(char *));

char ** GetFiles()
{	
	int i = 0;
	int cCount = 0;
	char myChars[2];
	char Sectors[512];
	struct DiskDirectory *MyData;
	readSector(Sectors, 257);
	MyData = (struct DiskDirectory*)Sectors;
	myChars[1] = '\0';
	for(i = 0; i < 16; i++)
	{
		if(MyData->Entries[i].EntryName[0] != 0)
		{

			//StringsInner[cCount] = (char *)malloc(9);
			strcpy(StringsInner[cCount], MyData->Entries[i].EntryName);
			//printString_Text(StringsInner[cCount], 0);
			StringsInner[cCount][8] = 0;
			cCount++;
		} else if(cCount != 15)
		{
			cCount++;
		
			StringsInner[cCount][0] = 0;
		} 
	}	

	return StringsInner;
}



/*Delete a disk file. ax7 bxAddress of
character array
containing the
file name.
*/
void deleteFile(char *FileNameToDelete)
{

	int g;
	int i;
	int zeroOffset = 0;
	struct DiskDirectory *DiskDir;// = GetDiskDirectory();	
  	struct DiskMap *myDisk;
	char Sectors[512];// = (char *)malloc(512);
  	char Sectors2[512];// = (char *)malloc(512);
	readSector(Sectors2, 256);
	myDisk = (struct DiskMap *)Sectors2;
	readSector(Sectors, 257);
	DiskDir = (struct DiskDirectory*)Sectors;
	for(i = 0; i < 16; i++)
	{	

		if(strcmp(DiskDir->Entries[i].EntryName, FileNameToDelete) == 0)
		{
			memset(DiskDir->Entries[i].EntryName, 0, 7);
			for(g = 0; g < 24; g++)
			{
				if(DiskDir->Entries[i].ResidentSectors[g] != 0)
				{
					myDisk->Sectors[DiskDir->Entries[i].ResidentSectors[g]] = 0x00; //Free!
					DiskDir->Entries[i].ResidentSectors[g] = 0x00;
				}
			}
			
			writeSector( DiskDir , 257);;
			writeSector(myDisk, 256);
			//free(DiskDir);
			//free(myDisk);
			//free(Sectors);
			return;
		}
	}

	interrupt(33,15, 0,0,0);
}




void writeCharacter_Text(char c)
{	
	int AX = SetRegister(0x9, c);
	if(c == '\0') return;
	if(c == '\r') return;
	if(c == '\n')
	{	
		xCursor = 0;
		yCursor++;
		setCursorPosition_Text(xCursor, yCursor);
	}
	else {
		xCursor++;
		interrupt(16, AX, SetRegister(0, abackground | aforeground),1, 0);
		setCursorPosition_Text(xCursor, yCursor);
	}
}


void switchVideoMode_Text(int x)
{
	int AX = SetRegister(0x00,x);
	interrupt(0x10, AX, 0 , 0, 0);
}



void ScrollUp_Text()
{
	interrupt(0x10, SetRegister(0x07, 1),SetRegister(abackground | aforeground, 0),0, SetRegister(ScreenTextRows, ScreenTextColumns));
	setCursorPosition_Text(xCursor, yCursor);	
}


void SetScreenColor_Text(int background, int foreground)
{
	abackground = background;
	aforeground = foreground;
	
}


void clearScreen(char Background, char Foreground)
{	
    int BX;
    if(Background <= 8 && Foreground <= 16 && Background > 0 && Foreground > 0)
    {    
   printString_Text("Before Setting BACK\n\0", 1);
	   writeInt_Text(abackground, 1);
   printString_Text("\n\0", 1);
	   writeInt_Text(aforeground, 1);  
        BX = SetRegister(Background-1, Foreground-1);  
   printString_Text("\nFirst Setting BACK\n\0", 1);
	   writeInt_Text(BX, 1); 
        SetScreenColor_Text(Background-1,Foreground-1);  
   printString_Text("\nAFter Setting BACK\n\0", 1);
	   writeInt_Text(abackground, 1);
printString_Text("\n\0", 1);
	   writeInt_Text(aforeground, 1);  
   printString_Text("Finish\n\0", 1);
		setCursorPosition_Text(0,0);
  printString_Text("\nFirst Setting BACK2\n\0", 1);
   printString_Text("\nAFter Setting BACK\n\0", 1);
	   writeInt_Text(abackground, 1);
printString_Text("\n\0", 1);
	   writeInt_Text(aforeground, 1);
printString_Text("\n\0", 1);
        interrupt(0x10, SetRegister(0x06, 0),BX,0, SetRegister(ScreenTextRows, ScreenTextColumns));
        setCursorPosition_Text(0,0);	   
    }
}

void printString_Text(char* c, int printval)
{	
	int AX;
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
			xCursor--;
			setCursorPosition_Text(xCursor, yCursor);
			writeCharacter_Text(' ');
			setCursorPosition_Text(--xCursor, yCursor);
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


void error(int myErrorCode)
{
	clearScreen(0x2, 0x8);
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
	char Data[4096];
	char *Test = Data;
	int i = 0, g = 0;
	int size = 0;
	readFile(bx, Data,&size);
	for(i = 0; i < (size * 512); i++)
	{
		putInMemory(0x1000*cx, i, Data[i]);
	}
	launchProgram(0x1000*cx);

}

void stop()
{

	while(1);

}



void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	int  i=0;
	char *change;
	aforeground = 0x0A;
	abackground = 0x00;
	switch (ax) {
	case 0:
   printString_Text("fore\n\0", 1);
	writeInt_Text(aforeground, 1);
   printString_Text("back\n\0", 1);
	writeInt_Text(abackground, 1);
   printString_Text("\n\0", 1);
		//change = (char *)bx;
    		printString_Text( bx, cx);
		break;
	case 1:
printString_Text("Read String Called\n\0", 1);
		readString_Text(bx);
		break;
	case 2:
//printString_Text("readSector Called\n\0", 1);
		readSector(bx, cx);
		break;
	case 3:
//printString_Text("readFile Called\n\0", 1);
		readFile(bx, cx, dx);
		break;
	case 4:

//printString_Text("runProgram Called\n\0", 1);
		runProgram(bx, cx);
		break;
	case 5:
//printString_Text("stop Called\n\0", 1);
		stop();
		break;
	case 6:
//printString_Text("writeSector Called\n\0", 1);
		writeSector(bx, cx);
		break;
	case 7:
//printString_Text("deleteFile Called\n\0", 1);
		deleteFile(bx);
		break;

 	case 8:
//        printString_Text("writeFile Called\n\0", 1);
		writeFile(bx,cx,dx);
		break;
	 case 9:
//        printString_Text("Int 9 called\n\0", 1);
		break; 
case 10:
///        printString_Text("Int 10 called\n\0", 1);
		break;
	case 11:
//        printString_Text("Int 11 called\n\0", 1);
		interrupt(25,0,0,0,0);
		break;
	case 12:
		printString_Text("Stuff happened", 1);
		clearScreen(bx, cx);
		break;
	case 13:
	//	printString_Text("writeInt_Text Called\n\0", 1);

	aforeground = 0x0A;
	abackground = 0x00;
        	writeInt_Text(bx, cx);
		break;
	case 14:
       // printString_Text("readInt Called\n\0", 1);	
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
	case 18:
printString_Text("writeCharacter_Text Called\n\0", 1);
		GetFiles();
		for(i = 0; i < 16; i++)
		{
			if(StringsInner[i][0] != 0 && StringsInner[i][0] > 0x60)
			{
				printString_Text("\n\n\0",0);
				printString_Text(StringsInner[i], 0);
				printString_Text("\n\n\0",0);
				strcpy(((char *)bx)[i], StringsInner[i]);
			}
		}
		break;
	default: 
printString_Text("Empty Called\n\0", 1);
		break;
	}
}
