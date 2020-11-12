#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "matrixmul.h"

void matrix_mul_sw(ap_uint<8> inArray1[n][m], ap_uint<8> inArray2[m][p],
                   unsigned int outArray[n][p])
{

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < p; j++)
        {
            for(int k = 0; k < m; k++)
            {
                outArray[i][j] += inArray1[i][k] * inArray2[k][j];
            }
        }
    }
}

int main(int argc, char** argv)
{



    ap_uint<8> arr1[n][m];
    ap_uint<8> arr2[m][p];
    unsigned int hw_out[n][p];
    unsigned int sw_out[n][p];

    // Initialize input arrays with random numbers in the range of 0 - 255
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            arr1[i][j] = rand() % 255;
        }
    }

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < p; j++)
        {
            arr2[i][j] = rand() % 255;
        }
    }

    // Initialize output arrays to zero
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < p; j++)
        {

            hw_out[i][j] = 0;
            sw_out[i][j] = 0;

        }
    }

    // Call Hardware function
    matrix_mul_hw(arr1, arr2, hw_out);

    // Call Software function
    auto t1 = std::chrono::high_resolution_clock::now();

    matrix_mul_sw(arr1, arr2, sw_out);

    auto t2 = std::chrono::high_resolution_clock::now();

    int allgood = 1;
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0; j < p; j++)
        {
            if(sw_out[i][j] != hw_out[i][j])
            {
                std::cout << "Bad Result!" << std::endl;
                std::cout << "CPU out: " << sw_out[i][j] << " HW out: " << hw_out[i][j] << std::endl;
                allgood = 0;
            }
        }
    }

    if(allgood)
        std::cout << "Test Passed" << std::endl;

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "Software function duration in us : " << duration << std::endl;
    return 0;

}
