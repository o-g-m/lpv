#include <stdio.h>
#include <limits.h>

#define B 256

__global__ void sumR(int *in,int *out,int n){
    __shared__ int s[B];
    int t=threadIdx.x,i=blockIdx.x*blockDim.x+t;
    s[t]=(i<n)?in[i]:0; __syncthreads();
    for(int x=blockDim.x/2;x>0;x>>=1){
        if(t<x) s[t]+=s[t+x];
        __syncthreads();
    }
    if(t==0) out[blockIdx.x]=s[0];
}

__global__ void minR(int *in,int *out,int n){
    __shared__ int s[B];
    int t=threadIdx.x,i=blockIdx.x*blockDim.x+t;
    s[t]=(i<n)?in[i]:INT_MAX; __syncthreads();
    for(int x=blockDim.x/2;x>0;x>>=1){
        if(t<x && s[t+x]<s[t]) s[t]=s[t+x];
        __syncthreads();
    }
    if(t==0) out[blockIdx.x]=s[0];
}

__global__ void maxR(int *in,int *out,int n){
    __shared__ int s[B];
    int t=threadIdx.x,i=blockIdx.x*blockDim.x+t;
    s[t]=(i<n)?in[i]:INT_MIN; __syncthreads();
    for(int x=blockDim.x/2;x>0;x>>=1){
        if(t<x && s[t+x]>s[t]) s[t]=s[t+x];
        __syncthreads();
    }
    if(t==0) out[blockIdx.x]=s[0];
}

int main(){
    int n=1024,h[n];
    for(int i=0;i<n;i++) h[i]=i+1;

    int *d_in,*d_out,gs=(n+B-1)/B;
    cudaMalloc(&d_in,n*sizeof(int));
    cudaMalloc(&d_out,gs*sizeof(int));
    cudaMemcpy(d_in,h,n*sizeof(int),cudaMemcpyHostToDevice);

    int out[gs];

    sumR<<<gs,B>>>(d_in,d_out,n);
    cudaMemcpy(out,d_out,gs*sizeof(int),cudaMemcpyDeviceToHost);
    int sum=0; for(int i=0;i<gs;i++) sum+=out[i];

    minR<<<gs,B>>>(d_in,d_out,n);
    cudaMemcpy(out,d_out,gs*sizeof(int),cudaMemcpyDeviceToHost);
    int mn=out[0]; for(int i=1;i<gs;i++) if(out[i]<mn) mn=out[i];

    maxR<<<gs,B>>>(d_in,d_out,n);
    cudaMemcpy(out,d_out,gs*sizeof(int),cudaMemcpyDeviceToHost);
    int mx=out[0]; for(int i=1;i<gs;i++) if(out[i]>mx) mx=out[i];

    float avg=(float)sum/n;

    printf("Min = %d\n",mn);
    printf("Max = %d\n",mx);
    printf("Sum = %d\n",sum);
    printf("Average = %.2f\n",avg);

    cudaFree(d_in);
    cudaFree(d_out);
}
