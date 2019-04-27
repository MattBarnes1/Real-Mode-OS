#include "string.h"
#include "malloc.h"
#include "UtilityItems.h"


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

struct DiskDirectory *GetDiskDirectory()
{
	char *Sectors = (char *)malloc(sizeof(struct DiskDirectory));
	readSector(Sectors, 257);
	return (struct DiskDirectory*)Sectors;
}


struct DiskMap *GetDiskMap()
{
	char *Sectors = (char *)malloc(sizeof(struct DiskMap));
	readSector(Sectors, 256);
	return (struct DiskMap *)Sectors;
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
	char *BufferCopyFrom = buffer;

	int lastSector = 0;	
	struct DiskMap *myDisk = GetDiskMap();
	struct DiskDirectory *DiskDir;// = GetDiskDirectory();	
  	char *Sectors = (char *)malloc(512);//[512];
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
	free(DiskDir);
	free(Sectors);
	free(myDisk);

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
	struct DiskDirectory *MyData = GetDiskDirectory();
	for(i = 0; i < 16; i++)
	{
		if(strcmp(MyData->Entries[i].EntryName, fname) == 0)
		{
			for(g= 0; g < 24; g++)
			{
				if(MyData->Entries[i].ResidentSectors[g] == 0)
				{		
					free(MyData);	
					return;
				} else {
					readSector(buffer, MyData->Entries[i].ResidentSectors[g]);
					buffer += 512;
					(*size)++;
				}
			}


			free(MyData);
			return;	
		} 
	}

	free(MyData);
	interrupt(33, 15, 0, 0,0);
}


/*
void SetupDisk(int *Free, int *Used)
{
	int i;
	struct DiskMap *myMap = GetDiskMap();
	*Free = 0;
	*Used = 0;
	for(i = 0; i < 256; i++)
	{
		if(myMap->Sectors[i] == 0xFF)
		{
			(*Used)++;
		}	
		else
		{
			(*Free)++;
		}
	}
	myFreeSpace = *Free;
	myUsedSpace = *Used;
	free(myMap);
}

*/
char ** GetFiles()
{	
	int i = 0;
	int cCount = 0;
	char myChars[2];
	char **StringsInner = (char **)malloc(16 * sizeof(char *));
	struct DiskDirectory *MyData = GetDiskDirectory();
	myChars[1] = '\0';
	for(i = 0; i < 16; i++)
	{
		if(MyData->Entries[i].EntryName[0] != 0)
		{

			StringsInner[cCount] = (char *)malloc(9);
			strcpy(StringsInner[cCount], MyData->Entries[i].EntryName);

			StringsInner[cCount][8] = 0;
			cCount++;
		} 
	}
	if(cCount != 15)
	{
		cCount++;
		StringsInner[cCount] = 0;
	}	
	free(MyData);
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
  	char Sectors = (char *)malloc(512);
	struct DiskMap *myDisk= GetDiskMap();
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
			free(DiskDir);
			free(myDisk);
			free(Sectors);
			return;
		}
	}

	interrupt(33,15, 0,0,0);
}

