#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TESTING
#include "simulated_disk.c"

void test_sizes(void)
{
    assert(sizeof(sector[0].directory.dir[0]) == 20);
    assert(sizeof(union block) == 512);
    assert(sizeof(sector) == 51200);
}

void test_initialization(void)
{
    initialize_disk();

    assert(sector[0].directory.back == 0);
    assert(sector[0].directory.frwd == 0);
    assert(sector[0].directory.free == DIR_ENTRIES);
    assert(currentDirectory == 0);

    for (int i = 0; i < DIR_ENTRIES; i++) {
        assert(sector[0].directory.dir[i].type == 'F');
        assert(sector[0].directory.dir[i].link == 0);
        assert(sector[0].directory.dir[i].size == 0);
    }
}

void test_free_list(void)
{
    initialize_disk();

    int count = 0;
    for (nodeType *node = freeList; node != NULL; node = node->link) {
        assert(node->info >= 1);
        assert(node->info < NUM_SECTORS);
        count++;
    }

    assert(count == 99);
}

void test_root(void)
{
    initialize_disk();
    currentDirectory = 42;
    root();

    assert(currentDirectory == 0);
}

void test_cd(void)
{
    initialize_disk();

    strcpy(sector[0].directory.dir[0].name, "DOCS");
    sector[0].directory.dir[0].type = 'D';
    sector[0].directory.dir[0].link = 3;
    sector[0].directory.dir[0].size = 0;

    assert(cd("DOCS") == true);
    assert(currentDirectory == 3);

    currentDirectory = 0;
    assert(cd("missing") == false);
}

void test_dir(void){}
void test_create(void){}
void test_open(void){}
void test_close(void){}
void test_seek(void){}
void test_write(void){}
void test_read(void){}
void test_delete(void){}

int main(void)
{
    test_sizes();
    test_initialization();
    test_free_list();
    test_root();
    test_cd();
    test_dir();
    test_create();
    test_open();
    test_close();
    test_seek();
    test_write();
    test_read();
    test_delete();

    releaseFreeList();
    puts("All tests passed.");
    return 0;
}
