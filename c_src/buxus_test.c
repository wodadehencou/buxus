#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "buxus_type.h"
#include "bigint.h"
#include "point.h"
#include "sm2_algo.h"

int sm2_function_test() {
	UINT8 g [64] = {
		0x32, 0xC4, 0xAE, 0x2C, 0x1F, 0x19, 0x81, 0x19,
		0x5F, 0x99, 0x04, 0x46, 0x6A, 0x39, 0xC9, 0x94,
		0x8F, 0xE3, 0x0B, 0xBF, 0xF2, 0x66, 0x0B, 0xE1,
		0x71, 0x5A, 0x45, 0x89, 0x33, 0x4C, 0x74, 0xC7,
		0xBC, 0x37, 0x36, 0xA2, 0xF4, 0xF6, 0x77, 0x9C,
		0x59, 0xBD, 0xCE, 0xE3, 0x6B, 0x69, 0x21, 0x53,
		0xD0, 0xA9, 0x87, 0x7C, 0xC6, 0x2A, 0x47, 0x40,
		0x02, 0xDF, 0x32, 0xE5, 0x21, 0x39, 0xF0, 0xA0,
	};
	UINT8 p [32] = {
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};
	UINT8 n [32] = {
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B,
		0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x23,
	};
	UINT8 sk [32] = {
		0x12, 0x8B, 0x2F, 0xA8, 0xBD, 0x43, 0x3C, 0x6C,
		0x06, 0x8C, 0x8D, 0x80, 0x3D, 0xFF, 0x79, 0x79,
		0x2A, 0x51, 0x9A, 0x55, 0x17, 0x1B, 0x1B, 0x65,
		0x0C, 0x23, 0x66, 0x1D, 0x15, 0x89, 0x72, 0x63,
	};
	UINT8 pk [64] = {
		0xD5, 0x54, 0x8C, 0x78, 0x25, 0xCB, 0xB5, 0x61,
		0x50, 0xA3, 0x50, 0x6C, 0xD5, 0x74, 0x64, 0xAF,
		0x8A, 0x1A, 0xE0, 0x51, 0x9D, 0xFA, 0xF3, 0xC5,
		0x82, 0x21, 0xDC, 0x81, 0x0C, 0xAF, 0x28, 0xDD,
		0x92, 0x10, 0x73, 0x76, 0x8F, 0xE3, 0xD5, 0x9C,
		0xE5, 0x4E, 0x79, 0xA4, 0x94, 0x45, 0xCF, 0x73,
		0xFE, 0xD2, 0x30, 0x86, 0x53, 0x70, 0x27, 0x26,
		0x4D, 0x16, 0x89, 0x46, 0xD4, 0x79, 0x53, 0x3E,
	};
	UINT8 hash [32] = {
		0xB5, 0x24, 0xF5, 0x52, 0xCD, 0x82, 0xB8, 0xB0,
		0x28, 0x47, 0x6E, 0x00, 0x5C, 0x37, 0x7F, 0xB1,
		0x9A, 0x87, 0xE6, 0xFC, 0x68, 0x2D, 0x48, 0xBB,
		0x5D, 0x42, 0xE3, 0xD9, 0xB9, 0xEF, 0xFE, 0x76,
	};
	UINT8 random [32] = {
		0x6C, 0xB2, 0x8D, 0x99, 0x38, 0x5C, 0x17, 0x5C,
		0x94, 0xF9, 0x4E, 0x93, 0x48, 0x17, 0x66, 0x3F,
		0xC1, 0x76, 0xD9, 0x25, 0xDD, 0x72, 0xB7, 0x27,
		0x26, 0x0D, 0xBA, 0xAE, 0x1F, 0xB2, 0xF9, 0x6F,
	};

	UINT8 sign_r [32] = {
		0xAB, 0xCB, 0x7C, 0xFF, 0x24, 0xC7, 0x8E, 0x7B,
		0xE2, 0x64, 0x67, 0x3D, 0xE0, 0x6D, 0xC1, 0x21,
		0x9E, 0x47, 0xB6, 0x55, 0x1D, 0xC9, 0xC4, 0xDD,
		0x5F, 0xDA, 0x04, 0x80, 0x75, 0x95, 0x47, 0x4C,
	};
	UINT8 sign_s [32] = {
		0x14, 0x5C, 0x15, 0xA7, 0x7D, 0x6A, 0x5B, 0x69,
		0x2F, 0xF0, 0x7B, 0x34, 0xCC, 0x8C, 0xA1, 0x81,
		0xDD, 0x23, 0x08, 0x44, 0xF8, 0xCC, 0x22, 0xD2,
		0x66, 0x1C, 0x60, 0x5E, 0x59, 0x51, 0x9D, 0x53,
	};
	UINT8 sign_r_out[32];
	UINT8 sign_s_out[32];
	ECGROUP group;
	int i;
	int ret;
	int flag = 0;

	group_set_p(&group, p);
	group_set_n(&group, n);
	group_set_g(&group, g);

	sm2_sign(hash, random, sk, sign_r_out, sign_s_out, &group);
	for (i=0; i<32; i++) {
		if (sign_r[i] != sign_r_out[i]) {
			printf("sign_r error: number %d, expect is %02X, actual is %02X\n", i, sign_r[i], sign_r_out[i]);
			flag =1 ;
		}
	}
	for (i=0; i<32; i++) {
		if (sign_s[i] != sign_s_out[i]) {
			printf("sign_s error: number %d, expect is %02X, actual is %02X\n", i, sign_s[i], sign_s_out[i]);
			flag =1 ;
		}
	}
	ret = sm2_verify(hash, pk, sign_r_out, sign_s_out, &group);
	if (ret != 0) {
		printf("verify error\n");
		flag = 1;
	}
	if (flag==0) {
		printf("Test pass\n");
	}
	return flag;
}

