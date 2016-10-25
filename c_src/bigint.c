
#include "buxus_type.h"
#include "bigint.h"

//frequently used macroes
#define LOHALF(x) ((BIGINT)((x) & 0xffff))
#define HIHALF(x) ((BIGINT)((x) >> 16 & 0xffff))
#define TOHIGH(x) ((BIGINT)((x) << 16))

BIGINT bigint_add (BIGINT* r, BIGINT* a, BIGINT* b) {
	BIGINT *ap;
	BIGINT *bp;
	BIGINT *rp;
	BIGINT t;
	BIGINT result;
	BIGINT carry = 0;
	int i = BIGINT_LEN;

	ap = a;
	bp = b;
	rp = r;

	while (i--) {
		t = *(ap++);
		t = t + carry;
		carry = (t < carry);
		result = *(bp++) + t;
		carry += (result < t);
		*(rp++) = result;
	}
	//for(i=0; i<BIGINT_LEN; i++) {
	//	BIGINT t1, t2;
	//	t1 = *(ap++);
	//	t2 = *(bp++);
	//	result = t1 + t2 + carry;
	//	if (result > t1) {
	//		carry = 0;
	//	}
	//	else if (result < t1) {
	//		carry = 1;
	//	}
	//	*(rp++) = result;
	//}

	return carry;
}

BIGINT bigint_sub (BIGINT* r, BIGINT* a, BIGINT* b) {
	BIGINT *ap;
	BIGINT *bp;
	BIGINT *rp;
	BIGINT t1;
	BIGINT t2;
	BIGINT result;
	BIGINT borrow = 0;
	int i = BIGINT_LEN;

	ap = a;
	bp = b;
	rp = r;

	while (i--) {
		t1 = *(ap++);
		t2 = *(bp++);
		result = t1 - t2 - borrow;
		if (t1 != t2) {
			borrow = (t1 < t2);
		}
		*(rp++) = result;
	}

	return borrow;
}

