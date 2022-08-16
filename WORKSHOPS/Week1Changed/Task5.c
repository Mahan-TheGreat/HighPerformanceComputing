#include <stdio.h> 
#include <stdlib.h> 
void main(int argc, char *argv[]) 
{ 
  int n,i=0;
  int *ptr=0;
 
  
  printf("Enter a number between 1-50. The array to  create: \n"); 
  scanf("%d",&n);
  ptr = (int *)malloc(n*sizeof(int));
  
  if(n<1){
     printf("Number Must be greater than 0!\n");
     return;
  }
  else if(n>50)
  {
      printf("Number Must be less than 50!\n");
      return;
  }

 int numbers[n];
 
 for( i=0; i < n; i++){ 
 numbers[n] = rand();  

 } 
 
 printf("The elements of the array are:\n ");
    for(i = 0; i < n; i++)
    {
        printf("Array index %d : Value: %d \n",i, numbers[i]);   
    }
    
     
} 

