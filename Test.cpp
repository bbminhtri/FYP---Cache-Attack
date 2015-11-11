
//Test.cpp

#include "Rijndael.h"
#include <iostream>
//#include <mach/mach_time.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <errno.h>

using namespace std;
//double conversion_factor;
//
//void Init() {
//	mach_timebase_info_data_t timebase;
//	mach_timebase_info(&timebase);
//	conversion_factor = (double)timebase.numer / (double)timebase.denom;
//}

//Function to convert unsigned char to string of length 2
void Char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//Function to convert string of length 2 to unsigned char
void Hex2Char(char const* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}    

//Function to convert string of unsigned chars to string of chars
void CharStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
void HexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}

void test() {
	
	try
	{
		char szHex[33];
		//One block testing
		CRijndael oRijndael;
		oRijndael.MakeKey("abcdefghabcdefgh", 16, 16);
		char szDataIn[] = "aaaaaaaabbbbbbbb";
		char szDataOut[17] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
		oRijndael.EncryptBlock(szDataIn, szDataOut);
		CharStr2HexStr((unsigned char*)szDataIn, szHex, 16);
		cout << szHex << endl;
		CharStr2HexStr((unsigned char*)szDataOut, szHex, 16);
		cout << szHex << endl;
		memset(szDataIn, 0, 16);
		oRijndael.DecryptBlock(szDataOut, szDataIn);
		CharStr2HexStr((unsigned char*)szDataIn, szHex, 16);
		cout << szHex << endl;
	}
	catch(exception& roException)
	{
		cout << roException.what() << endl;
	}
}
//
//void cacheAttack() {
//	const int KEY_SIZE = 16;
//	const int CHAR_SIZE = 255;
//	const int ATTACK_NUM = 1000111;
//	
//	//Init models and keys
//	CRijndael studyPhase;
//	char studyKey[KEY_SIZE + 1];
//	for(int i = 0; i < KEY_SIZE; i ++)
//		studyKey[i] = 0;
//	studyPhase.MakeKey(studyKey, KEY_SIZE, KEY_SIZE);
//	
//	CRijndael attackPhase;
//	char attackKey[] = "abcdefgh12345678";
//	attackPhase.MakeKey(attackKey, KEY_SIZE, KEY_SIZE);
//	
//	//Initialization
//	double t1[20][1011];
//	double t2[20][1011];
//	int n1[20][1011];
//	int n2[20][1011];
//	double v1[20][1011];
//	double v2[20][1011];
//	
//	memset(t1, 0, sizeof(t1));
//	memset(t2, 0, sizeof(t2));
//	memset(n1, 0, sizeof(n1));
//	memset(n2, 0, sizeof(n2));
//	memset(v1, 0, sizeof(v1));
//	memset(v2, 0, sizeof(v2));
//	
//	double sumT1 = 0;
//	double sumN1 = 0;
//	double sumT2 = 0;
//	double sumN2 = 0;
//	
//	srand(time(NULL));
//	
//	//Study Phase
//	for(int attackNo = 0; attackNo < ATTACK_NUM; attackNo ++) {
//		char plainText[KEY_SIZE + 1];
//		char encryptedText[KEY_SIZE + 1];
//		
//		for(int i = 0; i < KEY_SIZE; i ++)
//			plainText[i] = rand() % CHAR_SIZE;
//		
//		long long measure1 = mach_absolute_time();
//		studyPhase.Encrypt(plainText, encryptedText, KEY_SIZE);
//		long long measure2 = mach_absolute_time();
//		
//		double duration_ns = (double)(measure2 - measure1) * conversion_factor;
//		
////		cout << attackNo << ": " << duration_ns << endl;
//		
//		if (attackNo > 10) {
//			for(int i = 0; i < KEY_SIZE; i ++) {
//				t1[i][encryptedText[i]] += duration_ns;
//				sumT1 += duration_ns;
//				n1[i][encryptedText[i]] ++;
//				sumN1 ++;
//			}
//		}
//	}
//	
//	for(int i = 0; i < KEY_SIZE; i ++)
//		for(int j = 0; j < CHAR_SIZE; j ++) {
//			if (n1[i][j] > 0) {
//				v1[i][j] = t1[i][j] / n1[i][j] - sumT1 / sumN1;
//			}
//		}
//	
//	//Attack phase
//
//	for(int attackNo = 0; attackNo < ATTACK_NUM; attackNo ++) {
//		char plainText[KEY_SIZE + 1];
//		char encryptedText[KEY_SIZE + 1];
//		
//		for(int i = 0; i < KEY_SIZE; i ++)
//			plainText[i] = rand() % CHAR_SIZE;
//		
//		long long measure1 = mach_absolute_time();
//		attackPhase.Encrypt(plainText, encryptedText, KEY_SIZE);
//		long long measure2 = mach_absolute_time();
//		
//		double duration_ns = (double)(measure2 - measure1) * conversion_factor;
//		
////		cout << attackNo << ": " << duration_ns << endl;
//		
//		if (attackNo > 10) {
//			for(int i = 0; i < KEY_SIZE; i ++) {
//				t2[i][encryptedText[i]] += duration_ns;
//				sumT2 += duration_ns;
//				n2[i][encryptedText[i]] ++;
//				sumN2 ++;
//			}
//		}
//	}
//	
//	for(int i = 0; i < KEY_SIZE; i ++)
//		for(int j = 0; j < CHAR_SIZE; j ++) {
//			if (n2[i][j] > 0) {
//				v2[i][j] = t2[i][j] / n2[i][j] - sumT2 / sumN2;
//			}
//		}
//	
//	double c[20][1011];
//	for(int i = 0; i < KEY_SIZE; i ++) {
//		double maxNum = 0;
//		for(int b = 0; b < CHAR_SIZE; b ++) {
//			c[i][b] = 0;
//			for(int j = 0; j < CHAR_SIZE; j ++)
//				c[i][b] += v1[i][j] * v2[i][j ^ b];
//			if (c[i][b] > maxNum)
//				maxNum = c[i][b];
//		}
//		
//		double cor[255];
//		int trace[255];
//		for(int b = 0; b < CHAR_SIZE; b ++) {
//			cor[b] = c[i][b];
//			trace[b] = b;
//		}
//		for(int j1 = 0; j1 < CHAR_SIZE; j1 ++)
//			for(int j2 = j1 + 1; j2 < CHAR_SIZE; j2 ++) {
//				if (cor[j2] > cor[j1]) {
//					double tmp = cor[j1];
//					cor[j1] = cor[j2];
//					cor[j2] = tmp;
//					int tmpTrace = trace[j1];
//					trace[j1] = trace[j2];
//					trace[j2] = tmpTrace;
//				}
//			}
//		int keyPos = 0;
//		for(int j = 0; j < CHAR_SIZE; j ++)
//			if (trace[j] == attackKey[i])
//				keyPos = j;
//		cout << "i: " << i << endl;
//		cout << keyPos << endl;
////		cout << maxNum << " " << " " << cor[0] << " " << c[i][attackKey[i]] << endl;
//	}
//	
//}

int main() {
//	Init();
//	cacheAttack();
	int rst;
	
	struct timespec to;
	
	
	
	rst = clock_gettime(CLOCK_MONOTONIC, &to);
	
	cout << rst << endl;
//
//	cout << "hello" << endl;
//
//	if(rst != 0)
//		
//	{
//		
//		printf("Got some error and error code may be %d
//			   %d\n",rst,errno);
//			   
//			   }
//			   
//			   else
//			   
//			   {
//				   
//				   printf("clock_gettime returned=%d", rst);
//				   
//				   printf("rst=%d to.to_sec=%d", rst, to.tv_sec);
//				   
//				   
//				   
//			   }
	return 0;
}

