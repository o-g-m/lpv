#include <stdio.h>
#define N 4

__global__ void mul(int A[N][N],int B[N][N],int C[N][N]){
    int r=threadIdx.y,c=threadIdx.x,s=0;
    for(int k=0;k<N;k++) s+=A[r][k]*B[k][c];
    C[r][c]=s;
}

int main(){
    int A[N][N],B[N][N],C[N][N];

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            A[i][j]=i+j, B[i][j]=i*j;

    int (*dA)[N],(*dB)[N],(*dC)[N];

    cudaMalloc((void**)&dA,sizeof(int)*N*N);
    cudaMalloc((void**)&dB,sizeof(int)*N*N);
    cudaMalloc((void**)&dC,sizeof(int)*N*N);

    cudaMemcpy(dA,A,sizeof(int)*N*N,cudaMemcpyHostToDevice);
    cudaMemcpy(dB,B,sizeof(int)*N*N,cudaMemcpyHostToDevice);

    dim3 t(N,N);
    mul<<<1,t>>>(dA,dB,dC);

    cudaMemcpy(C,dC,sizeof(int)*N*N,cudaMemcpyDeviceToHost);

    printf("Result Matrix:\n");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++) printf("%d ",C[i][j]);
        printf("\n");
    }

    cudaFree(dA); cudaFree(dB); cudaFree(dC);
}
