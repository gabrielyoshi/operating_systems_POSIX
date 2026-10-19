#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
if (argc == 4)
{
int firstNumber = atoi(argv[1]);
int secondNumber = atoi(argv[2]);
int thirdNumber = atoi(argv[3]);
int sum = firstNumber + secondNumber + thirdNumber;
printf("The sum is %d\n", sum);
}
else if (argc == 3)
{
int firstNumber = atoi(argv[1]);
int secondNumber = atoi(argv[2]);
int sum = firstNumber + secondNumber;
printf("The sum is %d\n", sum);
}
else
{
printf("Two or three integers must be input. Try again.\n");
}
return 0;
}

