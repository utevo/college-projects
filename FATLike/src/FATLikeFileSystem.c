#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>


#include "FATLikeFileSystem.h"


int createVirtualDisk(size_t size) {

	size_t numberOfBlocks = sizeInBlocks(size);

	printf("numberOfBlocks = %ld\n", numberOfBlocks);

	size_t sizeOfVirtualDisk = 0;

	sizeOfVirtualDisk += sizeof(SuperBlock);
	sizeOfVirtualDisk += sizeof(int) * numberOfBlocks;
	sizeOfVirtualDisk += sizeof(Dentry) * MAX_FILE_COUNT; 
	sizeOfVirtualDisk += BLOCK_SIZE * numberOfBlocks;

	FILE* virtualDiskPtr = fopen("FATLIKE", "wb");

    if (!virtualDiskPtr){
        printf("Error. Failed to open VirtualDisk\n");

        return 3;
    }

	truncate("FATLIKE", sizeOfVirtualDisk);

	fclose(virtualDiskPtr);


	superBlock.superBlocksOffset = 0;
	superBlock.fileAllocationTableOffset = sizeof(SuperBlock);
	superBlock.rootDirectoryOffset = superBlock.fileAllocationTableOffset + sizeof(int) * numberOfBlocks;
	superBlock.dataOffset = superBlock.rootDirectoryOffset + sizeof(Dentry) * MAX_FILE_COUNT;
	superBlock.size = sizeOfVirtualDisk;
    superBlock.blockSize = BLOCK_SIZE;

	superBlock.allBlocksCount = numberOfBlocks;
	superBlock.freeBlocksCount = numberOfBlocks;

	superBlock.allDentryCount = MAX_FILE_COUNT;
	superBlock.freeDentryCount = MAX_FILE_COUNT;

	/* *** */

	fileAllocationTable = malloc(sizeof(int) * superBlock.allBlocksCount );

	if (!fileAllocationTable) {
        
        printf("Error. Failed to allocate memory for fileAllocationTable\n");

        return 3;
	}

	for (size_t i = 0; i < superBlock.allBlocksCount; ++i)
		fileAllocationTable[i] = -2;

	/* *** */

	for (size_t i = 0; i < superBlock.allDentryCount; ++i)
		rootDirectory[i].isValid = false;

	/* *** */

	printf("Virtual Disk was created!\n");
	return 0;
}

int deleteVirtualDisk()
{
	if (remove("FATLIKE") == 0) {
    	printf("Virtual Disk deleted\n");
    	return 0;
    }
  	else {
    	printf("Error. Can't delete VirtualDisk\n");
    	return 1;
  	}
}

