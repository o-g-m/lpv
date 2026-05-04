#include <stdio.h>
#include <omp.h>

// Bubble Sort (Odd-Even)
void bubble(int a[], int n) {
    for (int p = 0; p < n; p++)
        #pragma omp parallel for
        for (int i = p % 2; i < n - 1; i += 2)
            if (a[i] > a[i+1]) {
                int t = a[i];
                a[i] = a[i+1];
                a[i+1] = t;
            }
}

// Merge function
void merge(int a[], int l, int m, int r) {
    int i=l, j=m+1, k=0, t[100];
    while(i<=m && j<=r)
        t[k++] = (a[i]<a[j]) ? a[i++] : a[j++];
    while(i<=m) t[k++] = a[i++];
    while(j<=r) t[k++] = a[j++];
    for(i=l, k=0; i<=r; i++, k++) a[i]=t[k];
}

// Merge Sort
void mergesort(int a[], int l, int r) {
    if(l<r) {
        int m=(l+r)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergesort(a,l,m);
            #pragma omp section
            mergesort(a,m+1,r);
        }
        merge(a,l,m,r);
    }
}

// Main
int main() {
    int a[5]={5,2,9,1,3}, b[5]={4,8,9,1,3};

    bubble(a,5);
    mergesort(b,0,4);

    printf("Bubble: ");
    for(int i=0;i<5;i++) printf("%d ",a[i]);

    printf("\nMerge: ");
    for(int i=0;i<5;i++) printf("%d ",b[i]);

    return 0;
}
