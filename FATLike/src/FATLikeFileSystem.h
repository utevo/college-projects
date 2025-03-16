#ifndef FATLIKE_FILESYSTEM_H
#define FATLIKE_FILESYSTEM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define BLOCK_SIZE 4096
#define MAX_NAME_LENGHT 32
#define MAX_FILE_COUNT 128


typedef struct  {
	size_t superBlocksOffset;
	size_t fileAllocationTableOffset;
	size_t rootDirectoryOffset;
	size_t dataOffset;

	size_t size;
	size_t blockSize;

	size_t allBlocksCount;
	size_t freeBlocksCount;

	size_t allDentryCount;
	size_t freeDentryCount;
} SuperBlock;


typedef struct {
	size_t size;
	time_t createTime, modificationTime;
	size_t indexOfFirstBlock;
	char name[MAX_NAME_LENGHT + 1];
	bool isValid;
} Dentry;


SuperBlock superBlock;
int *fileAllocationTable = NULL;
Dentry rootDirectory[MAX_FILE_COUNT];


int createVirtualDisk(size_t size);
int deleteVirtualDisk();

int copyFileFromPhysicalDisk(char *fileName);
int copyFileFromVirtualDisk(char *fileName);
int deleteFileFromVirtualDisk(char * fileName); 

void displayCatalog();
void displayFileSystemInformation(); 


int loadVirtualDisk();
int saveVirtualDisk();
void closeVirtualDisk();


bool isEnoughMemoryLeft(size_t size);
bool isFileOnVirtualDisk(char *fileName);

void deleteInfoFromFAT(size_t index);


int getIndexOfFreeBlock();
int getIndexOfFreeDentry();

size_t getFileSize(FILE* filePtr);

size_t sizeInBlocks(size_t size);

#endif //FATLIKE_FILESYSTEM_H
