// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/**************************** sha.h ****************************/
/******************* See RFC 4634 for details ******************/
#ifndef _SHA_H_
#define _SHA_H_

/*
 *  Description:
 *      This file implements the Secure Hash Signature Standard
 *      algorithms as defined in the National Institute of Standards
 *      and Technology Federal Information Processing Standards
 *      Publication (FIPS PUB) 180-1 published on April 17, 1995, 180-2
 *      published on August 1, 2002, and the FIPS PUB 180-2 Change
 *      Notice published on February 28, 2004.
 *
 *      A combined document showing all algorithms is available at
 *              http://csrc.nist.gov/publications/fips/
 *              fips180-2/fips180-2withchangenotice.pdf
 *
 *      The five hashes are defined in these sizes:
 *              SHA-1           20 byte / 160 bit
 *              SHA-224         28 byte / 224 bit
 *              SHA-256         32 byte / 256 bit
 *              SHA-384         48 byte / 384 bit
 *              SHA-512         64 byte / 512 bit
 */

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typedef the following:
 *    name              meaning
 *  uint64_t         unsigned 64 bit integer
 *  uint32_t         unsigned 32 bit integer
 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
 *  int_least16_t    integer of >= 16 bits
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _SHA_enum_
#define _SHA_enum_
/*
 *  All SHA functions return one of these values.
 */
enum {
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError,      /* called Input after FinalBits or Result */
    shaBadParam         /* passed a bad parameter */
};
#endif /* _SHA_enum_ */

/*
 *  These constants hold size information for each of the SHA
 *  hashing operations
 */
enum {
    SHA1_Message_Block_Size = 64, SHA224_Message_Block_Size = 64,
    SHA256_Message_Block_Size = 64, SHA384_Message_Block_Size = 128,
    SHA512_Message_Block_Size = 128,
    USHA_Max_Message_Block_Size = SHA512_Message_Block_Size,

    SHA1HashSize = 20, SHA224HashSize = 28, SHA256HashSize = 32,
    SHA384HashSize = 48, SHA512HashSize = 64,
    USHAMaxHashSize = SHA512HashSize,

    SHA1HashSizeBits = 160, SHA224HashSizeBits = 224,
    SHA256HashSizeBits = 256, SHA384HashSizeBits = 384,
    SHA512HashSizeBits = 512, USHAMaxHashSizeBits = SHA512HashSizeBits
};

/*
 *  These constants are used in the USHA (unified sha) functions.
 */
typedef enum SHAversion {
    SHA1, SHA224, SHA256, SHA384, SHA512
} SHAversion;

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation.
 */
typedef struct SHA1Context {
    uint32_t Intermediate_Hash[SHA1HashSize/4]; /* Message Digest */

    uint32_t Length_Low;                /* Message length in bits */
    uint32_t Length_High;               /* Message length in bits */

    int_least16_t Message_Block_Index;  /* Message_Block array index */
                                        /* 512-bit message blocks */
    uint8_t Message_Block[SHA1_Message_Block_Size];

    int Computed;                       /* Is the digest computed? */
    int Corrupted;                      /* Is the digest corrupted? */
} SHA1Context;

/*
 *  This structure will hold context information for the SHA-256
 *  hashing operation.
 */
typedef struct SHA256Context {
    uint32_t Intermediate_Hash[SHA256HashSize/4]; /* Message Digest */

    uint32_t Length_Low;                /* Message length in bits */
    uint32_t Length_High;               /* Message length in bits */

    int_least16_t Message_Block_Index;  /* Message_Block array index */
                                        /* 512-bit message blocks */
    uint8_t Message_Block[SHA256_Message_Block_Size];

    int Computed;                       /* Is the digest computed? */
    int Corrupted;                      /* Is the digest corrupted? */
} SHA256Context;

/*
 *  This structure will hold context information for the SHA-512
 *  hashing operation.
 */
typedef struct SHA512Context {
#ifdef USE_32BIT_ONLY
    uint32_t Intermediate_Hash[SHA512HashSize/4]; /* Message Digest  */
    uint32_t Length[4];                 /* Message length in bits */
#else /* !USE_32BIT_ONLY */
    uint64_t Intermediate_Hash[SHA512HashSize/8]; /* Message Digest */
    uint64_t Length_Low, Length_High;   /* Message length in bits */
#endif /* USE_32BIT_ONLY */

    int_least16_t Message_Block_Index;  /* Message_Block array index */
                                        /* 1024-bit message blocks */
    uint8_t Message_Block[SHA512_Message_Block_Size];

    int Computed;                       /* Is the digest computed?*/
    int Corrupted;                      /* Is the digest corrupted? */
} SHA512Context;