void bigint_mul(BIGINT* r, BIGINT* a, BIGINT* b) {
	BIGINT r0=0, r1=0, r2=0;
	BIGINT x0, x1, y0, y1;
	BIGINT s1, s2, s3, s4;
	int i, j, k;

#define mul_and_add(r2, r1, r0, x, y)			\
do{												\
	x0 = LOHALF(x); x1 = HIHALF(x);				\
	y0 = LOHALF(y); y1 = HIHALF(y);				\
	s1 = x0*y0+LOHALF(r0);						\
	s2 = x0*y1+HIHALF(r0);						\
	s3 = x1*y0+LOHALF(s2)+HIHALF(s1);			\
	s4 = x1*y1+HIHALF(s2)+HIHALF(s3);			\
	r0 = TOHIGH(s3)|LOHALF(s1);					\
	r1 += s4;									\
	r2 += (r1 < s4);							\
}while (0) /* add x*y to (r2, r1, r0) */


#define FAST
#ifndef FAST
	/* multiplication */
	for (k=0; k<=2*(BIGINT_LEN-1); k++) {
		i = (k-(BIGINT_LEN-1) <= 0) ? 0: (k-(BIGINT_LEN-1));
		j = (k <= BIGINT_LEN-1) ? k : (BIGINT_LEN-1);
	//	i = MAX(0, k-(BIGINT_LEN-1)); j=MIN(k, BIGINT_LEN-1);
		while (i <= j) {
			mul_and_add(r2, r1, r0, a[i], b[k-i]);
			i++;
		}
		r[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	r[2*BIGINT_LEN-1] = r0;
#else
// multiplication
	mul_and_add(r2, r1, r0, a[0], b[0]);
	r[0] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[1], b[0]);
	mul_and_add(r2, r1, r0, a[0], b[1]);
	r[1] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[2], b[0]);
	mul_and_add(r2, r1, r0, a[1], b[1]);
	mul_and_add(r2, r1, r0, a[0], b[2]);
	r[2] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[3], b[0]);
	mul_and_add(r2, r1, r0, a[2], b[1]);
	mul_and_add(r2, r1, r0, a[1], b[2]);
	mul_and_add(r2, r1, r0, a[0], b[3]);
	r[3] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[4], b[0]);
	mul_and_add(r2, r1, r0, a[3], b[1]);
	mul_and_add(r2, r1, r0, a[2], b[2]);
	mul_and_add(r2, r1, r0, a[1], b[3]);
	mul_and_add(r2, r1, r0, a[0], b[4]);
	r[4] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[5], b[0]);
	mul_and_add(r2, r1, r0, a[4], b[1]);
	mul_and_add(r2, r1, r0, a[3], b[2]);
	mul_and_add(r2, r1, r0, a[2], b[3]);
	mul_and_add(r2, r1, r0, a[1], b[4]);
	mul_and_add(r2, r1, r0, a[0], b[5]);
	r[5] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[6], b[0]);
	mul_and_add(r2, r1, r0, a[5], b[1]);
	mul_and_add(r2, r1, r0, a[4], b[2]);
	mul_and_add(r2, r1, r0, a[3], b[3]);
	mul_and_add(r2, r1, r0, a[2], b[4]);
	mul_and_add(r2, r1, r0, a[1], b[5]);
	mul_and_add(r2, r1, r0, a[0], b[6]);
	r[6] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[0]);
	mul_and_add(r2, r1, r0, a[6], b[1]);
	mul_and_add(r2, r1, r0, a[5], b[2]);
	mul_and_add(r2, r1, r0, a[4], b[3]);
	mul_and_add(r2, r1, r0, a[3], b[4]);
	mul_and_add(r2, r1, r0, a[2], b[5]);
	mul_and_add(r2, r1, r0, a[1], b[6]);
	mul_and_add(r2, r1, r0, a[0], b[7]);
	r[7] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[1]);
	mul_and_add(r2, r1, r0, a[6], b[2]);
	mul_and_add(r2, r1, r0, a[5], b[3]);
	mul_and_add(r2, r1, r0, a[4], b[4]);
	mul_and_add(r2, r1, r0, a[3], b[5]);
	mul_and_add(r2, r1, r0, a[2], b[6]);
	mul_and_add(r2, r1, r0, a[1], b[7]);
	r[8] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[2]);
	mul_and_add(r2, r1, r0, a[6], b[3]);
	mul_and_add(r2, r1, r0, a[5], b[4]);
	mul_and_add(r2, r1, r0, a[4], b[5]);
	mul_and_add(r2, r1, r0, a[3], b[6]);
	mul_and_add(r2, r1, r0, a[2], b[7]);
	r[9] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[3]);
	mul_and_add(r2, r1, r0, a[6], b[4]);
	mul_and_add(r2, r1, r0, a[5], b[5]);
	mul_and_add(r2, r1, r0, a[4], b[6]);
	mul_and_add(r2, r1, r0, a[3], b[7]);
	r[10] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[4]);
	mul_and_add(r2, r1, r0, a[6], b[5]);
	mul_and_add(r2, r1, r0, a[5], b[6]);
	mul_and_add(r2, r1, r0, a[4], b[7]);
	r[11] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[5]);
	mul_and_add(r2, r1, r0, a[6], b[6]);
	mul_and_add(r2, r1, r0, a[5], b[7]);
	r[12] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[6]);
	mul_and_add(r2, r1, r0, a[6], b[7]);
	r[13] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], b[7]);
	r[14] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	r[15] = r0;
#endif
#undef mul_and_add
}

