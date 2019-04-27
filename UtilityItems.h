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

