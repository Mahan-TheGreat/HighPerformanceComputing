#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 

void *thread(void *p){ 
 for(int i=0; i<10; i++){ 
 printf("Thread ID %ld: i=%d\n", pthread_self(), i); 
 usleep(1000);
 } 
} 

void main(int argc, char *argv[]){ 
    
 int n  = atoi(*argv[1]);
 pthread_t tread[n]; 

  for(int i=0; i<n; i++){ 
 pthread_create(&thrId[i], NULL, thread, NULL); 
 } 

  for(int i=0; i<n; i++){ 
 pthread_join(thrId[i], NULL); 
  } 
return;
} 
