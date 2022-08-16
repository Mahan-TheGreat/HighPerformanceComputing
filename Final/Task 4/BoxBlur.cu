#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

//Compile with:  nvcc -o Blur BoxBlur.cu lodepng.cpp

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

//GPU Kernel function
__global__ void ImageFilter(unsigned char * cpu_imageOutput, unsigned char * cpu_imageInput,unsigned int width,unsigned int height){
    
	//for rgbt of a color
	int r = 0;
	int g = 0;
	int b = 0;
	int t = 0;
	int x,y;
	int count = 0;

	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	int pixel = idx*4;

	for(x = (pixel - 4); x <=  (pixel + 4); x+=4){
		//pixel check for found at x
		if ((x > 0) && x < (height * width * 4) && ((x-4)/(4*width) == pixel/(4*width))){
			for(y = (x - (4 * width)); y <=  (x + (4 * width)); y+=(4*width)){
				if(y > 0 && y < (height * width * 4)){
					r += cpu_imageInput[y];
					g += cpu_imageInput[1+y];
					b += cpu_imageInput[2+y]; 
					count++;
				}
			}
		}
	}
	
	t = cpu_imageInput[3+pixel];

	cpu_imageOutput[pixel] = r / count;
	cpu_imageOutput[1+pixel] = g / count;
	cpu_imageOutput[2+pixel] = b / count;
	cpu_imageOutput[3+pixel] = t;
}

int main(int argc, char **argv){

	unsigned int errors;
	unsigned int occErrors;
	unsigned char* images;
	unsigned int width;
	unsigned int height;
	struct timespec start, finish;   
  	long long int time_elapsed;
  	clock_gettime(CLOCK_MONOTONIC, &start);
	const char* filename = "Image.png";
	const char* newFileName = "blur Image.png";

	errors = lodepng_decode32_file(&images, &width, &height, filename);
	if(errors){
		printf("Error %u: %s\n", errors, lodepng_error_text(errors));
	}

	const int ARRAY_SIZE = width*height*4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

	unsigned char host_imageInput[ARRAY_SIZE * 4];
	unsigned char host_imageOutput[ARRAY_SIZE * 4];

	for (int i = 0; i < ARRAY_SIZE; i++) {
		host_imageInput[i] = images[i];
	}

	// declaring variable
	unsigned char * d_in;
	unsigned char * d_out;

	// allocating GPU memory
	cudaMalloc((void**) &d_in, ARRAY_BYTES);
	cudaMalloc((void**) &d_out, ARRAY_BYTES);

	cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

	// launch the kernel function
	ImageFilter<<<height, width>>>(d_out, d_in, width, height);

	//copy result back to cpu
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);

	
	printf("the blur Image has been created!\n");
	
	occErrors = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if(occErrors){
		printf("Error occurred %u: %s\n", errors, lodepng_error_text(occErrors));
	}

	//Free allocated memory 
	cudaFree(d_in);
	cudaFree(d_out);

	clock_gettime(CLOCK_MONOTONIC, &finish);
  	time_difference(&start, &finish, &time_elapsed);
  	printf("Time elapsed %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9)); 
	return 0;
}
