 #include <stdlib.h>
 #include <stdio.h>

int inc(int *w) 
{
   *w = *w + 1; 
 }

int main() 
{
   int x = 123;
   int y = x;
   inc(&y);
   printf("%d,%d\n", x, y);
   return EXIT_SUCCESS;
 }
 