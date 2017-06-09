#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#include "../matrix/definitions.h"

__global__ void multi(int *D_array, int *Rp_array, int *C_array, int *sonuc)//Grafik kart üzerinde koşacak kod
{
	int cvalue = 0;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int row = blockIdx.y * blockDim.y + threadIdx.y;

	Rp_array[counter+1],D_array[j]sonuc[0][k-1]V[0][counter]
	if (row > DIM || col > DIM) return;

	for (int k = 0; k < col; k++) {
	  int satir = 0,counter=0;
	 for (int  j = 0; j < row; j++) {
	   if (C_array[j]==k) {
	    while(satir < j){
	      satir = satir + (Rp_array[counter+1]-Rp_array[counter]);
	      counter++;
	    }
	    sonuc[0][k-1] += D_array[j] * V[0][counter];
	    }
	   }
	  }

	C[row*DIM + col] = cvalue;
}

void matrixmulti(int *D_array, int *Rp_array, int *C_array, int *sonuc){
	int *dev_a, *dev_b, *dev_c, *sonuc;

	cudaEvent_t start, stop;
	cudaEventCreate(&start);

	//allocate memory on global memory of gpu

	FILE *fp_C,*fp_D,*fp_Rp;
		 fp_C = fopen("C.txt", "r");
		 double C_array[99911];
		 for (int i = 0; i < 99911 ; i++) {
				 fscanf(fp_C, "%lf", &C_array[i]);
		 }
		 fclose(fp_C);

	cudaError_t err = cudaMalloc((void**)&fp_C, ((DIM)*(DIM))*sizeof(int));
	err = cudaMemcpy(fp_C, C_array, ((DIM*DIM))*sizeof(int), cudaMemcpyHostToDevice);

	fp_D = fopen("D.txt","r");
	double D_array[99911];
	for (int j = 0; j < 99911; j++) {
		fscanf(fp_D,"%lf",&D_array[j]);
	}

	fclose(fp_D);
	cudaError_t err = cudaMalloc((void**)&fp_D, ((DIM)*(DIM))*sizeof(int));
	err = cudaMemcpy(fp_D, D_array, ((DIM*DIM))*sizeof(int), cudaMemcpyHostToDevice);


	int Rp_array[10001];
	 fp_Rp = fopen("Rp.txt", "r");
	for (int i = 0; i < 10001 ; i++) {
			fscanf(fp_Rp, "%d", &Rp_array[i]);
	}

	cudaError_t err = cudaMalloc((void**)&fp_Rp, ((DIM)*(DIM))*sizeof(int));
	err = cudaMemcpy(fp_Rp, Rp_array, ((DIM*DIM))*sizeof(int), cudaMemcpyHostToDevice);

	fclose(fp_Rp);
	//veri kopyala gpu'da  koş veriyi geri al
	// Saat bulma
	//two dimension threads
	dim3 dimBlock(BlockSize, BlockSize);
	dim3 dimGrid((DIM + dimBlock.x - 1) / dimBlock.x, (DIM + dimBlock.y - 1) / dimBlock.y);

	cudaEventRecord(start);
	multi << < dimGrid, dimBlock >> >(dev_a, dev_b, dev_c);
	cudaEventRecord(stop);

	err = cudaMemcpy(sonuc, sonuc, ((DIM*DIM))*sizeof(int), cudaMemcpyDeviceToHost);
	printf("Cuda memcpy to HOST C:%s \n", cudaGetErrorString(err));

	//call the kernel function multi
	cudaEventRecord(start);
	multi << < dimGrid, dimBlock >> >(dev_a, dev_b, dev_c);
	cudaEventRecord(stop);

	for (int i = 0; i < 10000; i++) {
	  /* code */
	  fprintf(f,"%.3f ",sonuc[0][i]);
	}
	 fclose(f);

	//free the memory

	cudaFree(D_array);
	cudaFree(Rp_array);
	cudaFree(C_array);
	cudaFree(sonuc);

	cudaEventCreate(&stop);
}
