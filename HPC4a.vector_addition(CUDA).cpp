#include <stdio.h>

__global__ void add(int *A,int *B,int *C,int n){
    int i=blockIdx.x*blockDim.x+threadIdx.x;
    if(i<n) C[i]=A[i]+B[i];
}

int main(){
    int n=1024,A[n],B[n],C[n];

    for(int i=0;i<n;i++) A[i]=i, B[i]=i*2;

    int *dA,*dB,*dC;
    cudaMalloc((void**)&dA,n*sizeof(int));
    cudaMalloc((void**)&dB,n*sizeof(int));
    cudaMalloc((void**)&dC,n*sizeof(int));

    cudaMemcpy(dA,A,n*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(dB,B,n*sizeof(int),cudaMemcpyHostToDevice);

    int bs=256,gs=(n+bs-1)/bs;
    add<<<gs,bs>>>(dA,dB,dC,n);

    cudaMemcpy(C,dC,n*sizeof(int),cudaMemcpyDeviceToHost);

    printf("First 10 Results:\n");
    for(int i=0;i<10;i++)
        printf("%d + %d = %d\n",A[i],B[i],C[i]);

    cudaFree(dA); cudaFree(dB); cudaFree(dC);
}
