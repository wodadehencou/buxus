#include "buxus_type.h"
#include "bigint.h"
#include "point.h"
#include "sm2_algo.h"

//==============================================================
// SM2 sign a message
// Sign a message
int sm2_sign(UINT8* hash, UINT8* random, UINT8* sk, UINT8* sign_r, UINT8* sign_s, ECGROUP* group) {
	BIGINT big_k	[BIGINT_LEN];
	BIGINT big_sk	[BIGINT_LEN];
	BIGINT big_e	[BIGINT_LEN];
	BIGINT big_r	[BIGINT_LEN];
	BIGINT big_s	[BIGINT_LEN];
	BIGINT big_t1	[BIGINT_LEN] = {1};
	BIGINT big_t2	[BIGINT_LEN];
	AFFPOINT kg;

	char2bigint(big_k, random);
	char2bigint(big_sk, sk);
	char2bigint(big_e, hash);

	basepoint_mul(&kg, big_k, group);
	bigint_mod_add(big_r, big_e, kg.x, group->order);
	if (bigint_is_zero(big_r)) { //r=0
		return -1;
	}
	bigint_mod_add(big_s, big_r, big_k, group->order);
	if (bigint_is_zero(big_s)) { //r+k=n
		return -1;
	}

	//bigint_set_one(big_t1);
	bigint_mod_add(big_t2, big_t1, big_sk, group->order);
	bigint_mod_inv(big_t1, big_t2, group->order); //(1+dA)^-1
	bigint_modorder_mul(big_s, big_r, big_sk, group->order);
	bigint_mod_sub(big_t2, big_k, big_s, group->order); //k-r*dA
	bigint_modorder_mul(big_s, big_t1, big_t2, group->order);

	if (bigint_is_zero(big_s)) { //s=0
		return -1;
	}

	bigint2char(sign_r, big_r);
	bigint2char(sign_s, big_s);
	return 0;
}


//==============================================================
// SM2 verify a sign
int sm2_verify (UINT8* hash, UINT8* pk, UINT8* sign_r, UINT8* sign_s, ECGROUP* group) {
	BIGINT big_r[BIGINT_LEN];
	BIGINT big_s[BIGINT_LEN];
	BIGINT big_e[BIGINT_LEN];
	BIGINT big_t[BIGINT_LEN];
	AFFPOINT pa;
	AFFPOINT point;

	char2bigint(big_r, sign_r);
	if (bigint_is_zero(big_r)) {
		return -1;
	}
	if (bigint_cmp(big_r, group->order) == 1) {
		return -1;
	}

	char2bigint(big_s, sign_s);
	if (bigint_is_zero(big_s)) {
		return -1;
	}
	if (bigint_cmp(big_s, group->order) == 1) {
		return -1;
	}

	char2bigint(big_e, hash);

	bigint_mod_add(big_t, big_r, big_s, group->order);
	if (bigint_is_zero(big_t)) { //r+s=0
		return -1;
	}

	char2affpoint(&pa, pk);
	point_mul_add(&point, big_s, &pa, big_t, group); //sG+tPA

	bigint_mod_add(big_s, big_e, point.x, group->order);
	if (bigint_is_equal(big_r, big_s) == 1) {
		return 0;
	}
	return 1;
}

