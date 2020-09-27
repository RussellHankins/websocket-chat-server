// "Borrowed" from https://github.com/B-Con/crypto-algorithms/blob/master/sha256.h
/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>
#include <cstdint>
/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
//typedef unsigned char BYTE;             // 8-bit byte
//typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

class sha256
{
	public:
	uint8_t data[64];
	uint32_t datalen;
	uint64_t bitlen;
	uint32_t state[8];
/*********************** FUNCTION DECLARATIONS **********************/
	void sha256_init();
	void sha256_update(const uint8_t input[], size_t len);
	void sha256_final(uint8_t hash[]); // hash should be 32 bytes long.
	private:
	void sha256_transform(const uint8_t input[]);
};



#endif   // SHA256_H
