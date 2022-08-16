#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


int flag=0;
int primeCount;
pthread_mutex_t lock;


struct threadArgs {
int start;
int finish;
};

void *threadMain(void *p)
{
   pthread_t tid = pthread_self();

 if(primeCount==5){
		pthread_cancel(tid);
  }

struct threadArgs *pargs = p;
int nstart=pargs->start, nfinish=pargs->finish;
int i,c;

for( i=nstart; i<=nfinish; i++){

		for( c=2; c<=i-1; c++) {

      

			if ( i%c==0 )
			      break;
			 	}
     
         pthread_mutex_lock(&lock);
			if ( c==i ){
          
					primeCount = primeCount + 1 ; 
			    printf("Primes Found: %d , Prime number: %d\n",primeCount,c);

	    }

	    	pthread_mutex_unlock(&lock); 

	    	if(primeCount==5){
				 flag =1;
				 pthread_cancel(tid);
		    }
		    
	  }

	  pthread_exit(NULL);
 	
	}



void main(){

pthread_mutex_init(&lock,NULL);
int i;
int numThreads;


printf("Enter Number of Threads: \n");
scanf("%d",&numThreads);

void *totalCount[numThreads]; 

int total[numThreads];


pthread_t thrID[numThreads];

struct threadArgs targs[100];

if (numThreads > 0 && numThreads <= 100){

int workload = 10000/numThreads ;

for (i=0; i < numThreads; i++){

targs[i].start = i * workload;

targs[i].finish = (i * workload) + workload;

pthread_create(&thrID[i], NULL, threadMain, &targs[i]);

}

for (i=0; i < numThreads; i++){

pthread_join(thrID[i], &totalCount[i]);

}
  printf("%d prime numbers found. \n",primeCount);
}
else{
	printf("Enter a value between 0-101");
}

pthread_mutex_destroy(&lock);


}

