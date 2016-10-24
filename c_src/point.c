#include "buxus_type.h"
#include "bigint.h"
#include "point.h"

void char2affpoint(AFFPOINT* r, UINT8* c) {
	char2bigint(r->x, c);
	char2bigint(r->y, c+32);
}

void affpoint2char (UINT8* r, AFFPOINT* a) {
	bigint2char(r, a->x);
	bigint2char(r+32, a->y);
}

void aff_set_infinity(AFFPOINT *r) {
	bigint_set_zero(r->x);
	bigint_set_zero(r->y);
}

void aff_point_double(AFFPOINT* r, AFFPOINT* p, ECGROUP* group) {

	BIGINT r1 [BIGINT_LEN];
	BIGINT r2 [BIGINT_LEN];
	BIGINT r3 [BIGINT_LEN];
	BIGINT x3 [BIGINT_LEN];
	BIGINT con1 [BIGINT_LEN] = {1};

	if (bigint_is_zero(p->x) && (bigint_is_zero(p->y))) {
		aff_set_infinity(r);
		return;
	}

//#ifdef SM2
	bigint_mod_sqr(r1, p->x, group->p);
	bigint_mod_sub(r1, r1, con1, group->p);
	bigint_mod_lshift(r2, r1, group->p);
	bigint_mod_add(r1, r2, r1, group->p);
//#else
//	bigint_mod_sqr(r1, p->x, group->p);
//	bigint_mod_lshift(r2, r1, group->p);
//	bigint_mod_add(r1, r2, r1, group->p);
//	bigint_mod_add(r1, r1, group->a, group->p);
//#endif

	bigint_mod_lshift(r2, p->y, group->p);
	bigint_mod_inv(r2, r2, group->p);
	bigint_mod_mul(r1, r1, r2, group->p); //lamada
	bigint_mod_sqr(r2, r1, group->p);
	bigint_mod_lshift(r3, p->x, group->p);
	bigint_mod_sub(x3, r2, r3, group->p);
	bigint_mod_sub(r3, p->x, x3, group->p);
	bigint_mod_mul(r2, r1, r3, group->p);
	bigint_copy(r->x, x3);
	bigint_mod_sub(r->y, r2, p->y, group->p);
	return;
}

void aff_point_add(AFFPOINT* r, AFFPOINT* p, AFFPOINT* q, ECGROUP* group) {

	BIGINT r1 [BIGINT_LEN];
	BIGINT r2 [BIGINT_LEN];
	BIGINT r3 [BIGINT_LEN];
	BIGINT x3 [BIGINT_LEN];

	if (bigint_is_zero(p->x) && (bigint_is_zero(p->y))) {
		bigint_copy(r->x, q->x);
		bigint_copy(r->y, q->y);
		return;
	}
	if (bigint_is_zero(q->x) && (bigint_is_zero(q->y))) {
		bigint_copy(r->x, p->x);
		bigint_copy(r->y, p->y);
		return;
	}
	if (bigint_is_equal(p->x, q->x)) {
		if (bigint_is_equal(p->y, q->y)) {
			aff_point_double(r, p, group);
			return;
		}
		else {
			aff_set_infinity(r);
			return;
		}
	}

	bigint_mod_sub(r1, q->x, p->x, group->p);
	bigint_mod_sub(r2, q->y, p->y, group->p);
	bigint_mod_inv(r1, r1, group->p);
	bigint_mod_mul(r1, r1, r2, group->p); //lamada
	bigint_mod_sqr(r2, r1, group->p);
	bigint_mod_add(r3, p->x, q->x, group->p);
	bigint_mod_sub(x3, r2, r3, group->p);
	bigint_mod_sub(r3, p->x, x3, group->p);
	bigint_mod_mul(r2, r1, r3, group->p);
	bigint_copy(r->x, x3);
	bigint_mod_sub(r->y, r2, p->y, group->p);
	return;
}