int copyFileFromPhysicalDisk(char *fileName) {
	
    FILE *filePtr, *virtualDiskPtr;


    if (superBlock.freeDentryCount == 0 ) {
    	printf("The maximum number of files has been reached\n");

    	return 6;
    }

    size_t lengthOfFileName = strlen(fileName);

    if (lengthOfFileName >= MAX_NAME_LENGHT){
        printf("Error. File name too long\n");
        return 1;
    }

    if (isFileOnVirtualDisk(fileName) ) {
        printf("Error. File of that name is already in VirtualDisk\n");

        return 4;
    }


    filePtr = fopen(fileName, "rb");
    virtualDiskPtr = fopen("FATLIKE", "r+b");

    if (!filePtr) {
        printf("Error. Failed to open file %s\n", fileName);
        return 2;
    }

    if (!virtualDiskPtr) {
        printf("Error. Failed to open VirtualDisk\n");

        fclose(filePtr);
        return 3;
    }

    size_t sizeOfFile = getFileSize(filePtr);

    if (!isEnoughMemoryLeft(sizeOfFile)) {
    	printf("Error. Not enought memory left\n");

        fclose(filePtr);
        fclose(virtualDiskPtr);
        return 5;
    }


    int indexOfBlock = getIndexOfFreeBlock();

    size_t blockAdress;
    size_t howManyRead;
    size_t howManyMoreToWrite = sizeOfFile;
    char blockTmp[BLOCK_SIZE];

    Dentry newDentry;

    newDentry.size = sizeOfFile;
    newDentry.createTime = newDentry.modificationTime = time(NULL);
    newDentry.indexOfFirstBlock = indexOfBlock;
    strcpy(newDentry.name, fileName);
    newDentry.name[lengthOfFileName] = 0;



    blockAdress = superBlock.dataOffset + indexOfBlock * BLOCK_SIZE;
    howManyRead = fread(&blockTmp, 1, BLOCK_SIZE, filePtr);


    howManyMoreToWrite -= howManyRead;

    if (howManyRead != BLOCK_SIZE) {

    	if (howManyMoreToWrite != 0) {
    		printf("Error. Problem with read one\n");

        	fclose(filePtr);
        	fclose(virtualDiskPtr);
    		return 6;
    	}
    }

	fseek(virtualDiskPtr, blockAdress, SEEK_SET);
	if (fwrite(&blockTmp, 1, howManyRead, virtualDiskPtr) != howManyRead) {
		printf("Error. Problem with write one\n");

        fclose(filePtr);
        fclose(virtualDiskPtr);
		return 7;
	}



    while (howManyMoreToWrite != 0)
    {
    	fileAllocationTable[indexOfBlock] = -1;
    	indexOfBlock = fileAllocationTable[indexOfBlock] = getIndexOfFreeBlock();

	    blockAdress = superBlock.dataOffset + indexOfBlock * BLOCK_SIZE;
	    howManyRead = fread(&blockTmp, 1, BLOCK_SIZE, filePtr);

	    howManyMoreToWrite -= howManyRead;

	    if (howManyRead != BLOCK_SIZE) {

	    	if (howManyMoreToWrite != 0) {
	    		printf("Error. Problem with read\n");

	    		fileAllocationTable[indexOfBlock] = -1;
	    		deleteInfoFromFAT(newDentry.indexOfFirstBlock);

	    		fclose(filePtr);
        		fclose(virtualDiskPtr);
	    		return 6;
	    	}
	    }

		fseek(virtualDiskPtr, blockAdress, SEEK_SET);
		if (fwrite(&blockTmp, 1, howManyRead, virtualDiskPtr) != howManyRead) {
			printf("Error. Problem with write\n");

	    	fileAllocationTable[indexOfBlock] = -1;
	    	deleteInfoFromFAT(newDentry.indexOfFirstBlock);


            printf("\n");
			fclose(filePtr);
        	fclose(virtualDiskPtr);
			return 7;
		}

    }



    fileAllocationTable[indexOfBlock] = -1;
    newDentry.isValid = true;

    size_t indexOfDentry = getIndexOfFreeDentry();

    rootDirectory[indexOfDentry] = newDentry;
    --superBlock.freeDentryCount;
    superBlock.freeBlocksCount -= sizeInBlocks(sizeOfFile);


    printf("File has been copied from Physical Disk\n");
	fclose(filePtr);
    fclose(virtualDiskPtr);
    return 0;
}

int copyFileFromVirtualDisk(char *fileName) {

	FILE *filePtr, *virtualDiskPtr;

    size_t lengthOfFileName = strlen(fileName);

    if (lengthOfFileName >= MAX_NAME_LENGHT){
        printf("Error. File name too long\n");
        return 1;
    }

    if (!isFileOnVirtualDisk(fileName) ) {
        printf("Error. File of that name is not in VirtualDisk\n");

        return 4;
    }


    filePtr = fopen(fileName, "wb");
    virtualDiskPtr = fopen("FATLIKE", "rb");

    if (!filePtr){
        printf("Error. Failed to open file %s\n", fileName);
        return 2;
    }

    if (!virtualDiskPtr){
        printf("Error. Failed to open VirtualDisk\n");

        fclose(filePtr);
        return 3;
    }

    size_t indexOfDentry;
    int indexOfBlock;
    size_t howManyToRead;
    size_t blockAdress;
    char blockTmp[BLOCK_SIZE];
    size_t sizeOfFile;
    size_t howManyMoreToWrite;


	for (size_t i = 0; i < superBlock.allDentryCount; ++i)
		if (rootDirectory[i].isValid)
			if (strcmp(rootDirectory[i].name, fileName) == 0)
				indexOfDentry = i;


	indexOfBlock = rootDirectory[indexOfDentry].indexOfFirstBlock;

	sizeOfFile = rootDirectory[indexOfDentry].size;
	howManyMoreToWrite = sizeOfFile;

    blockAdress = superBlock.dataOffset + indexOfBlock * BLOCK_SIZE;
	fseek(virtualDiskPtr, blockAdress, SEEK_SET);

	if (howManyMoreToWrite < BLOCK_SIZE)
		howManyToRead = howManyMoreToWrite;
	else
		howManyToRead = BLOCK_SIZE;


   	if (fread(&blockTmp, 1, howManyToRead, virtualDiskPtr) != howManyToRead) {
   		printf("Error. Problem with read data\n");

   		fclose(filePtr);
   		fclose(virtualDiskPtr);
   		return 4;
   	}

   	if (fwrite(&blockTmp, 1, howManyToRead, filePtr) != howManyToRead) {
   		printf("Error. Problem with qrite data\n");

   		fclose(filePtr);
   		fclose(virtualDiskPtr);
   		return 5;
   	}

   	howManyMoreToWrite -= howManyToRead;

    while (fileAllocationTable[indexOfBlock] != -1) {

		indexOfBlock = fileAllocationTable[indexOfBlock];

	    blockAdress = superBlock.dataOffset + indexOfBlock * BLOCK_SIZE;
		fseek(virtualDiskPtr, blockAdress, SEEK_SET);

		if (howManyMoreToWrite < BLOCK_SIZE)
			howManyToRead = howManyMoreToWrite;
		else
			howManyToRead = BLOCK_SIZE;

	   	if (fread(&blockTmp, 1, howManyToRead, virtualDiskPtr) != howManyToRead) {
	   		printf("Error. Problem with read data\n");

	   		fclose(filePtr);
	   		fclose(virtualDiskPtr);
	   		return 4;
	   	}

	   	if (fwrite(&blockTmp, 1, howManyToRead, filePtr) != howManyToRead) {
	   		printf("Error. Problem with qrite data\n");

	   		fclose(filePtr);
	   		fclose(virtualDiskPtr);
	   		return 5;
	   	}

	   	howManyMoreToWrite -= howManyToRead;
    }


    rootDirectory[indexOfDentry].modificationTime = time(NULL);

    printf("File has been copied from Virtual Disk\n");
	fclose(filePtr);
    fclose(virtualDiskPtr);
    return 0;

}

