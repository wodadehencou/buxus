
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

typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef unsigned long long int UINT64;
#ifdef BIT32
	#define BIGINT_LEN 8
	typedef UINT32 BIGINT;
#else
	#define BIGINT_LEN 4
	typedef UINT64 BIGINT;
#endif

typedef struct {
	BIGINT x [BIGINT_LEN];
	BIGINT y [BIGINT_LEN];
} AFFPOINT;

typedef struct {
	BIGINT x [BIGINT_LEN];
	BIGINT y [BIGINT_LEN];
	BIGINT z [BIGINT_LEN];
} JPOINT;

typedef struct {
	BIGINT p		[BIGINT_LEN]	;
	BIGINT order	[BIGINT_LEN]	;
	AFFPOINT g;
} ECGROUP;

#ifdef __cplusplus
}
#endif

#endif