void aff_point_mul(AFFPOINT* r, AFFPOINT* p, BIGINT* k, ECGROUP* group) {
	int i=256;
	BIGINT local_k[BIGINT_LEN];

	if (bigint_is_zero(k) || (bigint_is_zero(p->x) && bigint_is_zero(p->y))) {
		aff_set_infinity(r);
		return;
	}

	aff_set_infinity(r);
	bigint_copy(local_k, k);
	while(i--) {
		aff_point_double(r, r, group);
		if ((local_k[BIGINT_LEN-1] & MSB_MASK) == MSB_MASK) {
			aff_point_add(r, r, p, group);
		}
		bigint_lshift(local_k, local_k);
	}
	return;
}

void jacobian_set_infinity(JPOINT *r) {
	bigint_set_one(r->x);
	bigint_set_one(r->y);
	bigint_set_zero(r->z);
}

void jacobian_point_double(JPOINT* r, JPOINT* p, ECGROUP* group) {

	BIGINT r1 [BIGINT_LEN];
	BIGINT r2 [BIGINT_LEN];
	BIGINT r3 [BIGINT_LEN];
	BIGINT r4 [BIGINT_LEN];
	BIGINT r5 [BIGINT_LEN];

	if (bigint_is_zero(p->y) || (bigint_is_zero(p->z))) {
		jacobian_set_infinity(r);
		return;
	}

//#ifdef SM2
	bigint_mod_sqr(r1, p->z, group->p); //z^2
	bigint_mod_sub(r2, p->x, r1, group->p); //x-z^2
	bigint_mod_add(r1, p->x, r1, group->p); //x+z^2
	bigint_mod_mul(r2, r1, r2, group->p); //x^2-z^4
	bigint_mod_lshift(r1, r2, group->p); //2(x^2-z^4)
	bigint_mod_add(r1, r1, r2, group->p); //3(x^2-z^4)=B
//#else
//	bigint_mod_sqr(r1, p->x, group->p);
//	bigint_mod_lshift(r2, r1, group->p);
//	bigint_mod_add(r1, r2, r1, group->p);
//	bigint_mod_add(r1, r1, group->a, group->p);
//#endif

	bigint_mod_mul(r2, p->y, p->z, group->p); //yz
	bigint_mod_lshift(r->z, r2, group->p); //2yz
	bigint_mod_sqr(r2, p->y, group->p); //y^2
	bigint_mod_mul(r3, p->x, r2, group->p); //xy^2
	bigint_mod_lshift(r3, r3, group->p); //2xy^2
	bigint_mod_lshift(r3, r3, group->p); //4xy^2=A
	bigint_mod_sqr(r4, r1, group->p); //B^2
	bigint_mod_lshift(r5, r3, group->p); //2A
	bigint_mod_sub(r->x, r4, r5, group->p); //-2A+B^2
	bigint_mod_sqr(r2, r2, group->p); //y^4
	bigint_mod_lshift(r2, r2, group->p); //2y^4
	bigint_mod_lshift(r2, r2, group->p); //4y^4
	bigint_mod_lshift(r2, r2, group->p); //8y^4
	bigint_mod_sub(r3, r3, r->x, group->p); //A-x3
	bigint_mod_mul(r3, r3, r1, group->p); //B(A-x3)
	bigint_mod_sub(r->y, r3, r2, group->p); //-8y^4+B(A-x3)
	return;
}


