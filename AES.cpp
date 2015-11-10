//
//  AES.cpp
//  First C
//
//  Created by Minh Tri Tran on 5/9/15.
//  Copyright (c) 2015 Minh Tri Tran. All rights reserved.
//

#include <stdio.h>
#include "AES.h"
#include "fundamentals.h"

void byteSub(R_BYTE* State) {
	for(int i = 0; i < 4; i ++)
		std::cout << State[i] << std::endl;
}

void makeT(R_BYTE T[4][4][256]) {
	R_BYTE tableVectors[4][4] = {
		{0x02, 0x01, 0x01, 0x03},
		{0x03, 0x02, 0x01, 0x01},
		{0x01, 0x03, 0x02, 0x01},
		{0x01, 0x01, 0x03, 0x02}
	};
	for (int j = 0; j < 4; j ++)
		Rijndael::vectorMultiply(tableVectors[j], T[j]);
}

void roundTransform(R_BYTE state[][Rijndael::NB], R_BYTE rounded_key[][Rijndael::NB]) {
	R_BYTE T[4][4][256];
	makeT(T);
	R_BYTE newState[4][Rijndael::NB];
	for(int j = 0; j < Rijndael::NB; j ++) {
		for(int i = 0; i < 4; i ++) {
			newState[i][j] = T[0][i][state[0][j]] ^ T[1][i][state[1][(j + Rijndael::NB - Rijndael::shiftOffset(1))]]
			^ T[2][i][state[2][(j + Rijndael::NB - Rijndael::shiftOffset(2))]]
			^ T[3][i][state[3][(j + Rijndael::NB - Rijndael::shiftOffset(3))]]
			^ rounded_key[i][j];
		}
	}
	
	for(int i = 0; i < 4; i ++)
		for(int j = 0; j < Rijndael::NB; j++) {
			state[i][j] = newState[i][j];
		}
}

void rotByte(R_BYTE* word) {
	R_BYTE temp[4];
	for(int i = 0; i < 3; i ++)
		temp[i] = word[i + 1];
	temp[3] = word[0];
	for(int i = 0; i < 4; i ++)
		word[i] = temp[i];
}

void subByte(R_BYTE* word) {
	for(int i = 0; i < 4; i ++)
		word[i] = Rijndael::S_BOX[word[i]];
}

void keyExpansion(R_BYTE key[][Rijndael::NK], R_BYTE W[][Rijndael::NB * (Rijndael::NR + 1)]) {
	for(int j = 0; j < Rijndael::NK; j++)
		for(int i = 0; i < 4; i ++)
			W[i][j] = key[i][j];
	
	R_BYTE RC[101];
	RC[0] = 0x01;
	for(int i = 1; i < 100; i ++)
		RC[i] = Rijndael::r_multiply(0x02, RC[i - 1]);
	R_BYTE Rcon[4][101];
	for(int j = 0; j < 100; j ++) {
		Rcon[0][j] = RC[j];
		for(int i = 1; i < 4; i++)
			Rcon[i][j] = 0x00;
	}
	
	for(int j = Rijndael::NK; j < Rijndael::NB * (Rijndael::NR + 1); j++) {
		R_BYTE temp[4];
		for(int i = 0; i < 4; i ++) {
			temp[i] = W[i][j - 1];
		}
		if (j % Rijndael::NK == 0) {
			rotByte(temp);
			subByte(temp);
			for(int i = 0; i < 4; i ++) {
				temp[i] = temp[i] ^ Rcon[i][j / Rijndael::NK];
			}
		}
		else {
			if (j % Rijndael::NK == 4) {
				subByte(temp);
			}
		}
		for(int i = 0; i < 4; i ++)
			W[i][j] = W[i][j - Rijndael::NK] ^ temp[i];
	}
}



void AES(R_BYTE state[][Rijndael::NB], R_BYTE key[][Rijndael::NK]) {
	R_BYTE W[4][Rijndael::NB * (Rijndael::NR + 1)];
	keyExpansion(key, W);
	int nr = Rijndael::getRoundNo();
	for(int i = 0; i < nr; i ++) {
		R_BYTE newKey[4][Rijndael::NB];
		for (int j = i * Rijndael::NB; j < (i + 1) * Rijndael::NB; j ++) {
			for (int k = 0; k < 4; k ++)
				newKey[k][j - i * Rijndael::NB] = W[k][j];
		}
		roundTransform(state, newKey);
	}
}