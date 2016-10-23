#ifndef _BIGINT_H_
#define _BIGINT_H_

#ifdef __cplusplus
extern "C" {
#endif

BIGINT bigint_add (BIGINT* r, BIGINT* a, BIGINT* b);
BIGINT bigint_sub (BIGINT* r, BIGINT* a, BIGINT* b);
void bigint_mul(BIGINT* r, BIGINT* a, BIGINT* b);
void bigint_sqr(BIGINT* r, BIGINT* a);
void bigint_mod (BIGINT* r, BIGINT *p);
void bigint_mod_add (BIGINT* r, BIGINT* a, BIGINT* b, BIGINT *p);
void bigint_mod_sub (BIGINT* r, BIGINT* a, BIGINT* b, BIGINT *p);
void bigint_mod_mul (BIGINT* r, BIGINT* a, BIGINT* b, BIGINT *p);
void bigint_modorder_mul (BIGINT* r, BIGINT* a, BIGINT* b, BIGINT *p);
void bigint_mod_sqr (BIGINT* r, BIGINT* a, BIGINT *p);
void bigint_modorder_sqr (BIGINT* r, BIGINT* a, BIGINT *p);
void bigint_mod_inv(BIGINT* r, BIGINT* a, BIGINT* p);
void bigint_rshift (BIGINT* r, BIGINT* a);
BIGINT bigint_lshift (BIGINT* r, BIGINT* a);
void bigint_mod_rshift (BIGINT* r, BIGINT* a, BIGINT* p);
void bigint_mod_lshift (BIGINT* r, BIGINT* a, BIGINT* p);
void bigint_set_zero (BIGINT *a);
void bigint_set_one (BIGINT *a);
int bigint_cmp (BIGINT *a, BIGINT *b);
int bigint_is_zero (BIGINT *a);
int bigint_is_equal (BIGINT *a, BIGINT *b);
void char2bigint (BIGINT *r, UINT8* c);
void bigint2char (UINT8* r, BIGINT* a);
void bigint_copy (BIGINT* r, BIGINT* a);

void bigint_sm2_mod (BIGINT* r, BIGINT *a);
void bigint_sm2_modorder (BIGINT* r, BIGINT *a);
void bigint_sm2_mod_mul (BIGINT* r, BIGINT* a, BIGINT* b);
void bigint_sm2_modorder_mul (BIGINT* r, BIGINT* a, BIGINT* b);
void bigint_sm2_mod_sqr (BIGINT* r, BIGINT* a);
void bigint_sm2_modorder_sqr (BIGINT* r, BIGINT* a);
#ifdef __cplusplus
}
#endif

#endif
