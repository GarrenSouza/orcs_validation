#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);
    int v_size = 0;
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(float);
    else return 0;
    
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    int elem = sqrt (v_size);
    while (elem % 16 != 0) elem++;
    
    __m512 elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b;
    __m512 mul = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
    int i = 0;
    #pragma omp parallel shared (data_a, data_b, mul) private (i, elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b)
    {
        #pragma omp for schedule (dynamic)
        for (i = elem; i < v_size; i += 16) {
            elem_a1 = _mm512_load_ps (&data_a[i]);
            elem_a2 = _mm512_loadu_ps (&data_a[i-elem]);
            elem_a3 = _mm512_load_ps (&data_a[i+elem]);
            elem_a4 = _mm512_loadu_ps (&data_a[i+1]);
            elem_a5 = _mm512_loadu_ps (&data_a[i-1]);
            elem_b = _mm512_add_ps(elem_a1, elem_a2);
            elem_b = _mm512_add_ps(elem_b, elem_a3);
            elem_b = _mm512_add_ps(elem_b, elem_a4);
            elem_b = _mm512_add_ps(elem_b, elem_a5);
            elem_b = _mm512_mul_ps(elem_b, mul);
            _mm512_stream_ps (&data_b[i], elem_b);        
        }
    }

    printf ("%f\n", data_b[v_size-1]);
    
    return 0;
}