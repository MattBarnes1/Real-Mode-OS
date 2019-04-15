int SetRegister(char high, char low);
char *GetHex(int myInt);
void DumpToAscii(char *myDataStructure);
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