void digitrand(unsigned char *x, int len) {
	int i, j;
	static int seed = 1;
	if (seed) {
		srand((unsigned long)time(NULL));
		j = rand() & 0xFF;
		while (j--) {
			rand();
		}
		seed = 0;
	}
	for (i=0; i < len; i++) {
		x[i] = (rand() & 0xFF);
	}
}


void sm2_performance_test() {
	UINT8 g [64] = {
		0x32, 0xC4, 0xAE, 0x2C, 0x1F, 0x19, 0x81, 0x19,
		0x5F, 0x99, 0x04, 0x46, 0x6A, 0x39, 0xC9, 0x94,
		0x8F, 0xE3, 0x0B, 0xBF, 0xF2, 0x66, 0x0B, 0xE1,
		0x71, 0x5A, 0x45, 0x89, 0x33, 0x4C, 0x74, 0xC7,
		0xBC, 0x37, 0x36, 0xA2, 0xF4, 0xF6, 0x77, 0x9C,
		0x59, 0xBD, 0xCE, 0xE3, 0x6B, 0x69, 0x21, 0x53,
		0xD0, 0xA9, 0x87, 0x7C, 0xC6, 0x2A, 0x47, 0x40,
		0x02, 0xDF, 0x32, 0xE5, 0x21, 0x39, 0xF0, 0xA0,
	};
	UINT8 p [32] = {
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};
	UINT8 n [32] = {
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B,
		0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x23,
	};
	UINT8 sk [32] = {
		0x12, 0x8B, 0x2F, 0xA8, 0xBD, 0x43, 0x3C, 0x6C,
		0x06, 0x8C, 0x8D, 0x80, 0x3D, 0xFF, 0x79, 0x79,
		0x2A, 0x51, 0x9A, 0x55, 0x17, 0x1B, 0x1B, 0x65,
		0x0C, 0x23, 0x66, 0x1D, 0x15, 0x89, 0x72, 0x63,
	};
	UINT8 pk [64] = {
		0xD5, 0x54, 0x8C, 0x78, 0x25, 0xCB, 0xB5, 0x61,
		0x50, 0xA3, 0x50, 0x6C, 0xD5, 0x74, 0x64, 0xAF,
		0x8A, 0x1A, 0xE0, 0x51, 0x9D, 0xFA, 0xF3, 0xC5,
		0x82, 0x21, 0xDC, 0x81, 0x0C, 0xAF, 0x28, 0xDD,
		0x92, 0x10, 0x73, 0x76, 0x8F, 0xE3, 0xD5, 0x9C,
		0xE5, 0x4E, 0x79, 0xA4, 0x94, 0x45, 0xCF, 0x73,
		0xFE, 0xD2, 0x30, 0x86, 0x53, 0x70, 0x27, 0x26,
		0x4D, 0x16, 0x89, 0x46, 0xD4, 0x79, 0x53, 0x3E,
	};
	UINT8 hash [32] = {
		0xB5, 0x24, 0xF5, 0x52, 0xCD, 0x82, 0xB8, 0xB0,
		0x28, 0x47, 0x6E, 0x00, 0x5C, 0x37, 0x7F, 0xB1,
		0x9A, 0x87, 0xE6, 0xFC, 0x68, 0x2D, 0x48, 0xBB,
		0x5D, 0x42, 0xE3, 0xD9, 0xB9, 0xEF, 0xFE, 0x76,
	};
	UINT8 random [32] = {
		0x6C, 0xB2, 0x8D, 0x99, 0x38, 0x5C, 0x17, 0x5C,
		0x94, 0xF9, 0x4E, 0x93, 0x48, 0x17, 0x66, 0x3F,
		0xC1, 0x76, 0xD9, 0x25, 0xDD, 0x72, 0xB7, 0x27,
		0x26, 0x0D, 0xBA, 0xAE, 0x1F, 0xB2, 0xF9, 0x6F,
	};

	UINT8 sign_r [32] = {
		0xAB, 0xCB, 0x7C, 0xFF, 0x24, 0xC7, 0x8E, 0x7B,
		0xE2, 0x64, 0x67, 0x3D, 0xE0, 0x6D, 0xC1, 0x21,
		0x9E, 0x47, 0xB6, 0x55, 0x1D, 0xC9, 0xC4, 0xDD,
		0x5F, 0xDA, 0x04, 0x80, 0x75, 0x95, 0x47, 0x4C,
	};
	UINT8 sign_s [32] = {
		0x14, 0x5C, 0x15, 0xA7, 0x7D, 0x6A, 0x5B, 0x69,
		0x2F, 0xF0, 0x7B, 0x34, 0xCC, 0x8C, 0xA1, 0x81,
		0xDD, 0x23, 0x08, 0x44, 0xF8, 0xCC, 0x22, 0xD2,
		0x66, 0x1C, 0x60, 0x5E, 0x59, 0x51, 0x9D, 0x53,
	};
	ECGROUP group;
	int i, j, k;
	int ret;
	int flag = 0;
#ifdef LINUX_PERFORMANCE
	struct timeval start;
	struct timeval end;
	double d1;
#endif

	group_set_p(&group, p);
	group_set_n(&group, n);
	group_set_g(&group, g);

	for (j=0; j<2; j++) {
		BIGINT big_sk [BIGINT_LEN];
		AFFPOINT point_pk;

		digitrand(sk, 32);
		digitrand(hash, 32);
		digitrand(random, 32);

		char2bigint(big_sk, sk);

		basepoint_mul(&point_pk, big_sk, &group);
		affpoint2char(pk, &point_pk);

#ifdef LINUX_PERFORMANCE
		gettimeofday(&start, NULL);
#endif
		for (k=0; k<1000; k++) {
			sm2_sign(hash, random, sk, sign_r, sign_s, &group);
		}
#ifdef LINUX_PERFORMANCE
		gettimeofday(&end, NULL);
		d1 = end.tv_sec*1000.0 - start.tv_sec*1000.0 + (end.tv_usec - start.tv_usec)/1000.0;
		fprintf(stdout,"--- total time is %f ms \n", d1);
		fprintf(stdout,"--- one time is %f ms \n", d1/1000);

		gettimeofday(&start, NULL);
#endif
		for (k=0; k<1000; k++) {
			ret = sm2_verify(hash, pk, sign_r, sign_s, &group);
			if (ret != 0) {
				printf("verify error\n");
			}
		}
#ifdef LINUX_PERFORMANCE
		gettimeofday(&end, NULL);
		d1 = end.tv_sec*1000.0 - start.tv_sec*1000.0 + (end.tv_usec - start.tv_usec)/1000.0;
		fprintf(stdout,"--- total time is %f ms \n", d1);
		fprintf(stdout,"--- one time is %f ms \n", d1/1000);
#endif
	}
}

