#include <stdio.h>
#include <stdlib.h>

struct Student

{

int id;

int age;

};

int main()

{

struct Student *sptr = (struct Student *  )malloc(sizeof(struct Student));

sptr->id = 5;

sptr->age = 35;

printf("id = %d and age = %d\n", sptr->id, sptr->age);

}
