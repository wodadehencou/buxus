
#ifndef _BUXUS_TYPE_H_
#define _BUXUS_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BIT32
	#ifndef BIT64
		#define BIT32
	#endif
#endif

#ifdef BIT32
	#define BIGINT_LEN 8
	#define MSB_MASK 0x80000000
#else
	#define BIGINT_LEN 4
	#define MSB_MASK 0x8000000000000000
#endif

typedef unsigned char UINT8;
typedef unsigned int UINT32;
//typedef size_t BIGINT;
typedef unsigned int BIGINT;

//typedef struct {
//	INT d [BIGINT_LEN],
//} BIGINT;
//typedef BIGINT_st* BIGINT;

typedef struct {
	BIGINT x [BIGINT_LEN];
	BIGINT y [BIGINT_LEN];
} AFFPOINT;
//typedef AFFPOINT_st* AFFPOINT;

typedef struct {
	BIGINT x [BIGINT_LEN];
	BIGINT y [BIGINT_LEN];
	BIGINT z [BIGINT_LEN];
} JPOINT;
//typedef JPOINT_st* JPOINT;

typedef struct {
	BIGINT p		[BIGINT_LEN]	;
	BIGINT order	[BIGINT_LEN]	;
	AFFPOINT g;
} ECGROUP;
//typedef ECGROUP_st* ECGROUP;

#ifdef __cplusplus
}
#endif

#endif
