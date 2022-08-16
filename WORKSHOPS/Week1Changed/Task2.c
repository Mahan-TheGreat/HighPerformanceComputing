#include <stdio.h>
#include <stdint.h>
void main(int argc, char *argv[])
{
int age;
char name[20];
printf("Enter your Name:\n");
fgets(name,20,stdin);
printf("Enter your Age:\n");
scanf("%d",&age);
printf("Hello %s, you are %d years old.\n", name, age);
}
