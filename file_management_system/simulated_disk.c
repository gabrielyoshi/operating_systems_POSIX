#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define BLOCK_SIZE 64 // 64 bytes in each block
#define NUM_BLOCKS 100 // a disk with an array of 100 blocks
#define MAX_FILES 16
#define MAX_NAME 32

typedef struct {
    bool in_use;
    char name[MAX_NAME];
    size_t size; // files size in bytes
    int start_block; // -1 for an empty file
    int block_count;
} File;

// simulated disk: 2-D array of bytes. this disk holds 6400 bytes
unsigned char disk[NUM_BLOCKS][BLOCK_SIZE] = {0}; // initializes every byte = 0
// tracks occupied blocks (i.e. free space management)
bool bitmap[NUM_BLOCKS] = {false}; // false = free; true = allocated
// file metadata table
File files[MAX_FILES] = {0}; // initializes every field to 0

void initialize_disk(void)
{
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].start_block = -1;
    }
}

void list_files(void) {
    bool found = false;

    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].in_use) {
            continue;
        }

        found = true;

        printf("File Name: %s \n", files[i].name);
        printf("Size: %zu bytes \n", files[i].size);
        printf("Blocks:");

        // prints contiguous blocks
        for (int j = 0; j < files[i].block_count; j++) {
            printf(" %d", files[i].start_block + j);
        }

        if (files[i].block_count == 0) {
            printf(" none");
        }

        printf("\n");
    }

    if (!found) {
        printf("No files. \n");
    }
}

int main(void)
{
    initialize_disk();

    printf("First entry's start block: %d\n", files[0].start_block);

    // sample entry
    files[0] = (File) {
        .in_use = true,
        .name = "notes.txt",
        .size = 150,
        .start_block = 0,
        .block_count = 3
    };
    list_files();

    return 0;
}