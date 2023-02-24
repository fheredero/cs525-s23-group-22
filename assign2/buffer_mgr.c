#include "buffer_mgr_stat.h"
#include "buffer_mgr.h"

#include <stdio.h>
#include <stdlib.h>


RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
                   const int numPages, ReplacementStrategy strategy, void *stratData) {

    // Initialize buffer pool fields
    bm->pageFile = (char *)pageFileName;
    bm->numPages = numPages;
    bm->strategy = strategy;
    bm->mgmtData = malloc(sizeof(BM_BufferPool));
    BM_BufferPool *const bm_pool = (BM_BufferPool*)bm->mgmtData;
    bm_pool->pageTable = malloc(sizeof(PageTableEntry) * numPages);
    bm_pool->poolHandle = calloc(numPages, sizeof(SM_PageHandle));

    // Open page file
    FILE *fp;
    fp = fopen(pageFileName, "r+");
    if (fp == NULL) {
        return RC_FILE_NOT_FOUND;
    }

    // Initialize page table entries
    int i;
    for (i = 0; i < numPages; i++) {
        bm_pool->pageTable[i].pageNum = NO_PAGE;
        bm_pool->pageTable[i].frameNum = NO_FRAME;
        bm_pool->pageTable[i].isDirty = false;
        bm_pool->pageTable[i].fixCount = 0;
        bm_pool->pageTable[i].next = NULL;
    }

    // Set buffer pool fields based on replacement strategy
    switch (strategy) {
        case RS_FIFO:
            bm_pool->strategyData = initFIFOBufferPool(bm_pool);
            break;
        case RS_LRU:
            bm_pool->strategyData = initLRUBufferPool(bm_pool);
            break;
        default:
            return RC_INVALID_STRATEGY;
    }

    // Close page file
    fclose(fp);

    return RC_OK;
}