/*
 *  This structure will hold context information for the SHA-224
 *  hashing operation. It uses the SHA-256 structure for computation.
 */
typedef struct SHA256Context SHA224Context;

/*
 *  This structure will hold context information for the SHA-384
 *  hashing operation. It uses the SHA-512 structure for computation.
 */
typedef struct SHA512Context SHA384Context;

/*
 *  This structure holds context information for all SHA
 *  hashing operations.
 */
typedef struct USHAContext {
    int whichSha;               /* which SHA is being used */
    union {
      SHA1Context sha1Context;
      SHA224Context sha224Context; SHA256Context sha256Context;
      SHA384Context sha384Context; SHA512Context sha512Context;
    } ctx;
} USHAContext;

/*
 *  This structure will hold context information for the HMAC
 *  keyed hashing operation.
 */
typedef struct HMACContext {
    int whichSha;               /* which SHA is being used */
    int hashSize;               /* hash size of SHA being used */
    int blockSize;              /* block size of SHA being used */
    USHAContext shaContext;     /* SHA context */
    unsigned char k_opad[USHA_Max_Message_Block_Size];
                        /* outer padding - key XORd with opad */
} HMACContext;


/*
 *  Function Prototypes
 */

/* SHA-1 */
int SHA1Reset(SHA1Context *);
int SHA1Input(SHA1Context *, const uint8_t *bytes, unsigned int bytecount);
int SHA1FinalBits(SHA1Context *, const uint8_t bits, unsigned int bitcount);
int SHA1Result(SHA1Context *, uint8_t Message_Digest[SHA1HashSize]);

/* SHA-224 */
int SHA224Reset(SHA224Context *);
int SHA224Input(SHA224Context *, const uint8_t *bytes, unsigned int bytecount);
int SHA224FinalBits(SHA224Context *, const uint8_t bits, unsigned int bitcount);
int SHA224Result(SHA224Context *, uint8_t Message_Digest[SHA224HashSize]);

/* SHA-256 */
int SHA256Reset(SHA256Context *);
int SHA256Input(SHA256Context *, const uint8_t *bytes, unsigned int bytecount);
int SHA256FinalBits(SHA256Context *, const uint8_t bits, unsigned int bitcount);
int SHA256Result(SHA256Context *, uint8_t Message_Digest[SHA256HashSize]);

/* SHA-384 */
int SHA384Reset(SHA384Context *);
int SHA384Input(SHA384Context *, const uint8_t *bytes, unsigned int bytecount);
int SHA384FinalBits(SHA384Context *, const uint8_t bits, unsigned int bitcount);
int SHA384Result(SHA384Context *, uint8_t Message_Digest[SHA384HashSize]);

/* SHA-512 */
int SHA512Reset(SHA512Context *);
int SHA512Input(SHA512Context *, const uint8_t *bytes, unsigned int bytecount);
int SHA512FinalBits(SHA512Context *, const uint8_t bits, unsigned int bitcount);
int SHA512Result(SHA512Context *, uint8_t Message_Digest[SHA512HashSize]);

/* Unified SHA functions, chosen by whichSha */
int USHAReset(USHAContext *, SHAversion whichSha);
int USHAInput(USHAContext *, const uint8_t *bytes, unsigned int bytecount);
int USHAFinalBits(USHAContext *, const uint8_t bits, unsigned int bitcount);
int USHAResult(USHAContext *, uint8_t Message_Digest[USHAMaxHashSize]);
int USHABlockSize(enum SHAversion whichSha);
int USHAHashSize(enum SHAversion whichSha);
int USHAHashSizeBits(enum SHAversion whichSha);

/*
 * HMAC Keyed-Hashing for Message Authentication, RFC2104,
 * for all SHAs.
 * This interface allows a fixed-length text input to be used.
 */
int hmac(SHAversion whichSha, /* which SHA algorithm to use */
    const unsigned char *text,     /* pointer to data stream */
    int text_len,                  /* length of data stream */
    const unsigned char *key,      /* pointer to authentication key */
    int key_len,                   /* length of authentication key */
    uint8_t digest[USHAMaxHashSize]); /* caller digest to fill in */

/*
 * HMAC Keyed-Hashing for Message Authentication, RFC2104,
 * for all SHAs.
 * This interface allows any length of text input to be used.
 */
int hmacReset(HMACContext *ctx, enum SHAversion whichSha, const unsigned char *key, int key_len);
int hmacInput(HMACContext *ctx, const unsigned char *text, int text_len);

int hmacFinalBits(HMACContext *ctx, const uint8_t bits, unsigned int bitcount);
int hmacResult(HMACContext *ctx, uint8_t *digest);


#ifdef __cplusplus
}
#endif

#endif /* _SHA_H_ */

