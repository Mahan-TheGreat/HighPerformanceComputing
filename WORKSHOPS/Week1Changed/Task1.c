#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[])
{
int age = atoi(argv[2]);
char *name = argv[1];
printf("Hello %s, you are %d years old.", name, age);
}