int deleteFileFromVirtualDisk(char * fileName) {

	FILE *filePtr, *virtualDiskPtr;
    size_t lengthOfFileName = strlen(fileName);

	if (lengthOfFileName >= MAX_NAME_LENGHT){
        printf("Error. File name too long\n");
        return 1;
    }

    if (!isFileOnVirtualDisk(fileName) ) {
        printf("Error. File of that name is not in VirtualDisk\n");
        return 4;
    }

    virtualDiskPtr = fopen("FATLIKE", "rb");

    if (!virtualDiskPtr){
        printf("Error. Failed to open VirtualDisk\n");

        fclose(filePtr);
        return 3;
    }

    size_t indexOfDentry;

	for (size_t i = 0; i < superBlock.allDentryCount; ++i)
		if (rootDirectory[i].isValid)
			if (strcmp(rootDirectory[i].name, fileName) == 0)
				indexOfDentry = i;

	int indexOfFirstBlock = rootDirectory[indexOfDentry].indexOfFirstBlock;
	size_t sizeOfFile = rootDirectory[indexOfDentry].size;

	rootDirectory[indexOfDentry].isValid = false;


	deleteInfoFromFAT(indexOfFirstBlock);

	superBlock.freeBlocksCount += sizeInBlocks(sizeOfFile);
	superBlock.freeDentryCount += 1;

    printf("File has been deleted from Virtual Disk\n");
	fclose(filePtr);
	return 0;
}


void displayFileSystemInformation() {

	printf( "size = %ld\n"
            "blockSize = %ld\n"
			"allBlocksCount = %ld\n"
			"freeBlocksCount = %ld\n"
			"allDentryCount = %ld\n"
			"freeDentryCount = %ld\n", superBlock.size, superBlock.blockSize, superBlock.allBlocksCount, 
            superBlock.freeBlocksCount, superBlock.allDentryCount, superBlock.freeDentryCount );

}

void displayCatalog() {
    printf("Root Catalog:\n");
	for (size_t i = 0; i < superBlock.allDentryCount; ++i)
		if (rootDirectory[i].isValid == true)
			printf( "name = %s, "
					"size = %ld, "
					"CT = %ld, "
					"MT = %ld\n", rootDirectory[i].name, rootDirectory[i].size, rootDirectory[i].createTime,
										rootDirectory[i].modificationTime);
}

int loadVirtualDisk(){

	FILE* virtualDiskPtr = fopen("FATLIKE", "rb");

	if (!virtualDiskPtr) {
    	printf("Error. Can't open VirtualDisk\n");
        return 1;
	}

	/* *** */

	if (fread(&superBlock, sizeof(SuperBlock), 1, virtualDiskPtr) != 1) {

        printf("Error. Failed to read superBlock\n");

        fclose(virtualDiskPtr);
        return 2;
	}

	/* *** */

	fileAllocationTable = malloc(sizeof(int) * superBlock.allBlocksCount );

	if (!fileAllocationTable) {
        
        printf("Error. Failed to allocate memory for fileAllocationTable\n");

        fclose(virtualDiskPtr);
        return 3;
	}

	if (fread(fileAllocationTable, sizeof(int), superBlock.allBlocksCount, virtualDiskPtr) !=
			superBlock.allBlocksCount ) {
		
		printf("Error. Failed to read fileAllocationTable\n");
        
        free(fileAllocationTable);
        fclose(virtualDiskPtr);
        return 4;
	}

	/* *** */

	if (fread(rootDirectory, sizeof(Dentry), MAX_FILE_COUNT, virtualDiskPtr) != MAX_FILE_COUNT) {

        printf("Error. Failed to read rootDirectory\n");

        free(fileAllocationTable);
        fclose(virtualDiskPtr);
        return 5;
	}

	/* *** */

	//printf("load complete\n");
	fclose(virtualDiskPtr);
	return 0;
}