void bigint_sqr(BIGINT* r, BIGINT* a) {
	BIGINT r0=0, r1=0, r2=0;
	BIGINT x0, x1, y0, y1;
	BIGINT s1, s2, s3, s4;
	int i, j, k;

// add x*y to (r2, r1, r0) */
#define mul_and_add(r2, r1, r0, x, y)			\
	x0 = LOHALF(x); x1 = HIHALF(x);				\
	y0 = LOHALF(y); y1 = HIHALF(y);				\
	s1 = x0*y0+LOHALF(r0);						\
	s2 = x0*y1+HIHALF(r0);						\
	s3 = x1*y0+LOHALF(s2)+HIHALF(s1);			\
	s4 = x1*y1+HIHALF(s2)+HIHALF(s3);			\
	r0 = TOHIGH(s3)|LOHALF(s1);					\
	r1 += s4;									\
	r2 += (r1 < s4);

// add 2*x*y to (r2, r1, r0) */
#define mul_and_doubleadd(r2, r1, r0, x, y)		\
	x0 = LOHALF(x); x1 = HIHALF(x);				\
	y0 = LOHALF(y); y1 = HIHALF(y);				\
	s1 = x0*y0;	s2 = x1*y0;						\
	s3 = x0*y1+HIHALF(s1)+LOHALF(s2);			\
	s4 = x1*y1+HIHALF(s3)+HIHALF(s2);			\
	s3 = TOHIGH(s3)|LOHALF(s1);					\
	r2 += (s4>>31);								\
	s4 = (s4<<1) | (s3>>31);					\
	s3 <<= 1;									\
	r0 += s3;									\
	r1 += (s4 + (r0<s3));						\
	r2 += ((r1<s4)||((r1==s4)&&(r0<s3)));

#define FAST
#ifndef FAST
	//bigint_mul(r, a, a);

	/* multiplication */
	for (k=0; k<=2*(BIGINT_LEN-1); k++) {
		i = (k-(BIGINT_LEN-1) <= 0) ? 0: (k-(BIGINT_LEN-1));
		j = (k <= BIGINT_LEN-1) ? k : (BIGINT_LEN-1);
	//	i = MAX(0, k-(BIGINT_LEN-1)); j=MIN(k, BIGINT_LEN-1);
		while (i <= j) {
			if (i == (k-i)) {
				mul_and_add(r2, r1, r0, a[i], a[i]);
			}
			else if (i<(k-i)) {
				mul_and_doubleadd(r2, r1, r0, a[i], a[k-i]);
			}
			i++;
		}
		r[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	r[2*BIGINT_LEN-1] = r0;

#else

// square */
	mul_and_add(r2, r1, r0, a[0], a[0]);
	r[0] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[1]);
	r[1] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[2]);
	mul_and_add(r2, r1, r0, a[1], a[1]);
	r[2] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[3]);
	mul_and_doubleadd(r2, r1, r0, a[1], a[2]);
	r[3] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[4]);
	mul_and_doubleadd(r2, r1, r0, a[1], a[3]);
	mul_and_add(r2, r1, r0, a[2], a[2]);
	r[4] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[5]);
	mul_and_doubleadd(r2, r1, r0, a[1], a[4]);
	mul_and_doubleadd(r2, r1, r0, a[2], a[3]);
	r[5] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[6]);
	mul_and_doubleadd(r2, r1, r0, a[1], a[5]);
	mul_and_doubleadd(r2, r1, r0, a[2], a[4]);
	mul_and_add(r2, r1, r0, a[3], a[3]);
	r[6] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[0], a[7]);
	mul_and_doubleadd(r2, r1, r0, a[1], a[6]);
	mul_and_doubleadd(r2, r1, r0, a[2], a[5]);
	mul_and_doubleadd(r2, r1, r0, a[3], a[4]);
	r[7] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[1], a[7]);
	mul_and_doubleadd(r2, r1, r0, a[2], a[6]);
	mul_and_doubleadd(r2, r1, r0, a[3], a[5]);
	mul_and_add(r2, r1, r0, a[4], a[4]);
	r[8] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[2], a[7]);
	mul_and_doubleadd(r2, r1, r0, a[3], a[6]);
	mul_and_doubleadd(r2, r1, r0, a[4], a[5]);
	r[9] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[3], a[7]);
	mul_and_doubleadd(r2, r1, r0, a[4], a[6]);
	mul_and_add(r2, r1, r0, a[5], a[5]);
	r[10] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[4], a[7]);
	mul_and_doubleadd(r2, r1, r0, a[5], a[6]);
	r[11] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[5], a[7]);
	mul_and_add(r2, r1, r0, a[6], a[6]);
	r[12] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, a[6], a[7]);
	r[13] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, a[7], a[7]);
	r[14] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	r[15] = r0;

