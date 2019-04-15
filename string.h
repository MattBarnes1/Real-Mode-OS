/*Calculates the length of string*/
int strlen(char *String);
/*Copies a string to another string*/	
char *strcpy(char *dest, char *src);
/*Concatenates(joins) two strings*/	
char *strcat(char *dest, char *src);
/*Compares two strings
if Return value < 0 then it indicates str1 is less than str2.

if Return value > 0 then it indicates str2 is less than str1.

if Return value = 0 then it indicates str1 is equal to str2.*/
int strcmp(char *str1, char *str2);
/*Converts string to lowercase*/	
//strlwr()
/*Converts string to uppercase*/	
char *strupr(char *str);	
char *strchr(char *str, int c);
void memset(void *myArray, char myValue, int amount);