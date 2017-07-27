//
// Created by nebula on 7/27/17.
//

#include <stdlib.h>
#include "benchmark.h"
#include <arm_neon.h>

extern "C" {


void daxpy_core_neon(int array_size, FLOAT *a, FLOAT *b, FLOAT *c)
{
    //const FLOAT scalar = 3.141592;
    FLOAT scalar[2] = {3.141592, 3.141592};
    float64x2_t scalarx2;

    int i;
    const int interval = 2;

    scalarx2 = vld1q_f64(scalar);
//#pragma omp parallel for
    for (i=0; i<array_size; i+=interval){
        float64x2_t ax2;
        float64x2_t bx2 = vld1q_f64(&b[i]);
        float64x2_t cx2 = vld1q_f64(&c[i]);
        ax2 = vmlaq_f64(bx2, scalarx2, cx2);
        vst1q_f64(&a[i], ax2);
        //a[i] = b[i] + scalar * c[i];
    }
}

void daxpy_core_neon4(int array_size, FLOAT *a, FLOAT *b, FLOAT *c)
{

    int i;
    const int interval = 8;
    FLOAT scalar[2] = {3.141592, 3.141592};

    float64x2_t ax2[4];
    float64x2_t bx2[4];
    float64x2_t cx2[4];
    float64x2_t scalarx2;

    scalarx2 = vld1q_f64(scalar);
//#pragma omp parallel for
    for (i=0; i<array_size; i+=interval){
        bx2[0] = vld1q_f64(&b[i]);
        cx2[0] = vld1q_f64(&c[i]);
        ax2[0] = vmlaq_f64(bx2[0], scalarx2, cx2[0]);
        vst1q_f64(&a[i], ax2[0]);

        bx2[1] = vld1q_f64(&b[i+2]);
        cx2[1] = vld1q_f64(&c[i+2]);
        ax2[1] = vmlaq_f64(bx2[1], scalarx2, cx2[1]);
        vst1q_f64(&a[i+2], ax2[1]);

        bx2[2] = vld1q_f64(&b[i+4]);
        cx2[2] = vld1q_f64(&c[i+4]);
        ax2[2] = vmlaq_f64(bx2[2], scalarx2, cx2[2]);
        vst1q_f64(&a[i+4], ax2[2]);

        bx2[3] = vld1q_f64(&b[i+6]);
        cx2[3] = vld1q_f64(&c[i+6]);
        ax2[3] = vmlaq_f64(bx2[3], scalarx2, cx2[3]);
        vst1q_f64(&a[i+6], ax2[3]);
    }
}


double benchmark_daxpy_neon(int max_step, int size) {
    double start_time, stop_time;
    size_t align = alignof(float64x2_t);
    FLOAT *a;
    FLOAT *b;
    FLOAT *c;
    posix_memalign((void **)&a, align, size * sizeof(FLOAT));
    posix_memalign((void **)&b, align, size * sizeof(FLOAT));
    posix_memalign((void **)&c, align, size * sizeof(FLOAT));
    init_rand();
    init_array_rand(size, b, 1.0);
    init_array_rand(size, c, 1.0);

    //D("NEON MODE");

    start_time = getTime();
    for (int i=0; i<max_step; i++){
        daxpy_core_neon4(size, a, b, c);
    }
    stop_time = getTime();

    char msg_buf[1024];
    sprintf(msg_buf, "%f = %f + 3.14 * %f\n%f = %f + 3.14 * %f\n%f = %f + 3.14 * %f\n%f = %f + 3.14 * %f\n",
            a[0], b[0], c[0],
            a[1], b[1], c[1],
            a[2], b[2], c[2],
            a[3], b[3], c[3]
    );
    __android_log_write(ANDROID_LOG_INFO,"benchmark", msg_buf);

    free(a);
    free(b);
    free(c);

    return(stop_time - start_time);
}

}
