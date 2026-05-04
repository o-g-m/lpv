#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;

#define SIZE 10000

void bubbleSortSeq(vector<int>& a){
    for(int i=0;i<a.size()-1;i++)
        for(int j=0;j<a.size()-i-1;j++)
            if(a[j]>a[j+1]) swap(a[j],a[j+1]);
}

void bubbleSortParallel(vector<int>& a){
    int n=a.size();
    for(int i=0;i<n;i++){
        #pragma omp parallel for
        for(int j=0;j<n-1;j+=2)
            if(a[j]>a[j+1]) swap(a[j],a[j+1]);

        #pragma omp parallel for
        for(int j=1;j<n-1;j+=2)
            if(a[j]>a[j+1]) swap(a[j],a[j+1]);
    }
}

void merge(vector<int>& a,int l,int m,int r){
    vector<int> L(a.begin()+l,a.begin()+m+1),R(a.begin()+m+1,a.begin()+r+1);
    int i=0,j=0,k=l;
    while(i<L.size()&&j<R.size())
        a[k++]=(L[i]<=R[j])?L[i++]:R[j++];
    while(i<L.size()) a[k++]=L[i++];
    while(j<R.size()) a[k++]=R[j++];
}

void mergeSortSeq(vector<int>& a,int l,int r){
    if(l<r){
        int m=(l+r)/2;
        mergeSortSeq(a,l,m);
        mergeSortSeq(a,m+1,r);
        merge(a,l,m,r);
    }
}

void mergeSortParallel(vector<int>& a,int l,int r,int d){
    if(l<r){
        int m=(l+r)/2;
        if(d<=0){
            mergeSortSeq(a,l,m);
            mergeSortSeq(a,m+1,r);
        } else{
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(a,l,m,d-1);
                #pragma omp section
                mergeSortParallel(a,m+1,r,d-1);
            }
        }
        merge(a,l,m,r);
    }
}

void gen(vector<int>& a){
    for(int& x:a) x=rand()%100000;
}

int main(){
    vector<int> a(SIZE),t;
    srand(time(0));
    gen(a);

    double s,e;

    t=a; s=omp_get_wtime();
    bubbleSortSeq(t);
    e=omp_get_wtime();
    cout<<"Sequential Bubble Sort Time: "<<e-s<<" sec\n";

    t=a; s=omp_get_wtime();
    bubbleSortParallel(t);
    e=omp_get_wtime();
    cout<<"Parallel Bubble Sort Time: "<<e-s<<" sec\n";

    t=a; s=omp_get_wtime();
    mergeSortSeq(t,0,SIZE-1);
    e=omp_get_wtime();
    cout<<"Sequential Merge Sort Time: "<<e-s<<" sec\n";

    t=a; s=omp_get_wtime();
    mergeSortParallel(t,0,SIZE-1,4);
    e=omp_get_wtime();
    cout<<"Parallel Merge Sort Time: "<<e-s<<" sec\n";
}
