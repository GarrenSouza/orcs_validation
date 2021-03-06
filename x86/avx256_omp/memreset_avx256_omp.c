#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

int main(int argc, char const *argv[]) {
    int v_size;
    int size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(int);
    else return 0;

    int* data_a = (int*) aligned_alloc (32, v_size*sizeof (int));
    
    __m256i vec_a;
    int i;//, nthreads, tid;

    //#pragma omp parallel shared (vec_a, data_a, nthreads) private (tid, i)
    #pragma omp parallel shared (data_a) private (i, vec_a)
    {
        /*tid = omp_get_thread_num();
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf ("%d threads rodando\n", nthreads);
        }

        printf ("Thread %d começando...\n", tid); */
        #pragma omp for schedule (dynamic)
        for (int i = 0; i < v_size; i += 8) {
            vec_a = _mm256_load_si256 ((__m256i *) &data_a[i]);
            vec_a = _mm256_setzero_si256();
            _mm256_store_si256 ((__m256i *) &data_a[i], vec_a);
        }
    }

    printf ("%d\n", data_a[v_size-1]);

    return 0;
}