int saveVirtualDisk() {

	FILE* virtualDiskPtr = fopen("FATLIKE", "r+b");

	if (!virtualDiskPtr) {
    	printf("Error. Can't open VirtualDisk\n");
        return 1;
	}

	/* *** */

	if (fwrite(&superBlock, sizeof(SuperBlock), 1, virtualDiskPtr) != 1) {

        printf("Error. Failed to write superBlock\n");

        fclose(virtualDiskPtr);
        return 2;
	}

	/* *** */

	if (fwrite(fileAllocationTable, sizeof(int), superBlock.allBlocksCount, virtualDiskPtr) !=
			superBlock.allBlocksCount ) {
		
		printf("Error. Failed to save fileAllocationTable\n");
        
        fclose(virtualDiskPtr);
        return 4;
	}

	/* *** */

	if (fwrite(rootDirectory, sizeof(Dentry), MAX_FILE_COUNT, virtualDiskPtr) != MAX_FILE_COUNT) {

        printf("Error. Failed to write rootDirectory\n");

        fclose(virtualDiskPtr);
        return 5;
	}

	/* *** */

	//printf("save complete\n");
	fclose(virtualDiskPtr);
	return 0;
}


void closeVirtualDisk() {

	if (fileAllocationTable != NULL)
		free(fileAllocationTable);

}



int getIndexOfFreeBlock() {

	for (size_t i = 0; i < superBlock.allBlocksCount; ++i)
		if (fileAllocationTable[i] == -2)
			return i;

	return -1;
}

int getIndexOfFreeDentry() {

	for (size_t i = 0; i < superBlock.allDentryCount; ++i)
		if (rootDirectory[i].isValid == false)
			return i;

	return -1;
}


bool isFileOnVirtualDisk(char *fileName) {

	for (size_t i = 0; i < superBlock.allDentryCount; ++i)
		if (rootDirectory[i].isValid)
			if (strcmp(rootDirectory[i].name, fileName) == 0)
				return true;

	return false;
}

bool isEnoughMemoryLeft(size_t size) {

	if (superBlock.freeBlocksCount * BLOCK_SIZE >= size)
		return true;

	return false;
}

size_t getFileSize(FILE* filePtr) {
    fseek(filePtr, 0, SEEK_END);
    size_t size = ftell(filePtr);

    rewind(filePtr);
    return size;
}

size_t sizeInBlocks(size_t size) {

	return (size - 1) / BLOCK_SIZE + 1;
}

void deleteInfoFromFAT(size_t index) {

	do {
		int tmp = fileAllocationTable[index];
		fileAllocationTable[index] = -2;
		index = tmp;
	} while (index != -1);
}


int main(int argc, char* argv[]) {

    if(argc<2) {
        printf("Error. Wrong usage!\n");
        return 1;
    }

    switch(argv[1][0]) {


        case 'c' :
            if(argc != 3 || atoi(argv[2]) == 0)
                return 3;
            
            createVirtualDisk(atoi(argv[2]) );
            saveVirtualDisk();
            closeVirtualDisk();
            return 0;

        
        case 'v' :
            if(argc != 3)
                return 2;
            if (loadVirtualDisk() )
            	return 1;
            if (copyFileFromPhysicalDisk(argv[2]) == 0)
				saveVirtualDisk();
            closeVirtualDisk();
            return 0;
         

        case 'm' :
            if(argc != 3) 
                return 2;

            if (loadVirtualDisk() )
            	return 1;
            if (copyFileFromVirtualDisk(argv[2]) == 0)
            	saveVirtualDisk();
			closeVirtualDisk();
            return 0;


        case 'd' :
            if(argc != 3)
                return 3;

            if (loadVirtualDisk() )
                return 1;
            if (deleteFileFromVirtualDisk(argv[2]) == 0)
                saveVirtualDisk();
            closeVirtualDisk();
            return 0;
     

        case 'l' :
            if (loadVirtualDisk() )
            	return 1;
            displayCatalog();
			closeVirtualDisk();
            return 0;


        case 'i' :
            if (loadVirtualDisk() )
            	return 1;
            displayFileSystemInformation();
            closeVirtualDisk();
            return 0;


        case 'r' :
            return deleteVirtualDisk();
		  

        default :
        	printf("Error. incorrect command\n");
            return 2;
    }

}