#endif

#undef mul_and_add
#undef mul_and_doubleadd
}


void bigint_mod (BIGINT* r, BIGINT *p) {
	while (bigint_cmp(r, p) == 1) {
		bigint_sub(r, r, p);
	}
}

void bigint_sm2_modorder(BIGINT* r, BIGINT* a) {
	BIGINT order[BIGINT_LEN+1] = {
		0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
		0x00000000,
	};
	BIGINT mu[BIGINT_LEN+1] = {
		0xf15149a0, 0x12ac6361, 0xfa323c01, 0x8dfc2096,
		0x00000001, 0x00000001, 0x00000001, 0x00000001,
		0x00000001,
	};
	// 2^(32*(BIGINT_LEN*2))/order

	BIGINT r0, r1, r2;
	BIGINT x0, x1, y0, y1;
	BIGINT s1, s2, s3, s4;
	BIGINT *q1, q2[BIGINT_LEN+1], clone[BIGINT_LEN+2];
	BIGINT borrow, sum;
	int i, k, flag;

#define mul_and_add(r2, r1, r0, x, y)			\
do{												\
	x0 = LOHALF(x); x1 = HIHALF(x);				\
	y0 = LOHALF(y); y1 = HIHALF(y);				\
	s1 = x0*y0+LOHALF(r0);						\
	s2 = x0*y1+HIHALF(r0);						\
	s3 = x1*y0+LOHALF(s2)+HIHALF(s1);			\
	s4 = x1*y1+HIHALF(s2)+HIHALF(s3);			\
	r0 = TOHIGH(s3)|LOHALF(s1);					\
	r1 += s4;									\
	r2 += (r1 < s4);							\
}while (0) /* add x*y to (r2, r1, r0) */

    /* multiplication(k=BIGINT_LEN) */
	r0 = 0;
	r1 = 0;
	r2 = 0;
	for (k=0; k<=BIGINT_LEN; k++) {
		for (i=k; i<=BIGINT_LEN;i++) {
			if (mu[i] != 0) {
				mul_and_add(r2, r1, r0, mu[i], a[2*BIGINT_LEN-1+k-i]);
			}
		}
		clone[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	clone[BIGINT_LEN+1] = r0;
	q1 = clone + 1; // q1 = ((x/b^(k-1))*mu)/b^(k+1)

	r0 = 0;
	r1 = 0;
	r2 = 0;
	for (k=0; k<BIGINT_LEN+1; k++) {
		for (i=0; i<=k; i++) {
			mul_and_add(r2, r1, r0, q1[i], order[k-i]);
		}
		q2[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}// q2 = q1*order mod b^(k+1)

	borrow = 0;
	for (i=0; i<BIGINT_LEN+1; i++)
	{
		sum = a[i] - q2[i] - borrow;
		if (a[i] < q2[i])
		{
			borrow = 1;
		}
		else if (a[i] > q2[i])
		{
			borrow = 0;
		}
		q1[i] = sum;
	}// q1 = x- q2


	flag = 0;
	i = BIGINT_LEN + 1;
	while (i--)
	{
		if (q1[i] > order[i])
		{
			flag = 1;
			break;
		}
		else if (q1[i] < order[i])
		{
			flag = -1;
			break;
		}
	}// q1 >= order?
	while (flag>=0)
	{
		borrow = 0;
		for (i=0; i<BIGINT_LEN+1; i++)
		{
			sum = q1[i] - order[i] - borrow;
			if (q1[i] < order[i])
			{
				borrow = 1;
			}
			else if (q1[i] > order[i])
			{
				borrow = 0;
			}
			q1[i] = sum;
		}

		flag = 0;
		i = BIGINT_LEN + 1;
		while (i--)
		{
			if (q1[i] > order[i])
			{
				flag = 1;
				break;
			}
			else if (q1[i] < order[i])
			{
				flag = -1;
				break;
			}
		}// q1 >= order?
	}
	for (i=0; i<BIGINT_LEN; i++)
	{
		r[i] = q1[i];  // r = q1
	}

#undef mul_and_add
}

//u = v mod P(2^256-2^224-2^96+2^64-1)
void bigint_sm2_mod(BIGINT* r, BIGINT* a) {
	BIGINT w[BIGINT_LEN], carry, borrow, sum;
	BIGINT p[BIGINT_LEN] = {
		0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
	};

	int i, flag;

#define add_to_carry(carry, w, a)	\
do {								\
	w += a;	if (w < a) {carry++;}   \
} while (0)

#define sub_to_borrow(borrow, u, a)	\
do {								\
	if (u < a) {borrow++;}			\
	u -= a;							\
} while (0)

	w[0] = 0;
	carry = 0;
	add_to_carry(carry, w[0], a[0]);
	add_to_carry(carry, w[0], a[8]);
	add_to_carry(carry, w[0], a[9]);
	add_to_carry(carry, w[0], a[10]);
	add_to_carry(carry, w[0], a[11]);
	add_to_carry(carry, w[0], a[12]);
	add_to_carry(carry, w[0], a[13]);
	add_to_carry(carry, w[0], a[13]);
	add_to_carry(carry, w[0], a[14]);
	add_to_carry(carry, w[0], a[14]);
	add_to_carry(carry, w[0], a[15]);
	add_to_carry(carry, w[0], a[15]);

	w[1] = carry;
	carry = 0;
	add_to_carry(carry, w[1], a[1]);
	add_to_carry(carry, w[1], a[9]);
	add_to_carry(carry, w[1], a[10]);
	add_to_carry(carry, w[1], a[11]);
	add_to_carry(carry, w[1], a[12]);
	add_to_carry(carry, w[1], a[13]);
	add_to_carry(carry, w[1], a[14]);
	add_to_carry(carry, w[1], a[14]);
	add_to_carry(carry, w[1], a[15]);
	add_to_carry(carry, w[1], a[15]);

	w[2] = carry;
	carry = 0;
	add_to_carry(carry, w[2], a[2]);

	w[3] = carry;
	carry = 0;
	add_to_carry(carry, w[3], a[3]);
	add_to_carry(carry, w[3], a[8]);
	add_to_carry(carry, w[3], a[11]);
	add_to_carry(carry, w[3], a[12]);
	add_to_carry(carry, w[3], a[13]);
	add_to_carry(carry, w[3], a[13]);
	add_to_carry(carry, w[3], a[14]);
	add_to_carry(carry, w[3], a[15]);

	w[4] = carry;
	carry = 0;
	add_to_carry(carry, w[4], a[4]);
	add_to_carry(carry, w[4], a[9]);
	add_to_carry(carry, w[4], a[12]);
	add_to_carry(carry, w[4], a[13]);
	add_to_carry(carry, w[4], a[14]);
	add_to_carry(carry, w[4], a[14]);
	add_to_carry(carry, w[4], a[15]);

	w[5] = carry;
	carry = 0;
	add_to_carry(carry, w[5], a[5]);
	add_to_carry(carry, w[5], a[10]);
	add_to_carry(carry, w[5], a[13]);
	add_to_carry(carry, w[5], a[14]);
	add_to_carry(carry, w[5], a[15]);
	add_to_carry(carry, w[5], a[15]);

	w[6] = carry;
	carry = 0;
	add_to_carry(carry, w[6], a[6]);
	add_to_carry(carry, w[6], a[11]);
	add_to_carry(carry, w[6], a[14]);
	add_to_carry(carry, w[6], a[15]);

	w[7] = carry;
	carry = 0;
	add_to_carry(carry, w[7], a[7]);
	add_to_carry(carry, w[7], a[8]);
	add_to_carry(carry, w[7], a[9]);
	add_to_carry(carry, w[7], a[10]);
	add_to_carry(carry, w[7], a[11]);
	add_to_carry(carry, w[7], a[12]);
	add_to_carry(carry, w[7], a[12]);
	add_to_carry(carry, w[7], a[13]);
	add_to_carry(carry, w[7], a[13]);
	add_to_carry(carry, w[7], a[14]);
	add_to_carry(carry, w[7], a[14]);
	add_to_carry(carry, w[7], a[15]);
	add_to_carry(carry, w[7], a[15]);
	add_to_carry(carry, w[7], a[15]);

	r[0] = w[0];
	borrow = 0;

	r[1] = w[1] - borrow;
	borrow = 0;
	if (r[1] > w[1]) {
		borrow++;
	}

	r[2] = w[2] - borrow;
	borrow = 0;
	if (r[2] > w[2]) {
		borrow++;
	}
	sub_to_borrow(borrow, r[2], a[8]);
	sub_to_borrow(borrow, r[2], a[9]);
	sub_to_borrow(borrow, r[2], a[13]);
	sub_to_borrow(borrow, r[2], a[14]);

	r[3] = w[3] - borrow;
	borrow = 0;
	if (r[3] > w[3]) {
		borrow++;
	}

	r[4] = w[4] - borrow;
	borrow = 0;
	if (r[4] > w[4]) {
		borrow++;
	}

	r[5] = w[5] - borrow;
	borrow = 0;
	if (r[5] > w[5]) {
		borrow++;
	}

	r[6] = w[6] - borrow;
	borrow = 0;
	if (r[6] > w[6]) {
		borrow++;
	}

	r[7] = w[7] - borrow;
	borrow = 0;
	if (r[7] > w[7]) {
		borrow++;
	}


	if (carry >= borrow) {
		carry -= borrow;
		while (carry) {
			carry -= (bigint_sub(r, r, p));
		}
		bigint_mod(r, p);
	}
	else {
		borrow -= carry;
		while (borrow) {
			borrow -= (bigint_add(r, r, p));
		}// r = r + P
	}

#undef add_to_carry
#undef sub_to_borrow

}

void bigint_rshift (BIGINT* r, BIGINT* a) {
	int i = BIGINT_LEN-1;
	BIGINT *ap;
	BIGINT *rp;
	BIGINT d1;
	BIGINT d2;
	ap = a;
	rp = r;
	d2 = *(ap++);
	while (i--) {
		d1 = d2;
		d2 = *(ap++);
		*(rp++) = (d1>>1) | (d2<<31);
	}
	*rp = d2>>1;
}

BIGINT bigint_lshift (BIGINT* r, BIGINT* a) {
	int i = BIGINT_LEN-1;
	BIGINT *ap;
	BIGINT *rp;
	BIGINT d1;
	BIGINT d2;
	ap = a;
	rp = r;
	d2 = *(ap++);
	*(rp++) = d2<<1;
	while (i--) {
		d1 = d2;
		d2 = *(ap++);
		*(rp++) = (d1>>31) | (d2<<1);
	}
	return (d2>>31);
}

void bigint_mod_add (BIGINT* r, BIGINT* a, BIGINT* b, BIGINT *p) {
	if (bigint_add(r, a, b)) {
		while (!bigint_sub(r, r, p)) {};
	}
	bigint_mod(r, p);
}

void bigint_mod_sub (BIGINT* r, BIGINT* a, BIGINT* b, BIGINT *p) {
	if (bigint_sub(r, a, b)) {
		while (!bigint_add(r, r, p)) {};
	}
	bigint_mod(r, p);
}

void bigint_sm2_mod_mul(BIGINT* r, BIGINT* a, BIGINT* b) {
	BIGINT clone[2*BIGINT_LEN];

	bigint_mul(clone, a, b);
	bigint_sm2_mod(r, clone);
}
void bigint_sm2_modorder_mul(BIGINT* r, BIGINT* a, BIGINT* b) {
	BIGINT clone[2*BIGINT_LEN];

	bigint_mul(clone, a, b);
	bigint_sm2_modorder(r, clone);
}

void bigint_mod_mul(BIGINT* r, BIGINT* a, BIGINT* b, BIGINT* p) {
	bigint_sm2_mod_mul(r, a, b);
}
void bigint_modorder_mul(BIGINT* r, BIGINT* a, BIGINT* b, BIGINT* p) {
	bigint_sm2_modorder_mul(r, a, b);
}


void bigint_sm2_mod_sqr(BIGINT* r, BIGINT* a) {
	BIGINT clone[2*BIGINT_LEN];

	bigint_sqr(clone, a);
	bigint_sm2_mod(r, clone);
}
void bigint_sm2_modorder_sqr(BIGINT* r, BIGINT* a) {
	BIGINT clone[2*BIGINT_LEN];

	bigint_sqr(clone, a);
	bigint_sm2_modorder(r, clone);
}
void bigint_mod_sqr(BIGINT* r, BIGINT* a, BIGINT* p) {
	bigint_sm2_mod_sqr(r, a);
}
void bigint_modorder_sqr(BIGINT* r, BIGINT* a, BIGINT* p) {
	bigint_sm2_modorder_sqr(r, a);
}

void bigint_mod_inv(BIGINT* r, BIGINT* a, BIGINT* p)
{
	BIGINT bu[BIGINT_LEN], bv[BIGINT_LEN], br[BIGINT_LEN]={1}, bs[BIGINT_LEN]={0};
	BIGINT sum, carry;
	int i, flagu, flag;

	bigint_copy(bu, a);
	bigint_copy(bv, p);

	if (bigint_is_zero(bu)) {
		bigint_set_zero(r);
		return;
	}

	while (!bigint_is_zero(bu)) {
		while ((bu[0] & 1) == 0) {
			bigint_rshift(bu, bu);

			if ((br[0] & 1) == 0) {
				bigint_rshift(br, br);
			}
			else {
				bigint_mod_rshift(br, br, p);
			}// a=(a+P)>>1
		}
		while ((bv[0] & 1) == 0) {
			bigint_rshift(bv, bv);

			if ((bs[0] & 1) == 0) {
				bigint_rshift(bs, bs);
			}
			else {
				bigint_mod_rshift(bs, bs, p);
			}// c=(c+P)>>1
		}

		if (bigint_cmp(bu, bv) >= 0) {
			bigint_mod_sub(bu, bu, bv, p);
			bigint_mod_sub(br, br, bs, p);
		}
		else {
			bigint_mod_sub(bv, bv, bu, p);
			bigint_mod_sub(bs, bs, br, p);
		}
	}

	bigint_copy(r, bs);
	return;
}


void bigint_mod_rshift (BIGINT* r, BIGINT* a, BIGINT* p) {
	int i = BIGINT_LEN-1;
	BIGINT *ap;
	BIGINT *rp;
	BIGINT d1;
	BIGINT d2;
	BIGINT carry;
	BIGINT tmp[BIGINT_LEN];

	carry = bigint_add(tmp, a, p);

	ap = tmp;
	rp = r;
	d2 = *(ap++);
	while (i--) {
		d1 = d2;
		d2 = *(ap++);
		*(rp++) = (d1>>1) | (d2<<31);
	}
	*rp = (d2>>1) | (carry<<31);
}

void bigint_mod_lshift (BIGINT* r, BIGINT* a, BIGINT* p) {
	if (bigint_lshift(r, a)) {
		while (!bigint_sub(r, r, p)) {};
	}
	bigint_mod(r, p);
}

void bigint_set_zero (BIGINT *a) {
	int i = BIGINT_LEN;
	BIGINT *ap;
	ap = a;
	while (i--) {
		*(ap++)=0;
	}
	return;
}

void bigint_set_one (BIGINT *a) {
	int i = BIGINT_LEN-1;
	BIGINT *ap;
	ap = a;
	*(ap++) = 0x01;
	while (i--) {
		*(ap++)=0;
	}
	return;
}


int bigint_cmp (BIGINT *a, BIGINT *b) {
	int i = BIGINT_LEN;
	BIGINT *ap;
	BIGINT *bp;
	BIGINT ad;
	BIGINT bd;
	ap = a+BIGINT_LEN-1;
	bp = b+BIGINT_LEN-1;
	while (i--) {
		ad = *ap;
		bd = *bp;
		if (ad > bd) {
			return 1;
		}
		else if (ad < bd) {
			return -1;
		}
		ap--;
		bp--;
	}
	return 0;
}

int bigint_is_zero (BIGINT *a) {
	int i = BIGINT_LEN;
	BIGINT *ap;
	ap = a;
	while (i--) {
		if (*(ap++)!=0) {
			return 0;
		}
	}
	return 1;
}

int bigint_is_equal (BIGINT *a, BIGINT *b) {
	int i = BIGINT_LEN;
	BIGINT *ap;
	BIGINT *bp;
	ap = a;
	bp = b;
	while (i--) {
		if (*(ap++) != *(bp++)) {
			return 0;
		}
	}
	return 1;
}


void char2bigint (BIGINT *r, UINT8* c) {
	UINT8 *cp;
	BIGINT *rp;
	int i = BIGINT_LEN;
	cp = c + 31;
	rp = r;

#ifdef BIT32
	while (i--) {
		BIGINT t1, t2, t3, t4;
		t1 = ((*(cp--)) & 0xff);
		t2 = ((*(cp--)<<8) & 0xff00);
		t3 = ((*(cp--)<<16) & 0xff0000);
		t4 = ((*(cp--)<<24) & 0xff000000);
		*(rp++) = t1 | t2 | t3 | t4;
	}
#else
#endif
}

void bigint2char (UINT8* r, BIGINT* a) {
	UINT8 *rp;
	BIGINT d;
	int i = BIGINT_LEN;

	rp = r;
#ifdef BIT32
	while (i--) {
		d = a[i];
		*(rp++) = (d>>24) & 0xff;
		*(rp++) = (d>>16) & 0xff;
		*(rp++) = (d>>8) & 0xff;
		*(rp++) = (d) & 0xff;
	}
#else
#endif
}

void bigint_copy (BIGINT* r, BIGINT* a) {
	int i = BIGINT_LEN;
	BIGINT *ap;
	BIGINT *rp;
	ap = a;
	rp = r;
	while (i--) {
		*(rp++) = *(ap++);
	}
}

void bigint_rshift5 (BIGINT* r, BIGINT* a) {
	int i = BIGINT_LEN-1;
	BIGINT *ap;
	BIGINT *rp;
	BIGINT d1;
	BIGINT d2;
	ap = a;
	rp = r;
	d2 = *(ap++);
	while (i--) {
		d1 = d2;
		d2 = *(ap++);
		*(rp++) = (d1>>5) | (d2<<27);
	}
	*rp = d2>>5;
}

int bigint_compute_wnaf (int *naf, BIGINT *a) { //return is wNAF length

	BIGINT k [BIGINT_LEN];
	int i;
	int flag;
	int flag1 = 0;
	int carry;
	int *nafp;
	BIGINT *kp;

	bigint_copy(k, a);
	nafp = naf;

	i = 0;
	while (! flag1 ) {
		if ((k[0] & 1) == 0) {
		//if (k[0] % 2 == 0) {
			*(nafp++) = 0;
			bigint_rshift(k, k);
			i++;
		}
		else {
			flag = k[0] & 0x1f; // w[0]^0x1f is the last 5 bit of k

			bigint_rshift5(k, k);

			flag1 = bigint_is_zero(k);

			if (flag < 16) {
				*(nafp++) = flag;
				i++;
				if (! flag1) {
					*(nafp++) = 0;
					*(nafp++) = 0;
					*(nafp++) = 0;
					*(nafp++) = 0;
					i+=4;
				}
			}
			else {
				*(nafp++) = flag - 32;

				// k++
				kp = k;
				while (1) {
					(*kp)++;
					if ((*kp) != 0) {
						break;
					}
					kp++;
				}
				flag1 = 0;
				*(nafp++) = 0;
				*(nafp++) = 0;
				*(nafp++) = 0;
				*(nafp++) = 0;
				i+=5;
			}
		}
	}// end while (! flag1)
	while (1) {
		if (naf[i-1] == 0) {
			i--;
		}
		else {
			break;
		}
	}// the length of naf, finish NAF
	return i;
}

