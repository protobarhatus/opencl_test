#define ___META___
//this is for highlightning in redactor, and it is cut when loaded
#define __kernel
#define __global
#define __local
int get_local_id(int);
int get_group_id(int);
#define CLK_LOCAL_MEM_FENCE
void barrier();
#undef ___META___


#define ___EXTERNAL___
//there everything is also cut and loader will insert their own values
#define TS 1
#define _TYPE int
#undef ___EXTERNAL___

__kernel void gemm(__global _TYPE* A, __global _TYPE* B, __global _TYPE* C, int AX, int AY, int BY,
	_TYPE alpha, _TYPE beta)
{

	int k, t;
	const int row = get_local_id(0);                  // Local row ID (max: TS)
	const int col = get_local_id(1);                  // Local col ID (max: TS)
	const int globalRow = TS * get_group_id(0) + row; // Row ID of C (0..M)
	const int globalCol = TS * get_group_id(1) + col; // Col ID of C (0..N)

	__local _TYPE Asub[TS][TS];
	__local _TYPE Bsub[TS][TS];

	_TYPE acc = 0;

	const int numTiles = AY / TS;

	for (t = 0; t < numTiles; t++) {
		const int tiledRow = TS * t + row;
		const int tiledCol = TS * t + col;
		Asub[col][row] = A[globalRow * AY + tiledCol];
		Bsub[col][row] = B[tiledRow * BY + globalCol];

		// Synchronise to make sure the tile is loaded
		barrier(CLK_LOCAL_MEM_FENCE);

		for (k = 0; k < TS; k++)
			acc += Asub[k][row] * Bsub[col][k];

		// Synchronise before loading the next tile
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	// Store the final result in C
	C[globalRow * BY + globalCol] = alpha * acc + beta * C[globalRow * BY + globalCol];
}