void* sm2_1000times_test(void* arg) {
	UINT8 g [64] = {
		0x32, 0xC4, 0xAE, 0x2C, 0x1F, 0x19, 0x81, 0x19,
		0x5F, 0x99, 0x04, 0x46, 0x6A, 0x39, 0xC9, 0x94,
		0x8F, 0xE3, 0x0B, 0xBF, 0xF2, 0x66, 0x0B, 0xE1,
		0x71, 0x5A, 0x45, 0x89, 0x33, 0x4C, 0x74, 0xC7,
		0xBC, 0x37, 0x36, 0xA2, 0xF4, 0xF6, 0x77, 0x9C,
		0x59, 0xBD, 0xCE, 0xE3, 0x6B, 0x69, 0x21, 0x53,
		0xD0, 0xA9, 0x87, 0x7C, 0xC6, 0x2A, 0x47, 0x40,
		0x02, 0xDF, 0x32, 0xE5, 0x21, 0x39, 0xF0, 0xA0,
	};
	UINT8 p [32] = {
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	};
	UINT8 n [32] = {
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B,
		0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x23,
	};
	UINT8 sk [32];
	UINT8 pk [64];
	UINT8 hash [32];
	UINT8 random [32];

	BIGINT big_sk [BIGINT_LEN];
	AFFPOINT point_pk;
	UINT8 sign_r [32];
	UINT8 sign_s [32];
	ECGROUP group;

	int i, j, k;
	int ret;
	int flag = 0;

	//set the group info
	group_set_p(&group, p);
	group_set_n(&group, n);
	group_set_g(&group, g);

	//generate public key
	digitrand(sk, 32);
	char2bigint(big_sk, sk);
	basepoint_mul(&point_pk, big_sk, &group);
	affpoint2char(pk, &point_pk);

	for (j=0; j<1000; j++) {
		digitrand(hash, 32);
		digitrand(random, 32);
		sm2_sign(hash, random, sk, sign_r, sign_s, &group);
		ret = sm2_verify(hash, pk, sign_r, sign_s, &group);
		if (ret != 0) {
			printf("verify error\n");
		}
	}
	return ((void*)0);
}

