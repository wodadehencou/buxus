#ifndef _SM2_H_
#define _SM2_H_

//operation number length of word
#define DIG_LEN 8

//type of small and big
typedef unsigned int small;
typedef small* big;

//define ECC data structure
typedef struct {
	small x[DIG_LEN];
	small y[DIG_LEN];
} affpoint;

typedef affpoint* epoint;

typedef struct {
	small x[DIG_LEN];
	small y[DIG_LEN];
	small z[DIG_LEN];
} projpoint;

typedef projpoint* point;

//define ECC parameters: little endian
//size of GF(p)
#define M 256

//p=2^256-2^224-2^96+2^64-1
static small P[DIG_LEN] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
};
//a=-3
static small A[DIG_LEN] = {
	0xFFFFFFFC, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
};
//b
static small B[DIG_LEN] = {
	0x4D940E93, 0xDDBCBD41, 0x15AB8F92, 0xF39789F5,
	0xCF6509A7, 0x4D5A9E4B, 0x9D9F5E34, 0x28E9FA9E,
};

//y^2=x^3+ax+b
static small N[DIG_LEN] = {
	0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
};
static affpoint G = {
	{
		0x334C74C7, 0x715A4589, 0xF2660BE1, 0x8FE30BBF,
		0x6A39C994, 0x5F990446, 0x1F198119, 0x32C4AE2C,
	},
	{
		0x2139F0A0, 0x02DF32E5, 0xC62A4740, 0xD0A9877C,
		0x6B692153, 0x59BDCEE3, 0xF4F6779C, 0xBC3736A2,
	},
};


//-----------------------------------------------------
//  API
// ECC sign
int EccSign(unsigned char *hash, unsigned int hash_len,
			unsigned char *random, unsigned int random_len,
			unsigned char *sk, unsigned int sk_len,
			unsigned char *sign, unsigned int *sign_len);
//Input:
//		hash,		first address of hash
//		hash_len,	hash length of bytes
//		random,		first address of random
//		random_len,	random length of bytes
//		sk,			first address of privite key
//		sk_len,		privite key length of bytes
//Output:
//		sign,		first address of sign data
//		sign_len,	sign data length of bytes
//Return:
//		0,			compute right
//		-1,			invalid input

// ECC verify
int EccVerify(unsigned char *hash, unsigned int hash_len,
			  unsigned char *pk, unsigned int pk_len,
			  unsigned char *sign, unsigned int sign_len);
//Input:
//		hash,		first address of hash
//		hash_len,	hash length of bytes
//		pk,			first address of public key
//		pk_len,		public key length of bytes
//		sign,		first address of sign data
//		sign_len,	sign data length of bytes
//Output:
//		None
//Return:
//		0,			verify success
//		1,			verify fail
//		-1,			invalid input

// generate public key
int EccMakeKey(unsigned char *sk, unsigned int sk_len,
			   unsigned char *pk, unsigned int *pk_len);
//输入：
//		sk，			私钥首地址
//      sk_len，		私钥长度
//      type，			曲线类型
//输出：
//		pk，			公钥首地址
//      pk_len，		公钥长度
//返回值：
//		0，				运算成功
//		-1，			输入非法

//Input:
//		sk,			first address of privite key
//		sk_len,		privite key length of bytes
//Output:
//		pk,			first address of public key
//		pk_len,		public key length of bytes
//Return:
//		0,			generate success
//		-1,			invalid input

#endif
