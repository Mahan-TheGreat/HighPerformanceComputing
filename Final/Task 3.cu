#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//Compile with: nvcc -o cudaCrack Task3.cu
 
//Password Cracking using CUDA 
__device__ char* encryptDecrypt(char* tempPassword){

	char * generatedPwd = (char *) malloc(sizeof(char) * 11);

	generatedPwd[0] = tempPassword[0] + 2;
	generatedPwd[1] = tempPassword[0] - 2;
	generatedPwd[2] = tempPassword[0] + 1;
	generatedPwd[3] = tempPassword[1] + 3;
	generatedPwd[4] = tempPassword[1] - 3;
	generatedPwd[5] = tempPassword[1] - 1;
	generatedPwd[6] = tempPassword[2] + 2;
	generatedPwd[7] = tempPassword[2] - 2;
	generatedPwd[8] = tempPassword[3] + 4;
	generatedPwd[9] = tempPassword[3] - 4;
	generatedPwd[10] = '\0';

	for(int i =0; i<10; i++){
		if(i >= 0 && i < 6){ //Checking all uppercase letters limits
			if(generatedPwd[i] > 122){
				generatedPwd[i] = (generatedPwd[i] - 122) + 97;
			}else if(generatedPwd[i] < 97){
				generatedPwd[i] = (97 - generatedPwd[i]) + 97;
			}
		}else{ //Checking numbers
			if(generatedPwd[i] > 57){
				generatedPwd[i] = (generatedPwd[i] - 57) + 48;
			}else if(generatedPwd[i] < 48){
				generatedPwd[i] = (48 - generatedPwd[i]) + 48;
			}
		}
	}
	return generatedPwd;
}

//GPU function- can be launced by many blocks and threads
__global__ void crack(char * alphabet, char * numbers){

char matchedPwd[4];

matchedPwd[0] = alphabet[blockIdx.x];
matchedPwd[1] = alphabet[blockIdx.y];

matchedPwd[2] = numbers[threadIdx.x];
matchedPwd[3] = numbers[threadIdx.y];

//Idx --> Index of the block or thread

char* encryptedPwd = "gcfvpr8491"; 
char* search = encryptDecrypt(matchedPwd);

//Matching encrypted password with generated password
int iter = 0;
int is_match = 0;
while (*encryptedPwd != '\0' || *search != '\0') {
	if (*encryptedPwd == *search) {
		encryptedPwd++;
		search++;
	} else if ((*encryptedPwd == '\0' && *search != '\0') || (*encryptedPwd != '\0' && *search == '\0') || *encryptedPwd != *search) {
		is_match = 1;
	
		break;
	}
}
if (is_match == 0) {
	//Print password if encrypted password matches generated password
	printf("Password found successfully: %c%c%c%c \n", matchedPwd[0],matchedPwd[1],matchedPwd[2],matchedPwd[3]);
}


}

int time_count(struct timespec *start, struct timespec *end,
                   long long int *diff)
{
    long long int in_sec = end->tv_sec - start->tv_sec;
    long long int in_nano = end->tv_nsec - start->tv_nsec;
    if (in_nano < 0)
    {
        in_sec--;
        in_nano += 1000000000;
    }
    *diff = in_sec * 1000000000 + in_nano;
    return !(*diff > 0);
}

int main(int argc, char ** argv){

    struct timespec start, end;
    long long int time_used;
     
	//possible characters and digits
	char cpuCharacter[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	char cpuDigits[26] = {'0','1','2','3','4','5','6','7','8','9'};

	char * gpuCharacter;
	cudaMalloc( (void**) &gpuCharacter, sizeof(char) * 26); 
	cudaMemcpy(gpuCharacter, cpuCharacter, sizeof(char) * 26, cudaMemcpyHostToDevice);

	char * gpuDigits;
	cudaMalloc( (void**) &gpuDigits, sizeof(char) * 26); 
	cudaMemcpy(gpuDigits, cpuDigits, sizeof(char) * 26, cudaMemcpyHostToDevice);
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	crack<<< dim3(26,26,1), dim3(10,10,1) >>>( gpuCharacter, gpuDigits );
    cudaThreadSynchronize();

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_count(&start, &end, &time_used);

    printf("Time taken: %f seconds OR %lld Nano Seconds\n", (time_used / 1.0e9), time_used);
  
    
	return 0;
}