void sm2_multi_thread_test(int n) {
	HANDLE* handle;
	int i;
    //用QueryPerformanceCounter()来计时  微秒
    LARGE_INTEGER  large_interger;
    double dff;
    __int64  c1, c2;

	handle = malloc(n*sizeof(pthread_t*));

    QueryPerformanceFrequency(&large_interger);
    dff = large_interger.QuadPart;
    QueryPerformanceCounter(&large_interger);
    c1 = large_interger.QuadPart;

	for (i=0; i<n; i++) {
		err = pthread_create(tid+i, NULL, sm2_1000times_test, NULL);
		if (err!=0) {
			fprintf(stdout, "create thread error. n=%d, error info:%s\n", n, strerror(err));
			exit(-1);
		}
	}
	for (i=0; i<n; i++) {
		err = pthread_join(*(tid+i), &tret);
		if (err!=0) {
			fprintf(stdout, "can't join with thread. n=%d, error info:%s\n", n, strerror(err));
			exit(-1);
		}
	}

    QueryPerformanceCounter(&large_interger);
    c2 = large_interger.QuadPart;
    printf("本机高精度计时器频率%lf\n", dff);
    printf("第一次计时器值%I64d 第二次计时器值%I64d 计时器差%I64d\n", c1, c2, c2 - c1);
    printf("计时%lf us\n", (c2 - c1) * 1000000 / dff);

	fprintf(stdout,"--- Test of %d thread \n", n);
	fprintf(stdout,"--- total time is %f us \n", d1);
	fprintf(stdout,"--- one (sign+verify) time is %f us \n", d1/(n*1000));

	gettimeofday(&end, NULL);

	d1 = end.tv_sec*1000000.0 - start.tv_sec*1000000.0 + (end.tv_usec - start.tv_usec);

	fprintf(stdout,"--- Test of %d thread \n", n);
	fprintf(stdout,"--- total time is %f us \n", d1);
	fprintf(stdout,"--- one (sign+verify) time is %f us \n", d1/(n*1000));
	fprintf(stdout,"%d,%f,%f,\n", n, 0.45*(d1/(n*1000)), 0.55*(d1/(n*1000)));

	return;
}

void sm2_test() {
	sm2_function_test();
#ifdef PERFORMANCE
	sm2_performance_test();
	int i;
	for (i=1; i<60; i++) {
		sm2_multi_thread_test(i);
	}
#endif
}

int main() {
#ifdef BIT32
	printf("This is the software of 32bit\n");
	printf("BIGINT is %ld bytes\n", sizeof(BIGINT));
#else
	printf("This is the software of 64bit\n");
	printf("BIGINT is %ld bytes\n", sizeof(BIGINT));
#endif
	sm2_test();
	return 0;
}
