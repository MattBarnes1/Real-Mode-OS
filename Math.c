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
