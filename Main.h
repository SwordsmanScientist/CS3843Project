// Main.h
//
// This file is the main header file for the CS3843 Encrypt/Decrypt project
//

// Include Files
#include <windows.h>
#include <stdio.h>
#include <io.h>

// Defines
// #define TEST_CODE

#define CRYPTO_ORDER "ABCDE\0"

// Structures


// Prototypes
int sha256(char *fileName, char *dataBuffer, DWORD dataLength, unsigned char sha256sum[32]);
int encryptData(char *data, int length);
int decryptData(char *data, int length);

// Global Variable Extern Declarations
extern unsigned char gkey[65537];
extern unsigned char *gptrKey;			// used for inline assembly routines, need to access this way for Visual Studio
extern char gPassword[256];
extern unsigned char gPasswordHash[32];
extern unsigned char *gptrPasswordHash;	// used for inline assembly routines, need to access this way for Visual Studio
extern unsigned char gdebug1, gdebug2;

extern FILE *gfptrIn;
extern FILE *gfptrOut;
extern FILE *gfptrKey;
extern char gInFileName[256];
extern char gOutFileName[256];
extern char gKeyFileName[256];
extern int gOp;			// 1 = encrypt, 2 = decrypt
extern int gNumRounds;

extern unsigned char gEncodeTable[256];
extern unsigned char gDecodeTable[256];


