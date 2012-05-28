// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
// rights reserved.

// License to copy and use this software is granted provided that it
// is identified as the "RSA Data Security, Inc. MD5 Message-Digest
// Algorithm" in all material mentioning or referencing this software
// or this function.
//
// License is also granted to make and use derivative works provided
// that such works are identified as "derived from the RSA Data
// Security, Inc. MD5 Message-Digest Algorithm" in all material
// mentioning or referencing the derived work.
//
// RSA Data Security, Inc. makes no representations concerning either
// the merchantability of this software or the suitability of this
// software for any particular purpose. It is provided "as is"
// without express or implied warranty of any kind.
//
// These notices must be retained in any copies of any part of this
// documentation and/or software.

// The original md5 implementation avoids external libraries.
// This version has dependency on stdio.h for file input and
// string.h for memcpy.

#include <stdio.h>
#include <string.h>

#ifndef YROMD5_H_
#define YROMD5_H_

namespace std {

// BYTE defines a generic pointer type
typedef unsigned char BYTE;

// POINTER defines a generic pointer type
typedef unsigned char *POINTER;

// UINT2 defines a two byte word
typedef unsigned short int UINT2;

// UINT4 defines a four byte word
typedef unsigned long int UINT4;

class YroMd5 {
public:
	YroMd5();
	virtual ~YroMd5();
private:
	struct __context_t {
		UINT4 state[4]; /* state (ABCD) */
		UINT4 count[2]; /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64]; /* input buffer */
	} context;
	static void MD5Transform(UINT4 state[4], unsigned char block[64]);
	static void Encode(unsigned char *output, UINT4 *input, unsigned int len);
	static void Decode(UINT4 *output, unsigned char *input, unsigned int len);

	// an MD5 digest is a 16-byte number (32 hex digits)
	BYTE digestRaw[16];

	// This version of the digest is actually
	// a "printf'd" version of the digest.
	char digestChars[33];

public:
	void Init();
	void Update(unsigned char *input, unsigned int inputLen);
	void Final();
	void writeToString();

public:
	char* digestFile(char *filename);
	char* digestMemory(BYTE *memchunk, long unsigned int len);
	char* digestString(char *string);
};

} /* namespace std */
#endif /* YROMD5_H_ */
