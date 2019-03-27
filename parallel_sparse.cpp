#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>

#define N 3

int main() {

    int  mat[N][N], predicate[N], value[N*N], column[N*N], rowptr[N], scan_a, vec[N], y[N];

     // initialize
     mat[0][0] = 0;
     mat[0][1] = 0;
     mat[0][2] = 45;
     mat[1][0] = 0;
     mat[1][1] = 24;
     mat[1][2] = 0;
     mat[2][0] = 5;
     mat[2][1] = 7;
     mat[2][2] = 0;

     vec[0] =2;
     vec[1] =5;
     vec[2] =10;


    for(int i = 0; i < N; i++){
        rowptr[i] = i;
    }
    int key=0;
     int a[N] = {2,2,2};

    double start = omp_get_wtime();

    scan_a = 0;
    #pragma omp parallel for 
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(mat[i][j]!=0){
                value[(i*N)+j] = mat[i][j];
                column[(i*N)+j] = j;
                scan_a++;           
            } else {
              value[(i*N)+j] = -1;
              column[(i*N)+j] = -1;
            } 
        }
    }

    int value_r[scan_a], column_r[scan_a], ne[scan_a];

    int n = 0;
    for (int i = 0; i < N*N; i++){
         if(value[i]>=0){
            value_r[n] = value[i];
            column_r[n] = column[i];
            n++;
         }
    }

    #pragma omp parallel for
    for (int i=0; i<N; ++i) {
        y[i] = 0.0;
        for (int j=rowptr[i]; j<rowptr[i+1]; ++j)
            y[i] += value_r[j]*vec[column_r[j]];
    }

    double end = omp_get_wtime();

    std::cout<<" Values: "<<"\n";
    for(int i = 0; i < (sizeof(value_r)/sizeof(*value_r)); i++){
        std::cout << value_r[i] << " \n";
    }

    std::cout<<"Columns: "<<"\n";
    for(int i = 0; i < (sizeof(column_r)/sizeof(*column_r)); i++){
        std::cout << column_r[i] << " \n";
    }

    std::cout<<" Result is: "<<"\n";
    for(int i = 0; i < 3; i++){
        std::cout << y[i] << ", ";
    }

    std::cout<<"Time taken in seconds is "<<end-start<<"\n";

     return 0;
}