#include "string.h"
//////////////////////
//Memory Manager
/////////////////////
unsigned char myBytes[3000]; //Temporary
struct Segment
{
	struct Segment *NextSegment;
	unsigned int ArrayStart;
	unsigned int ArrayEnd;
};
struct Segment *Head = 0;
struct Segment *Tail = 0;
struct Segment *Current = 0;

unsigned short Freespace = 3000;//4000;
unsigned short MaxSize = 3000;


void DumpMemory()
{
	int i;
	for(i = 0; i < MaxSize; i++)
	{
		interrupt(33, 17, myBytes[i] , 0, 0);
	}

}



unsigned int GetFreespace(unsigned short sizeInBytes)
{
	struct Segment *Previous = 0;
	if (Head == 0)
	{
		return (unsigned int)&myBytes[0];
	}
	else
	{
		do {
			if (Head->ArrayStart != (unsigned int)&myBytes[0])
			{//At some point head was deleted.
				if (Head->ArrayStart - (unsigned int)&myBytes[0] >= sizeInBytes)
				{
					return (unsigned int)&myBytes[0];
				}
			}

			if (Current->NextSegment != 0)
			{
				if (Current->ArrayEnd < Current->NextSegment->ArrayStart)
				{
					if (Current->NextSegment->ArrayStart + 1 - Current->ArrayEnd >= sizeInBytes)
					{
						return (unsigned int)Current->ArrayEnd + 1;
					}
				}
			}
			else
			{
				if ((unsigned int)Current->ArrayEnd + sizeInBytes + 1 < (unsigned int)&myBytes + MaxSize)
				{
					return (unsigned int)(Current->ArrayEnd + 1);
				}
			}
			Current = Current->NextSegment;
		} while (Current != 0);
	}
	return 0;
}

void *malloc(unsigned int SizeInBytes)
{
	int chunksize;
	unsigned int Start = GetFreespace(sizeof(struct Segment) + SizeInBytes);
	struct Segment *MyPtr;
	if (Start == 0 || Freespace == 0 || Freespace < (SizeInBytes + sizeof(struct Segment)))
	{
		interrupt(33, 0, "\nMalloc reports no freespace.\0", 0, 0);
		interrupt(33, 5,0,0,0);
		return 0;
	}
	else {
//		interrupt(33, 0, "\nMalloc created segment:\0", 0, 0);
//		interrupt(33, 13, Start, 0 , 0);
//		interrupt(33, 0, "\nSize:\0", 0, 0);
//		interrupt(33, 13, SizeInBytes, 0 , 0);
	}
	Current = Head;
	MyPtr = (struct Segment *)Start;
	chunksize = sizeof(struct Segment) + SizeInBytes;
	memset((void *)Start, '\0', chunksize);
	MyPtr->ArrayStart = (unsigned int)Start;
	MyPtr->ArrayEnd = (unsigned int)Start + chunksize;
	MyPtr->NextSegment = 0;
	if (Head == 0)
	{
		Current = MyPtr;
		Head = MyPtr;
		Tail = MyPtr;
	}
	else {
		Tail->NextSegment = MyPtr;
		Tail = Tail->NextSegment;
	}
	Current = MyPtr;
	Freespace -= (SizeInBytes + sizeof(struct Segment));
	return (void *)(Start + sizeof(struct Segment));
}

void free(void *myPtr)
{
	unsigned int MyValue = (unsigned int)((char *)myPtr - sizeof(struct Segment));
	struct Segment *Ptr = Head;
	struct Segment *Last = Head;
//		interrupt(33, 0, "\nMalloc destroyed segment:\0", 0, 0);
	//	interrupt(33, 13, MyValue, 0 , 0);
		

	while (Ptr->ArrayEnd > MyValue) //Implement search
	{
		Last = Ptr;
		Ptr = Ptr->NextSegment;
	}
	if(Ptr == 0)
	{
		interrupt(33,0, "PTR RETURNED NULL\0",0,0);
	} else {

		//interrupt(33, 0, "\nSize:\0", 0, 0);
		//interrupt(33, 13, (Ptr->ArrayEnd - Ptr->ArrayStart), 0 , 0);

	}
	if (Ptr == Head)
	{
		Head = Ptr->NextSegment;
	}
	else {
		Last->NextSegment = Ptr->NextSegment;
	}
	Freespace +=  Ptr->ArrayEnd - Ptr->ArrayStart;
	memset((void *)myPtr,'\0', (Ptr->ArrayEnd - Ptr->ArrayStart));
	myPtr = 0;
}
