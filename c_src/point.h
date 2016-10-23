#ifndef _POINT_H_
#define _POINT_H_

#ifdef __cplusplus
extern "C" {
#endif

void char2affpoint( AFFPOINT* r, UINT8* c);
void affpoint2char( UINT8* r, AFFPOINT* a);
void aff_set_infinity(AFFPOINT *r);
void aff_point_double(AFFPOINT* r, AFFPOINT* p, ECGROUP* group);
void aff_point_add(AFFPOINT* r, AFFPOINT* p, AFFPOINT* q, ECGROUP* group);
void jacobian_set_infinity(JPOINT *r);
void jacobian_point_double(JPOINT* r, JPOINT* p, ECGROUP* group);
void jacobian_affine_point_add(JPOINT* r, JPOINT* p, AFFPOINT* q, ECGROUP* group);
void jacobian2affine(AFFPOINT* r, JPOINT* j, ECGROUP* group);
void aff_point_mul(AFFPOINT* r, AFFPOINT* p, BIGINT* k, ECGROUP* group);
void aff_point_mul2(AFFPOINT* r, AFFPOINT* p, BIGINT* k, ECGROUP* group);
void basepoint_mul(AFFPOINT* r, BIGINT *k, ECGROUP* group);
void point_mul_add(AFFPOINT* r, BIGINT *k, AFFPOINT* q, BIGINT *l, ECGROUP* group);
void group_set_p(ECGROUP *group, UINT8 *p);
void group_set_n(ECGROUP *group, UINT8 *n);
void group_set_g(ECGROUP *group, UINT8 *g);

#ifdef __cplusplus
}
#endif

#endif