void jacobian_affine_point_add(JPOINT* r, JPOINT* p, AFFPOINT* q, ECGROUP* group) {

	BIGINT r1 [BIGINT_LEN]	;
	BIGINT r2 [BIGINT_LEN]	;
	BIGINT r3 [BIGINT_LEN]	;

	if (bigint_is_zero(p->z)) {
		bigint_copy(r->x, q->x);
		bigint_copy(r->y, q->y);
		bigint_set_one(r->z);
		return;
	}
	if (bigint_is_zero(q->x) && bigint_is_zero(q->y)) {
		bigint_copy(r->x, p->x);
		bigint_copy(r->y, p->y);
		bigint_copy(r->z, p->z);
		return;
	}

	bigint_mod_sqr(r1, p->z, group->p); //z1^2
	bigint_mod_mul(r2, r1, q->x, group->p); //x2*z1^2=A
	bigint_mod_mul(r1, r1, p->z, group->p); //z1^3
	bigint_mod_mul(r1, r1, q->y, group->p); //y2*z1^3=B
	bigint_mod_sub(r1, r1, p->y, group->p); //B-y1=D
	bigint_mod_sub(r2, r2, p->x, group->p); //A-x1=C
	if (bigint_is_zero(r1)) {
		if (bigint_is_zero(r2)) {
			jacobian_point_double(r, p ,group); //can be replace by 2A=J
			return;
		}
		else {
			jacobian_set_infinity(r);
		}
	}

	bigint_mod_mul(r->z, p->z, r2, group->p);
	bigint_mod_sqr(r3, r2, group->p); //C^2
	bigint_mod_mul(r2, r2, r3, group->p); //C^3
	bigint_mod_mul(r3, r3, p->x, group->p);
	bigint_mod_sqr(r->x, r1, group->p);
	bigint_mod_mul(r->y, r2, p->y, group->p);
	bigint_mod_sub(r2, r->x, r2, group->p);
	bigint_mod_lshift(r->x, r3, group->p);
	bigint_mod_sub(r->x, r2, r->x, group->p);
	bigint_mod_sub(r2, r3, r->x, group->p);
	bigint_mod_mul(r2, r1, r2, group->p);
	bigint_mod_sub(r->y, r2, r->y, group->p);
	return;
}

void jacobian2affine(AFFPOINT* r, JPOINT* j, ECGROUP* group) {
	BIGINT r1 [BIGINT_LEN];
	BIGINT r2 [BIGINT_LEN];

	if (bigint_is_zero(j->z)) {
		aff_set_infinity(r);
		return;
	}
	bigint_mod_inv(r2, j->z, group->p);
	bigint_mod_sqr(r1, r2, group->p); //z^-2
	bigint_mod_mul(r2, r1, r2, group->p);//z^-3
	bigint_mod_mul(r->x, r1, j->x, group->p);
	bigint_mod_mul(r->y, r2, j->y, group->p);
	return;
}

void aff_point_mul2(AFFPOINT* r, AFFPOINT* p, BIGINT* k, ECGROUP* group) {
	int i=256;
	JPOINT jr;
	BIGINT local_k[BIGINT_LEN];

	if (bigint_is_zero(k) || (bigint_is_zero(p->x) && bigint_is_zero(p->y))) {
		aff_set_infinity(r);
		return;
	}

	jacobian_set_infinity(&jr);
	bigint_copy(local_k, k);
	while(i--) {
		jacobian_point_double(&jr, &jr, group);
		if ((local_k[BIGINT_LEN-1] & MSB_MASK) == MSB_MASK) {
			jacobian_affine_point_add(&jr, &jr, p, group);
		}
		bigint_lshift(local_k, local_k);
	}

	jacobian2affine(r, &jr, group);
	return;
}

void basepoint_mul(AFFPOINT* r, BIGINT *k, ECGROUP* group) {
	aff_point_mul2(r, &(group->g), k, group);
}

void point_mul_add(AFFPOINT* r, BIGINT *k, AFFPOINT* q, BIGINT *l, ECGROUP* group) { //kG+lQ
	AFFPOINT kg;
	AFFPOINT lq;
	basepoint_mul(&kg, k, group);
	aff_point_mul(&lq, q, l, group);
	aff_point_add(r, &kg, &lq, group);
	return;
}

void group_set_p(ECGROUP *group, UINT8 *p) {
	char2bigint(group->p, p);
	return;
}

void group_set_n(ECGROUP *group, UINT8 *n) {
	char2bigint(group->order, n);
	return;
}

void group_set_g(ECGROUP *group, UINT8 *g) {
	char2affpoint(&(group->g), g);
	return;
}

