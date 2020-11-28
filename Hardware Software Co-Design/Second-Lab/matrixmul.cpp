#include "MatrixMul.h"

// for tripcount pragmas
const int n_iter = n;
const int p_iter = p;
const int m_iter = m;

extern "C" {

void ARRAY_COPY1(ap_int<32> *inArray1,ap_int<32> A[n][m]){

	for(int i = 0; i < n; i++){
		#pragma HLS loop_tripcount min=n_iter max=n_iter
			for(int j = 0; j < m; j++){
				#pragma HLS loop_tripcount min=m_iter max=m_iter
				#pragma HLS UNROLL factor=2
				#pragma HLS PIPELINE II=1
				A[i][j] = inArray1[i*m + j];
			}
		}

}

void ARRAY_COPY2(ap_int<32> *inArray2,ap_int<32> B[m][p]){

	for(int i = 0; i < m; i++){
		#pragma HLS loop_tripcount min=m_iter max=m_iter
			for(int j = 0; j < p; j++){
				#pragma HLS loop_tripcount min=p_iter max=p_iter
				#pragma HLS UNROLL factor=2
				#pragma HLS PIPELINE II=1
				B[i][j] = inArray2[i*p + j];
			}
		}

}

void vadd(ap_int<32> *inArray1, ap_int<32> *inArray2,
		ap_int<32> *outArray){
	
	#pragma HLS INTERFACE m_axi port = inArray1 offset = slave bundle = gmem
	#pragma HLS INTERFACE m_axi port = inArray2 offset = slave bundle = gmem
	#pragma HLS INTERFACE m_axi port = outArray offset = slave bundle = gmem
	#pragma HLS INTERFACE s_axilite port = inArray1 bundle = control
	#pragma HLS INTERFACE s_axilite port = inArray2 bundle = control
	#pragma HLS INTERFACE s_axilite port = outArray bundle = control
	#pragma HLS INTERFACE s_axilite port = return bundle = control

	//#pragma HLS DATAFLOW
	
	ap_int<32> BRAM_in1[n][m];
	ap_int<32> BRAM_in2[m][p];

	#pragma HLS ARRAY_PARTITION variable=BRAM_in1 cyclic factor=m_iter dim=2
	#pragma HLS ARRAY_PARTITION variable=BRAM_in2 cyclic factor=m_iter dim=1

	ARRAY_COPY1(inArray1,BRAM_in1);
	ARRAY_COPY2(inArray2,BRAM_in2);

	int result;

	for(int i = 0; i < n; i++){
	#pragma HLS loop_tripcount min=n_iter max=n_iter

		for(int j = 0; j < p; j++) {
		#pragma HLS loop_tripcount min=p_iter max=p_iter
		#pragma HLS UNROLL factor=2
		#pragma HLS PIPELINE II=1

			result = 0;
	mainloop: for(int k = 0; k < m; k++){
				#pragma HLS loop_tripcount min=m_iter max=m_iter

				result += BRAM_in1[i][k] * BRAM_in2[k][j];
}

			outArray[i*p + j] = result;
		}

	}
}
}
