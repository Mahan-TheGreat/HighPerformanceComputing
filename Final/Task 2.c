#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<crypt.h>
#include<pthread.h>
#include<unistd.h>
#include <signal.h>

//Compile with: gcc -o Task2 Task2.c -lcrypt -pthread

struct value {
  char start;
  char stride;
  char *salt_and_encrypted;
  int number;
};

//defining threads
pthread_t *thread;

//Counter Variable
int count=0;

//declaring mutex
pthread_mutex_t mutex;


void *crack(void *p){
struct value *args_one = (struct value *)p;
  char salt[7]; //string used in password hashing
  char plain[5]; //Combination of letters being checked
  plain[4]='\0';

  int number=args_one->number;
  char * salt_and_encrypted_one=args_one->salt_and_encrypted;

  memcpy(salt,salt_and_encrypted_one,6);
  *(salt+6)='\0';

  //  printf("......Thread 1 is running from %c to %c.........\n",args_one->start,args_one->stride);
    for(int x_one=args_one->start; x_one<=args_one->stride;x_one++)
    {
        for(int y_one='A'; y_one<='Z'; y_one++)
        {
          for(int z_one=0; z_one<=99; z_one++)
          {
            sprintf(plain,"%c%c%02d",x_one,y_one,z_one);
            pthread_mutex_lock(&mutex);
            count++;
            printf(" plain %d with salt %s %s\n",number,plain,salt);
            char *check=(char *)crypt(plain,salt_one);
            printf(" plain %d check %s\n",number,check_one);
            pthread_mutex_unlock(&mutex);

            //if the passsword is found, code inside the if statement will run 
            if(strcmp(salt_and_encrypted_one, check) == 0)
            { 
              printf(" %s is your password with combination %d\n",plain,count);
              pthread_kill(thread[0],1);
              pthread_kill(thread[1],1);
            } 
            }
        }
    }
 
//  printf("%d solutions explored\n", count);
}



int main(int argc, char *argv[])
{

//setting argruments for two thread
  struct value v[2];
  pthread_t thread[2];
  v[0].salt_and_encrypted =argv[1];
  v[0].start='A';
  v[0].stride='M';
  v[0].number=0;

  v[1].salt_and_encrypted =argv[1];
  v[1].start='N';
  v[1].stride='Z';
  v[1].number=1;


  //creating two threads
  for(int x=0;x<2;x++)
  {
   pthread_create(&thread[x],NULL,crack,(void *)&v[x]);
  }
 
  //joining the threads 
  for(int x=0;x<2;x++)
  {
    pthread_join(thread[x],NULL);  
  }

}