#include <unistd.h>

#include<fcntl.h>

#include <stdio.h>

#include <errno.h>

int main()

{

int fd;

char buffer[80];

char msg[50] = "testing";

fd = open ("book.txt",O_RDWR);

printf("fd = %d\n", fd);

if (fd != -1)

{

printf(" the file is now open\n ");

lseek(fd,0,SEEK_END);

write(fd, msg, sizeof(msg));

printf("size of msg is %lu\n", sizeof(msg));

lseek(fd,0,SEEK_SET);

read(fd, buffer, sizeof(msg));

printf("%s\n",buffer);

close(fd);

printf("%s", "example\n");

}

else

  printf("errno = %d\n", errno);

}
