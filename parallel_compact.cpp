#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>

#define N 1600

template <typename T, typename F>
int map_scan(const T* in, T* out, int size, F filter)
{
     int* index = new int[size];
     #pragma omp parallel for
     for (int i = 0; i < size; i++)
         index[i] = filter(in[i]);

     // upsweep 
     for (int stride = 1; stride < size; stride <<= 1) {
         #pragma omp parallel for
         for (int i = 0; i < size; i += 2 * stride)
             index[2 * stride + i - 1] = index[2 * stride + i - 1] + index[stride + i - 1];
     }
     T last = index[size - 1];
     index[size - 1] = T(0);

     // downsweep
     for (int stride = size / 2; stride > 0; stride >>= 1) {
         #pragma omp parallel for
         for (int i = 0; i < size; i += 2 * stride) {
             T temp = index[stride + i - 1];
             index[stride + i - 1] = index[2 * stride + i - 1];
             index[2 * stride + i - 1] = temp + index[2 * stride + i - 1];
         }
     }

     #pragma omp parallel for
     for (int i = 0; i < size - 1; i++)
         if (index[i] < index[i + 1])
             out[index[i + 1] - 1] = in[i];
     if (index[size - 1] < last)
         out[last - 1] = in[size - 1];

     delete[] index;
     return last;
}

int main() {

    int a[N], predicate[N], serial_scan[N], scan_a, new_a[N], map1[N];

    int noutput, nn = N;
     int* input  = new int[N];
     int* output = new int[N];
     int  key    = 4;

     // initialize
     #pragma omp parallel for
     for (int i = 0; i < N; i++){
        a[i] = i*10;
        input[i] = a[i];
    }

     auto filter = [&](int a) { return (a % key) != 0; };

     double start = omp_get_wtime();
     noutput = map_scan(input, output, N, filter);
     double end = omp_get_wtime();

     if (noutput < N)
         nn = noutput - 1;
     for (int i = 0; i < nn; i++)
         std::cout << output[i] << ' ';
     std::cout << output[noutput - 1] << std::endl;

    std::cout<<"Time taken in seconds is "<<end-start<<"\n";

     return 0;
}