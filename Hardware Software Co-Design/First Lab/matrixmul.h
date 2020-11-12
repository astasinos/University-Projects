#include <ap_int.h>
#define lm 8
#define ln 8
#define lp 8
#define m (1 << lm) // m = 2**(lm)
#define n (1 << ln) // n = 2**(ln)
#define p (1 << lp) // p = 2**(lp)

// 1 <= lm,ln,lp <= 8

void matrix_mul_hw(ap_uint<8> inArray1[n][m], ap_uint<8> inArray2[m][p], unsigned int outArray[n][p]);

