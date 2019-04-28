void readSector(char * array, int sector);
void writeSector(char *data, int sector);
void writeFile(char *FileName, char *FileContents, int SectorsToWrite);
void readFile(char *FileToReadFrom, char *FileStorageArea);
void deleteFile(char *FileNameToDelete);
char ** GetFiles();
