#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct threadArgs {
int start;
int finish;
};

void *threadMain(void *p){
struct threadArgs *pargs = p;
int i, c;
int nstart=pargs->start, nfinish=pargs->finish;
pthread_t tid = pthread_self();

for(i=nstart; i<=nfinish; i++){
	for(c=2; c<=i-1; c++) {
	if ( i%c==0 )
	break;
	}
	if ( c==i )
	printf("Thread id: %ld : Prime number: %d\n",tid, i);
	}
return 0;
}

void main(int argc, char **argv){
int numThreads = 3;
int i;
pthread_t thrID[numThreads];
struct threadArgs targs[100];

if (numThreads > 0 && numThreads <= 100){
int chunksize = 10000/numThreads ;
for (i=0; i < numThreads; i++){
targs[i].start = i * chunksize;
targs[i].finish = (i * chunksize) + chunksize;
pthread_create(&thrID[i], NULL, threadMain, &targs[i]);
}
for (i=0; i < numThreads; i++){
pthread_join(thrID[i], NULL);
}
}
}
