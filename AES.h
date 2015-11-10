//
//  AES.h
//  First C
//
//  Created by Minh Tri Tran on 5/9/15.
//  Copyright (c) 2015 Minh Tri Tran. All rights reserved.
//

#ifndef First_C_AES_h
#define First_C_AES_h

#include "fundamentals.h"

void byteSub(R_BYTE* State);
void roundTransform(R_BYTE state[][Rijndael::NB], R_BYTE* rounded_key);
void keyExpansion(R_BYTE key[][Rijndael::NK], R_BYTE W[][Rijndael::NB * (Rijndael::NR + 1)]);
void makeT(R_BYTE T[4][4][256]);
void AES(R_BYTE state[][Rijndael::NB], R_BYTE key[][Rijndael::NK]);

#endif
