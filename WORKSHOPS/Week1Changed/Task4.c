#include<stdio.h>

void swap(int*x, int*y)
{
  int temp;

  temp = *x;
  *x = *y;
  *y = temp;
  
  }

int main()
{
  int a,b;
  
  printf("Enter Value of A and B\n");
  scanf("%d %d",&a, &b);
  
  printf("A: %d , B= %d. \n", a, b);
  
  printf("Swapping A and B\n");
  
  swap(&a, &b );
  
  printf("A = %d , B = %d.\n", a, b);
  
  
 }
  
