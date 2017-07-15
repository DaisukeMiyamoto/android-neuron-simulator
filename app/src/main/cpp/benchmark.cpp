//
// Created by nebula on 7/15/17.
//

#include <stdlib.h>
#include <time.h>
#include "benchmark.h"


extern "C" {

#ifdef USE_FLOAT
#define EXP(x) expf((x))
typedef float FLOAT;
#else
#define EXP(x) exp((x))
typedef double FLOAT;
#endif

double getTime();
void init_array(int size, FLOAT *array, FLOAT value);
void init_array_rand(int size, FLOAT *array, FLOAT max);

double getTime()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return(ts.tv_sec + ts.tv_nsec*1e-9);
}


void init_array(int size, FLOAT *array, FLOAT value) {
    for (int i = 0; i < size; i++) { array[i] = value; }
}

void init_array_rand(int size, FLOAT *array, FLOAT max) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < size; i++) { array[i] = ((double)rand() / ((double)RAND_MAX + 1)) * max; }
}

void triad_core(int array_size, FLOAT *a, FLOAT *b, FLOAT *c)
{
    FLOAT scalar = 3.141592;
    for (int i; i<array_size; i++){
        a[i] = b[i] + scalar * c[i];
    }
}

double benchmark_triad(int size) {
    FLOAT *a;
    FLOAT *b;
    FLOAT *c;
    double start_time, stop_time;

    a = (FLOAT *)malloc(size * sizeof(FLOAT));
    b = (FLOAT *)malloc(size * sizeof(FLOAT));
    c = (FLOAT *)malloc(size * sizeof(FLOAT));
    init_array_rand(size, b, 1.0);
    init_array_rand(size, c, 1.0);

    start_time = getTime();
    triad_core(size, a, b, c);
    stop_time = getTime();

    free(a);
    free(b);
    free(c);
    return(stop_time - start_time);
}

/*

// 4 OP + exp = 34 OP
FLOAT v_trap (FLOAT x, FLOAT y) { return( (fabs(x/y) > 1e-6)? ( x/(EXP(x/y) - 1.0) ) : ( y*(1. - x/y/2.) ) ); }

// 2 OP + v_trap = 6 OP + exp = 36 OP
FLOAT calc_alpha_n (FLOAT v) { return( 0.01  * v_trap(-(v+55.), 10.) );   }

// 3 OP + exp = 33 OP
FLOAT calc_beta_n  (FLOAT v) { return( 0.125 * EXP( -(v+65.) / 80.) );    }

// 2 OP + v_trap = 4 + exp = 34 OP
FLOAT calc_alpha_m (FLOAT v) { return( 0.1   * v_trap(-(v+40.), 10.));    }

// 3 OP + exp = 33 OP
FLOAT calc_beta_m  (FLOAT v) { return( 4.    * EXP( -(v+65) / 18.) );     }

// 3 OP + exp = 33 OP
FLOAT calc_alpha_h (FLOAT v) { return( 0.07  * EXP( -(v+65) / 20.) );     }

// 4 OP + exp = 34 OP
FLOAT calc_beta_h  (FLOAT v) { return( 1. / (EXP( -(v+35) / 10.) + 1.) ); }

// 5 OP + alpha_m + beta_m = 12 OP + 2 exp = 72 OP
FLOAT calc_dm_core(FLOAT v, FLOAT m, FLOAT dt) { return( (calc_alpha_m(v) * (1.0 - m) - calc_beta_m(v) * (m)) * dt ); }

// 5 OP + alpha_n + beta_n = 14 OP + 2 exp = 74 OP
FLOAT calc_dn_core(FLOAT v, FLOAT n, FLOAT dt) { return( (calc_alpha_n(v) * (1.0 - n) - calc_beta_n(v) * (n)) * dt ); }

// 5 OP + alpha_h + beta_h = 12 OP + 2 exp = 72 OP
FLOAT calc_dh_core(FLOAT v, FLOAT h, FLOAT dt) { return( (calc_alpha_h(v) * (1.0 - h) - calc_beta_h(v) * (h)) * dt ); }

//
FLOAT calc_dv_core(FLOAT v, FLOAT m, FLOAT n, FLOAT h, FLOAT *const_table)
{
    FLOAT dv;
    dv = const_table[HH_CONST_DT] * const_table[HH_CONST_CM_INV]
         * (const_table[HH_CONST_GK] * n * n * n * n * (const_table[HH_CONST_E_K] - v)
            + const_table[HH_CONST_GNA] * m * m * m * h * (const_table[HH_CONST_E_NA] - v)
            + const_table[HH_CONST_GM] * (const_table[HH_CONST_REST] - v)
            + const_table[HH_CONST_I_INJ]);
    return(dv);
}

double calc_hh_c_core(double *hh_v, double *hh_m, double *hh_n, double *hh_h, const double *const_table)
{
    // 1 step: 74 + 72 + 74 + 19 + 4 = 243
    double dm, dn, dh, m, h, n, v;
    m = *hh_m;
    h = *hh_h;
    n = *hh_n;
    v = *hh_v;

    // 74 OP
    m += calc_dm_core(v, m, const_table[HH_CONST_DT]);
    // 72 OP
    n += calc_dn_core(v, m, const_table[HH_CONST_DT]);
    // 74 OP
    h += calc_dh_core(v, m, const_table[HH_CONST_DT]);
    // 19 OP
    v += calc_dv_core(v, m, n, h, const_table);

    *hh_m = m;
    *hh_n = n;
    *hh_h = h;
    return(v);
}

void calc_hh_c(int max_step, int n_cell, const double *const_table, double *data_array)
{
    int step, i;
    double *m_array, *n_array, *h_array;
    data_array += n_cell;

    m_array = (double *)malloc(n_cell * sizeof(double));
    n_array = (double *)malloc(n_cell * sizeof(double));
    h_array = (double *)malloc(n_cell * sizeof(double));
    init_double(n_cell, m_array, calc_alpha_m(-65.0) / (calc_alpha_m(-65.0) + calc_beta_m(-65.0)));
    init_double(n_cell, n_array, calc_alpha_n(-65.0) / (calc_alpha_n(-65.0) + calc_beta_n(-65.0)));
    init_double(n_cell, h_array, calc_alpha_h(-65.0) / (calc_alpha_h(-65.0) + calc_beta_h(-65.0)));

    for (step = 0; step < (max_step-1); step++)
    {
        for (i = 0; i < n_cell; i++)
        {
            *(data_array) = calc_hh_c_core(data_array - n_cell, &(m_array[i]), &(n_array[i]), &(h_array[i]), const_table);
            data_array++;
        }
        //printf("%d: %f %f %f %f\n", step, *(data_array-1), m_array[0], n_array[0], h_array[0]);
    }

    free(m_array);
    free(n_array);
    free(h_array);
}
*/

}