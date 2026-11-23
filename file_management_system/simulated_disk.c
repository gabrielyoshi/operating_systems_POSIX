#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512 // 512 bytes in each block
#define NUM_SECTORS 100 // a disk with an array of 100 sectors
#define DIR_ENTRIES 25

// state variables
int currentDirectory; // sector number

// Type definitions
// simulated disk storage
union block {
    struct{ /* directory layout */
        int back; // block indices 4 bytes each
        int frwd; 
        int free;
        struct {
            // U: regular file, D: subdirectory, F:unused entry.
            char type; // entry type 1 byte
            char name[9]; // 9 bytes
            int link; // 4 bytes
            short size; // 2 bytes + 4 bytes of padding
        } dir[DIR_ENTRIES]; // 20 bytes
    } directory;
    struct { /* file-data layout */
        int backf; // file-data indices 4 bytes each
        int frwdf;
        char userdata[504]; // file contents
    } data; // 512 bytes
};

struct nodeType {
    int info; // free sector number
    struct nodeType *link; // points to next free sector node
};

typedef struct nodeType nodeType;

nodeType *freeList = NULL; // points to 1st node

// file metadata table
union block sector[NUM_SECTORS];

// function prototypes
void buildFreeList(void);
void initialize_disk(void);
void releaseFreeList(void);
bool startupSizeTest(void);
void initializeDirectory(int, int, int);

void buildFreeList(void) {
    releaseFreeList(); // safely releasing any existing list
    freeList = NULL; // marks end of the list
    for (int i = 1; i < NUM_SECTORS; i++) {
        nodeType *newNode = malloc(sizeof(*newNode)); // allocate node
        newNode->info = i; // initializes free sector number of node
        newNode->link = freeList; // places node at head
        freeList = newNode; // points to new head
    }
    // default 99 -> 98 -> ... -> 1 -> NULL
}

// Release dynamically allocated FreeList nodes
void releaseFreeList(void) {
    while (freeList != NULL) {
        nodeType *next = freeList->link;
        free(freeList);
        freeList = next;
    }
}

bool startupSizeTest(void) {
    if (sizeof(sector[0].directory.dir[0]) != 20 ||
    sizeof(union block) != 512 || 
    sizeof(sector) != 51200) {
        fprintf(stderr, "Invalid disk structure size.\n");
        return false;
    }
    return true;
}

void initialize_disk(void){
    if (!startupSizeTest()) return;
    // initialize reserved root directory
    initializeDirectory(0, 0, 0);
    // initialize FreeList
    buildFreeList();
    currentDirectory = 0;
}

void initializeDirectory(int sectorNumber, int back, int forward) {
    sector[sectorNumber].directory.back = back;
    sector[sectorNumber].directory.frwd = forward;
    sector[sectorNumber].directory.free = DIR_ENTRIES;

    for (int i = 0; i < DIR_ENTRIES; i++) {
        sector[sectorNumber].directory.dir[i].type = 'F';
        sector[sectorNumber].directory.dir[i].name[0] = '\0'; // null char end of c-string
        sector[sectorNumber].directory.dir[i].link = 0;
        sector[sectorNumber].directory.dir[i].size = 0;
    }
}
// linked list operation
void removeFreeSector(void) {

}
// linked list operation
void insert(void) {

}

void root(void) {
    currentDirectory = 0;
}

void dir(void) {

}

bool cd(const char *name) {
    int sectorNum = currentDirectory; 

    do { 
        for (int i = 0; i < DIR_ENTRIES; i++) {
            // strcmp returns 0 for equal strings
            if (strcmp(sector[sectorNum].directory.dir[i].name, name) == 0 &&
                sector[sectorNum].directory.dir[i].type == 'D') {
                currentDirectory = sector[sectorNum].directory.dir[i].link;
                return true;
            }
        }
        // advances sectorNum to next sector
        sectorNum = sector[sectorNum].directory.frwd;
    } while (sectorNum != 0); // "0" means no next directory sector
    return false;
}

void create(void) {

}

void open(void) {

}

void close(void) {

}

void seek(void) {

}

void write(void) {

}

void read(void) {

}

void delete(void) {

}

void exitFileSystem(void) {
    releaseFreeList();
}

#ifndef TESTING
int main(void)
{
    if(!startupSizeTest()) {
        return 1;
    }
    initialize_disk();

    exitFileSystem();

    return 0;
}
#endif
