#include<iostream>
#include<math.h>
#include<string.h>
#include <omp.h>

#define N 1600
using namespace std;

int main(){

        int serial_scan[N], naive_scan[N], simd_scan[N], scan_a, new_a[N], map1[N];

        int a[N] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160};
        int predicate[N] = {0,1,1,0,0,1,1,1,0,1,1,0,0,1,1,1};


        for(int i = 0; i < N; i++){
                map1[i] = 0;
                serial_scan[i] = 0;
                simd_scan[i] = 0;
                new_a[i] = 0;
        }

        double start = omp_get_wtime();
        //Serial Scan with "+" operator
        scan_a = 0;
        for(int i = 0; i < N; i++){
                serial_scan[i] = scan_a;
                scan_a += predicate[i];
        }

        //map
        for(int i = 0; i < N; i++){
                if(predicate[i]==1) {
                        new_a[serial_scan[i]] = a[i];
                }
        }

        std::cout<<"Serial Scan Output:\n";

        for(int i = 0; i < N; i++)
                std::cout<<new_a[i]<<"\t";
        std::cout<<"\n";

        double stop = omp_get_wtime();
        std::cout<<"Time taken in seconds is "<<stop-start<<"\n";

    
        
        return 0;
}
