


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
		} else {
			return i;
		}
	} 
	else {
		return 999;
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
