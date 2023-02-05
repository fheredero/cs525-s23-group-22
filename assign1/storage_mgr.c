#include "dberror.h"
#include "storage_mgr.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>



extern void initStorageManager(void){ //init

}

extern RC createPageFile (char *fileName){
    FILE *file = fopen(fileName, "w+");
	struct SM_FileHeader fHeader;
	fHeader.totalNumPages = 1
	fHeader.curPagePos = 0
	fwrite(&fHeader,sizeof(fHeader),1,file);
    char *charArray = malloc(PAGE_SIZE);
    int i = 0;
	while(i < numberOfChar){
	    charArray[i] = '\0';
		i++;
    }
	int write = fwrite(charArray, 1, PAGE_SIZE, file);
	fclose(file);
	if (write != PAGE_SIZE){ 			
	    return RC_WRITE_FAILED;			
	}
	return(RC_OK);
}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle){
    FILE *file = fopen(fileName, "r+"); // Opens a file for both reading and writing
    if(!file){   // If the file doesn't exist
        return RC_FILE_NOT_FOUND;   // File not found
    }
    struct SM_FileHeader fHeader;
    fread(&fHeader, sizeof(fHeader), 1, file);
    fHandle -> fileName = fileName;
    fHandle -> totalNumPages = fHeader.totalNumPages;
    fHandle -> curPagePos = 0;
    fHandle -> mgmtInfo = file;
    return RC_OK;
}

extern RC closePageFile(SM_FileHandle *fHandle) {
    file = fHandle->mgmtInfo; // Opens the file for both reading and writing
    if(!file){ // If the opened file is NULL 
        return RC_FILE_NOT_FOUND;   // File not found
    }   
    status = fclose(file); // Closes the file
    if(satus != 0){
        return RC_FILE_NOT_FOUND;
    } 
    return RC_OK;
}

extern RC destroyPageFile (char *fileName){
    FILE *file = fopen(fileName, "r+");
    if(!file){
        return RC_FILE_NOT_FOUND;
    }
    remove(file);   // Maybe add status
    return RC_OK;
}

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    if(fHandle->totalNumPages < pageNum || pageNum < 0){  // If the file has less than pageNum pages
        return RC_READ_NON_EXISTING_PAGE;   // Non existing page
    }
    file = fHandle->mgmtInfo; // Opens the file for both reading and writing
    if(!file){  // If file is NULL
        return RC_FILE_NOT_FOUND;   // File not found
    }
    int position = PAGE_SIZE * pageNum; // We declare the position as the size of one page times the page number
    if(fseek(file, position, SEEK_SET) != 0){ // If the seek is not successful (different than 0)
       return RC_READ_NON_EXISTING_PAGE;    // Non existing page
    } else { // If the seek is successful (equal to 0)
        fread(memPage, 1, PAGE_SIZE, file); // Read the page
    fHandle -> curPagePos = pageNum; // Update the current page position to the page number
    struct SM_FileHeader fHeader;
    fread(&fHeader, sizeof(fHeader), 1, file);
    fHeader.curPagePos = pageNum; 
    return RC_OK;   
}

extern int getBlockPos (SM_FileHandle *fHandle){ 
    return (fHandle -> curPagePos);
}

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    return readBlock(0, fHandle, memPage); // We might need to indicate 0 instead of 1 (lets try it)
}

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int position = getBlockPos(fHandle) - 1;
    if(position < 0){
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(position, fHandle, memPage);
}

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int position = getBlockPos(fHandle);
    return readBlock(position, fHandle, memPage);
}

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int position = getBlockPos(fHandle) + 1;
    if(position > fHandle -> totalNumPages){
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(position, fHandle, memPage);
}

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage{
    int numPages = fHandle -> totalNumPages;
    return readBlock(numPages, fHandle, memPage);
}

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    if(pageNum < 0 || pageNum > fHandle->totalNumPages){ // If pageNum is out of range
        return RC_READ_NON_EXISTING_PAGE;
    }
    file = fHandle->mgmtInfo; // Opens the file for both reading and writing
    if(!file){  // If file is NULL
        return RC_FILE_NOT_FOUND;
    }
    struct SM_FileHeader fHeader;
    fread(&fHeader, sizeof(fHeader), 1, file);
    int position = pageNum*PAGE_SIZE;
    fseek(file, position, SEEK_SET);
    int write = fwrite(memPage, 1, PAGE_SIZE, file);
    if(write != PAGE_SIZE){
        return RC_WRITE_FAILED;
    }
    fHandle -> curPagePos = pageNum;
    fHeader.curPagePos = pageNum;
    //fHandle -> totalNumPages++;
    return RC_OK;
}

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int position = getBlockPos(fHandle);
    return writeBlock(position, fHandle, memPage);
}

extern RC appendEmptyBlock (SM_FileHandle *fHandle){
    file = fHandle->mgmtInfo; // Opens the file for both reading and writing
    if(!file){ // If file is NULL
        return RC_FILE_NOT_FOUND;
    }
    int position = getBlockPos(fHandle);
    struct SM_FileHeader fHeader;
    fread(&fHeader, sizeof(fHeader), 1, file);
    fseek(file, fHandle->totalNumPages, SEEK_SET);
    char * charArray =  malloc(PAGE_SIZE);
    int i = 0;
    while(i < PAGE_SIZE){
        charArray[i] = '\0';
        i++;
    }
    int status = fwrite(charArray, 1, PAGE_SIZE, file);
    if(status != PAGE_SIZE){
        return RC_WRITE_FAILED;
    }
    fHandle -> totalNumPages++;
    fHeader.totalNumPages++;
    fseek(file, PAGE_SIZE*position, SEEK_SET); // We go back to the previous current position
    return RC_OK;
}

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
    file = fHandle->mgmtInfo; // Opens the file for both reading and writing
    if(!file){
        return RC_FILE_NOT_FOUND;
    }
    while(fHandle->totalNumPages < numberOfPages){
        RC status = appendEmptyBlock(fHandle);
        if(status != RC_OK){
            return RC_WRITE_FAILED;
        }
    }
    return RC_OK;
}





