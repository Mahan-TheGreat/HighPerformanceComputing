 #include <stdlib.h>
 #include <stdio.h>
 #include <malloc.h>

int inc(int *w) 
{
   *w = *w + 1; 
 }

int main() 
{
   int *x = malloc(sizeof(int));
   *x = 123;
   int *y;
   y = x;
   inc(y);
   printf("%d,%d\n", *x, *y);
   free(x);
   return EXIT_SUCCESS;
 }
 