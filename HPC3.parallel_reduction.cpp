#include <iostream>
#include <omp.h>
using namespace std;

int minval(int a[],int n){
    int m=a[0];
    #pragma omp parallel for reduction(min:m)
    for(int i=0;i<n;i++)
        if(a[i]<m) m=a[i];
    return m;
}

int maxval(int a[],int n){
    int m=a[0];
    #pragma omp parallel for reduction(max:m)
    for(int i=0;i<n;i++)
        if(a[i]>m) m=a[i];
    return m;
}

int sum(int a[],int n){
    int s=0;
    #pragma omp parallel for reduction(+:s)
    for(int i=0;i<n;i++)
        s+=a[i];
    return s;
}

double avg(int a[],int n){
    return (double)sum(a,n)/n;
}

int main(){
    int a[]={1,2,3,4,5},n=5;
    cout<<"The minimum value is: "<<minval(a,n)<<'\n';
    cout<<"The maximum value is: "<<maxval(a,n)<<'\n';
    cout<<"The summation is: "<<sum(a,n)<<'\n';
    cout<<"The average is: "<<avg(a,n)<<'\n';
}
