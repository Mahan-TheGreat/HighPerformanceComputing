#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Compile this program: gcc -o Task1.c Task1 -pthread
FILE * file1;
int totalCount = 0;
pthread_mutex_t mutex;

int count()
{
  FILE *fp0;

  int rows, cols;
  int row, col;
  float matval = 0.0;
  char c;
  int maxDimension = 0;
  int matrixDimension = 0;

  fp0 = fopen("SampleMatricesWithErrors.txt", "r");

  while (!feof(fp0))
  {
    fscanf(fp0, "%d,%d\n", &rows, &cols);
    matrixDimension = rows * cols;
    if (matrixDimension > maxDimension)
    {
      maxDimension = matrixDimension;
    }
    for (row = 0; row < rows; row++)
    {
      for (col = 0; col < cols - 1; col++)
      {
        fscanf(fp0, "%f,", &matval);
      }
      fscanf(fp0, "%f\n", &matval);
    }
    totalCount++;
  }
  // printf("Maximum Dimesion: %d\n", maxDimension);
  fclose(fp0);
  return totalCount;
}

//declaring struct matrix
struct matrix
{
  int ROW;
  int COLUMN;
  double **m;
};

//declaring struct argument
struct Argument{
	int start;
	int end;
};

struct matrix *a;
struct matrix *result;

void display(struct matrix *a){
  for (int i = 0; i < totalCount; i++)
  {
    for (int b = 0; b < a[i].ROW; b++)
    {
      for (int t = 0; t < a[i].COLUMN; t++)
      {
        printf("%lf\t", a[i].m[b][t]);
      }
      printf("\n");
    }
    printf("\n");
    printf("\n");
  } 

}
// Multiply matrix A and matrix B and Store Result in matrix C
void multiply(struct matrix *A,struct matrix *B,struct matrix *C)
{

 /* printf("Matrix A");
  printf("\n");
   for(int i=0;i<A->ROW;i++){
   for(int j=0;j<A->COLUMN;j++){
      printf("%lf \t",A->m[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  printf("Matrix B");
  printf("\n");
  for(int i=0;i<B->ROW;i++){
   for(int j=0;j<B->COLUMN;j++){
      printf("%lf \t",B->m[i][j]);
    }
    printf("\n");
  } */

if(A->COLUMN!=B->ROW)
{
  printf("Matrix cannot be multiplied");
}
else 
{
//Allocating space for matrix C
C->ROW=A->ROW;
C->COLUMN=B->COLUMN;
C->m=(double **)malloc(C->ROW*sizeof(double*));
for(int i=0;i<C->ROW;i++)
{
  C->m[i]=(double *)malloc(C->COLUMN*sizeof(double));
}

// multiplying matrix A and B and Storing it in matrix C 
for(int i=0;i<A->ROW;i++){
  for(int j=0;j<B->COLUMN;j++)
  {
    double sum=0;
    for(int k=0;k<B->ROW;k++)
    {
        sum=sum+(A->m[i][k])*(B->m[k][j]);
    }
    C->m[i][j]=sum;
  }
}

// printing matrix C 
/*printf("\n");
printf("Matrix A*B:");
printf("\n");
for(int i=0;i<C->ROW;i++){
   for(int j=0;j<C->COLUMN;j++){
      printf("%lf \t",C->m[i][j]);
    }
    printf("\n");
  }
  printf("\n"); */

}
  
}

//Declaring threadFunction
void *threadf(void *p)
{
  struct Argument *value=(struct Argument *) p;
  int start=value->start;
  int end=value->end;

  for(int k=start;k<=end;k++)
  {
    multiply(&a[k*2],&a[k*2+1],result+k);
    fprintf(file1, "Row after multiplication=%d, Column after multiplication=%d\n", result[k].ROW, result[k].COLUMN);
          
   for(int i=0;i<result[k].ROW;i++)
   {
    for(int j=0;j<result[k].COLUMN;j++)
    {
      fprintf(file1,"%lf \t",result[k].m[i][j]);
    }
    fprintf(file1,"\n");
   }
     // pthread_exit(0);
  } 

}

void main(int argc,char *argv[])
{ 
  int thread_n = atoi(argv[1]);
  pthread_t thread[thread_n];

  //getting total count 
  int totalCount = count();
  printf("Total Matrix Count: %d\n", totalCount);
  
  //opening file in read mode
  FILE *ptr = fopen("SampleMatricesWithErrors.txt", "r");
  
  //initializing a for total number of matrix
  a=(struct matrix*) malloc(totalCount* sizeof(struct matrix));

  //initializing result for the resulting matrices
  result=(struct matrix*) malloc((totalCount/2)* sizeof(struct matrix));

  //counter
  int j = 0;
  
  //reading from file and storing the matrices
  while (!feof(ptr))
  {
    fscanf(ptr, "%d,%d", &a[j].ROW, &a[j].COLUMN);
    a[j].m = (double **)malloc(a[j].ROW * sizeof(double *));
    for (int k = 0; k < a[j].ROW; k++)
    {
      a[j].m[k] = (double *)malloc(a[j].COLUMN * sizeof(double));
    }

    for (int l = 0; l < a[j].ROW; l++)
    {
      for (int n = 0; n < a[j].COLUMN; n++)
      {
        fscanf(ptr, "%lf,", &a[j].m[l][n]);
      }
    }
    j = j + 1;
  }
  fclose(ptr);

  //Equal load distribution among threads
  int rem=(totalCount/2) % thread_n;
  struct Argument value[thread_n];
  int slicelist[thread_n];
  printf("Remender is %d\n",rem);
  for(int i=0;i<thread_n;i++)
  {
     slicelist[i]=(totalCount/2)/thread_n;
  }
  
  for(int i=0;i<rem;i++)
  {
			slicelist[i]=slicelist[i]+1;
	}
  
  //assigning start and end for number of threads
  for(int i=0;i<thread_n;i++)
  {
    if(i==0)
    {
     value[i].start=0;
    }
    else
    {
      value[i].start=value[i-1].end+1;
    }
    value[i].end=value[i].start+slicelist[i]-1;
  }
  
  //printing start and end value for each threads
  for(int i=0;i<thread_n;i++)
  {
    printf("Thread %d : Start = %d, End = %d\n",i,value[i].start,value[i].end);
  }

	//Creating new file in write mode
	   file1 = fopen("matrixresults.txt", "w");

	//Creating the threads
		for(int i=0;i<thread_n;i++)
    {	
			pthread_create(&thread[i], NULL, threadf, (void *)&value[i]);
		}
      
  //Joining the threads with main threads 
		for(int i=0;i<thread_n;i++)
    {		
			pthread_join(thread[i], NULL);
		}
			
  //Closing the file
		fclose(file1); 
   
}
