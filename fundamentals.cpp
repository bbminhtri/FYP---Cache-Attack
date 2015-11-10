//
//  constants.cpp
//  First C
//
//  Created by Minh Tri Tran on 26/8/15.
//  Copyright (c) 2015 Minh Tri Tran. All rights reserved.
//

#include "fundamentals.h"

R_BYTE Rijndael::xtime(R_BYTE a) {
	return ((a & 0x80) ? ((a << 1) ^ (0x1B)) : (a << 1));
}

R_BYTE Rijndael::r_multiply(R_BYTE a, R_BYTE b) {
	R_BYTE result = 0x00;
	for (int i = 0; i < 8; i ++) {
		if (b & (1 << i)) {
			R_BYTE tempA = a;
			for (int j = 0; j < i; j ++)
				tempA = xtime(tempA);
			result ^= tempA;
		}
	}
	return result;
}

void Rijndael::vectorMultiply(R_BYTE* rVector, R_BYTE targetTable[][256]) { //256 * 4
	for (int i = 0; i < N_ROW; i ++) {
		for (int j = 0; j < S_BOX_SIZE; j ++) {
			targetTable[i][j] = r_multiply(Rijndael::S_BOX[j], rVector[i]);
		}
	}
}

int Rijndael::shiftOffset(int i) {
	return SHIFT_OFFSET[(Rijndael::NB - 4)/2][i - 1];
}

int Rijndael::getRoundNo() {
	return NO_ROUND[(Rijndael::NB - 4) / 2][(Rijndael::NK - 4) / 2];
}