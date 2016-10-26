#include "sm2.h"

//frequently used macroes
#define LOHALF(x) ((small)((x) & 0xffff))
#define HIHALF(x) ((small)((x) >> 16 & 0xffff))
#define TOHIGH(x) ((small)((x) << 16))

//==============================================================
//operation in field GF(P)
//w = u+v mod P
void add(big w, big u, big v)
{
	int i, flag;
	small sum, carry, borrow;

    /* addition */
	carry = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		sum = u[i] + v[i] + carry;
		if (sum < u[i])
		{
			carry = 1;
		}
		else if (sum > u[i])
		{
			carry = 0;
		}
		w[i] = sum;
	}

    /* reduction */
	flag = 0;
	i = DIG_LEN;
	while (i--)
	{
		if (w[i] > P[i])
		{
			flag = 1;
			break;
		}
		else if (w[i] < P[i])
		{
			flag = -1;
			break;
		}
	}// w >= P?

	if (carry==1 || flag>=0)
	{
		borrow = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = w[i] - P[i] - borrow;
			if (w[i] < P[i])
			{
				borrow = 1;
			}
			else if (w[i] > P[i])
			{
				borrow = 0;
			}
			w[i] = sum;
		}
	}// w = w - P
}

//w = u-v mod P
void sub(big w, big u, big v)
{
	int i;
	small sum, carry, borrow;

    /* subtraction */
	borrow = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		sum = u[i] - v[i] - borrow;
		if (u[i] < v[i])
		{
			borrow = 1;
		}
		else if (u[i] > v[i])
		{
			borrow = 0;
		}
		w[i] = sum;
	}

    /* reduction */
	if (borrow)
	{
		carry = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = w[i] + P[i] + carry;
			if (sum < w[i])
			{
				carry = 1;
			}
			else if (sum > w[i])
			{
				carry = 0;
			}
			w[i] = sum;
		}
	}//w = w + P
}

//u = v mod P(2^256-2^224-2^96+2^64-1)
void mod(big u, big v)
{
	small w[DIG_LEN], carry, borrow, sum;
	int i, flag;

#define add_to_carry(carry, w, v)	\
do {					\
	w += v;	if (w < v) {carry++;}   \
} while (0)

#define sub_to_borrow(borrow, u, v)	\
do {					\
	if (u < v) {borrow++;} 		\
	u -= v;				\
} while (0)

	w[0] = 0;
	carry = 0;
	add_to_carry(carry, w[0], v[0]);
	add_to_carry(carry, w[0], v[8]);
	add_to_carry(carry, w[0], v[9]);
	add_to_carry(carry, w[0], v[10]);
	add_to_carry(carry, w[0], v[11]);
	add_to_carry(carry, w[0], v[12]);
	add_to_carry(carry, w[0], v[13]);
	add_to_carry(carry, w[0], v[13]);
	add_to_carry(carry, w[0], v[14]);
	add_to_carry(carry, w[0], v[14]);
	add_to_carry(carry, w[0], v[15]);
	add_to_carry(carry, w[0], v[15]);

	w[1] = carry;
	carry = 0;
	add_to_carry(carry, w[1], v[1]);
	add_to_carry(carry, w[1], v[9]);
	add_to_carry(carry, w[1], v[10]);
	add_to_carry(carry, w[1], v[11]);
	add_to_carry(carry, w[1], v[12]);
	add_to_carry(carry, w[1], v[13]);
	add_to_carry(carry, w[1], v[14]);
	add_to_carry(carry, w[1], v[14]);
	add_to_carry(carry, w[1], v[15]);
	add_to_carry(carry, w[1], v[15]);

	w[2] = carry;
	carry = 0;
	add_to_carry(carry, w[2], v[2]);

	w[3] = carry;
	carry = 0;
	add_to_carry(carry, w[3], v[3]);
	add_to_carry(carry, w[3], v[8]);
	add_to_carry(carry, w[3], v[11]);
	add_to_carry(carry, w[3], v[12]);
	add_to_carry(carry, w[3], v[13]);
	add_to_carry(carry, w[3], v[13]);
	add_to_carry(carry, w[3], v[14]);
	add_to_carry(carry, w[3], v[15]);

	w[4] = carry;
	carry = 0;
	add_to_carry(carry, w[4], v[4]);
	add_to_carry(carry, w[4], v[9]);
	add_to_carry(carry, w[4], v[12]);
	add_to_carry(carry, w[4], v[13]);
	add_to_carry(carry, w[4], v[14]);
	add_to_carry(carry, w[4], v[14]);
	add_to_carry(carry, w[4], v[15]);

	w[5] = carry;
	carry = 0;
	add_to_carry(carry, w[5], v[5]);
	add_to_carry(carry, w[5], v[10]);
	add_to_carry(carry, w[5], v[13]);
	add_to_carry(carry, w[5], v[14]);
	add_to_carry(carry, w[5], v[15]);
	add_to_carry(carry, w[5], v[15]);

	w[6] = carry;
	carry = 0;
	add_to_carry(carry, w[6], v[6]);
	add_to_carry(carry, w[6], v[11]);
	add_to_carry(carry, w[6], v[14]);
	add_to_carry(carry, w[6], v[15]);

	w[7] = carry;
	carry = 0;
	add_to_carry(carry, w[7], v[7]);
	add_to_carry(carry, w[7], v[8]);
	add_to_carry(carry, w[7], v[9]);
	add_to_carry(carry, w[7], v[10]);
	add_to_carry(carry, w[7], v[11]);
	add_to_carry(carry, w[7], v[12]);
	add_to_carry(carry, w[7], v[12]);
	add_to_carry(carry, w[7], v[13]);
	add_to_carry(carry, w[7], v[13]);
	add_to_carry(carry, w[7], v[14]);
	add_to_carry(carry, w[7], v[14]);
	add_to_carry(carry, w[7], v[15]);
	add_to_carry(carry, w[7], v[15]);
	add_to_carry(carry, w[7], v[15]);

	u[0] = w[0];
	borrow = 0;

	u[1] = w[1] - borrow;
	borrow = 0;
	if (u[1] > w[1])
	{
		borrow++;
	}

	u[2] = w[2] - borrow;
	borrow = 0;
	if (u[2] > w[2])
	{
		borrow++;
	}
	sub_to_borrow(borrow, u[2], v[8]);
	sub_to_borrow(borrow, u[2], v[9]);
	sub_to_borrow(borrow, u[2], v[13]);
	sub_to_borrow(borrow, u[2], v[14]);

	u[3] = w[3] - borrow;
	borrow = 0;
	if (u[3] > w[3])
	{
		borrow++;
	}

	u[4] = w[4] - borrow;
	borrow = 0;
	if (u[4] > w[4])
	{
		borrow++;
	}

	u[5] = w[5] - borrow;
	borrow = 0;
	if (u[5] > w[5])
	{
		borrow++;
	}

	u[6] = w[6] - borrow;
	borrow = 0;
	if (u[6] > w[6])
	{
		borrow++;
	}

	u[7] = w[7] - borrow;
	borrow = 0;
	if (u[7] > w[7])
	{
		borrow++;
	}


	if (carry >= borrow)
	{
		carry -= borrow;
		while (carry != 0)
		{
			borrow = 0;
			for (i=0; i<DIG_LEN; i++)
			{
				sum = u[i] - P[i] - borrow;
				if (u[i] < P[i])
				{
					borrow = 1;
				}
				else if (u[i] > P[i])
				{
					borrow = 0;
				}
				u[i] = sum;
			}
			carry = carry - borrow;
		}// u = u - P

		flag = 0;
		i = DIG_LEN;
		while (i--)
		{
			if (u[i] > P[i])
			{
				flag = 1;
				break;
			}
			else if (u[i] < P[i])
			{
				flag = -1;
				break;
			}
		}// u >= P?

		if (flag<0)
		{
			return;
		}
		else
		{
			borrow = 0;
			for (i=0; i<DIG_LEN; i++)
			{
				sum = u[i] - P[i] - borrow;
				if (u[i] < P[i])
				{
					borrow = 1;
				}
				else if (u[i] > P[i])
				{
					borrow = 0;
				}
				u[i] = sum;
			}
		}// u = u - P
	}
	else
	{
		borrow -= carry;
		while (borrow)
		{
			carry = 0;
			for (i=0; i<DIG_LEN; i++)
			{
				sum = u[i] + P[i] + carry;
				if (sum < u[i])
				{
					carry = 1;
				}
				else if (sum > u[i])
				{
					carry = 0;
				}
				u[i] = sum;
			}
			borrow = borrow - carry;
		}// u = u + P
	}

#undef add_to_carry
#undef sub_to_borrow

}

//w = u*v mod P
void mul(big w, big u, big v)
{
	small clone[2*DIG_LEN];
	small r0=0, r1=0, r2=0;
	small x0, x1, y0, y1;
	small s1, s2, s3, s4;

// add x*y to (r2, r1, r0)
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
}while (0)

// multiplication
	mul_and_add(r2, r1, r0, u[0], v[0]);
	clone[0] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[1], v[0]);
	mul_and_add(r2, r1, r0, u[0], v[1]);
	clone[1] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[2], v[0]);
	mul_and_add(r2, r1, r0, u[1], v[1]);
	mul_and_add(r2, r1, r0, u[0], v[2]);
	clone[2] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[3], v[0]);
	mul_and_add(r2, r1, r0, u[2], v[1]);
	mul_and_add(r2, r1, r0, u[1], v[2]);
	mul_and_add(r2, r1, r0, u[0], v[3]);
	clone[3] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[4], v[0]);
	mul_and_add(r2, r1, r0, u[3], v[1]);
	mul_and_add(r2, r1, r0, u[2], v[2]);
	mul_and_add(r2, r1, r0, u[1], v[3]);
	mul_and_add(r2, r1, r0, u[0], v[4]);
	clone[4] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[5], v[0]);
	mul_and_add(r2, r1, r0, u[4], v[1]);
	mul_and_add(r2, r1, r0, u[3], v[2]);
	mul_and_add(r2, r1, r0, u[2], v[3]);
	mul_and_add(r2, r1, r0, u[1], v[4]);
	mul_and_add(r2, r1, r0, u[0], v[5]);
	clone[5] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[6], v[0]);
	mul_and_add(r2, r1, r0, u[5], v[1]);
	mul_and_add(r2, r1, r0, u[4], v[2]);
	mul_and_add(r2, r1, r0, u[3], v[3]);
	mul_and_add(r2, r1, r0, u[2], v[4]);
	mul_and_add(r2, r1, r0, u[1], v[5]);
	mul_and_add(r2, r1, r0, u[0], v[6]);
	clone[6] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[0]);
	mul_and_add(r2, r1, r0, u[6], v[1]);
	mul_and_add(r2, r1, r0, u[5], v[2]);
	mul_and_add(r2, r1, r0, u[4], v[3]);
	mul_and_add(r2, r1, r0, u[3], v[4]);
	mul_and_add(r2, r1, r0, u[2], v[5]);
	mul_and_add(r2, r1, r0, u[1], v[6]);
	mul_and_add(r2, r1, r0, u[0], v[7]);
	clone[7] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[1]);
	mul_and_add(r2, r1, r0, u[6], v[2]);
	mul_and_add(r2, r1, r0, u[5], v[3]);
	mul_and_add(r2, r1, r0, u[4], v[4]);
	mul_and_add(r2, r1, r0, u[3], v[5]);
	mul_and_add(r2, r1, r0, u[2], v[6]);
	mul_and_add(r2, r1, r0, u[1], v[7]);
	clone[8] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[2]);
	mul_and_add(r2, r1, r0, u[6], v[3]);
	mul_and_add(r2, r1, r0, u[5], v[4]);
	mul_and_add(r2, r1, r0, u[4], v[5]);
	mul_and_add(r2, r1, r0, u[3], v[6]);
	mul_and_add(r2, r1, r0, u[2], v[7]);
	clone[9] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[3]);
	mul_and_add(r2, r1, r0, u[6], v[4]);
	mul_and_add(r2, r1, r0, u[5], v[5]);
	mul_and_add(r2, r1, r0, u[4], v[6]);
	mul_and_add(r2, r1, r0, u[3], v[7]);
	clone[10] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[4]);
	mul_and_add(r2, r1, r0, u[6], v[5]);
	mul_and_add(r2, r1, r0, u[5], v[6]);
	mul_and_add(r2, r1, r0, u[4], v[7]);
	clone[11] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[5]);
	mul_and_add(r2, r1, r0, u[6], v[6]);
	mul_and_add(r2, r1, r0, u[5], v[7]);
	clone[12] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[6]);
	mul_and_add(r2, r1, r0, u[6], v[7]);
	clone[13] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, u[7], v[7]);
	clone[14] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	clone[15] = r0;

#undef mul_and_add

// reduction
	mod(w, clone);
}

//x = y^2 mod P
void squ(big x, big y)
{
	small clone[2*DIG_LEN];
	small r0=0, r1=0, r2=0;
	small x0, x1, y0, y1;
	small s1, s2, s3, s4;

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

// square */
	mul_and_add(r2, r1, r0, y[0], y[0]);
	clone[0] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[1]);
	clone[1] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[2]);
	mul_and_add(r2, r1, r0, y[1], y[1]);
	clone[2] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[3]);
	mul_and_doubleadd(r2, r1, r0, y[1], y[2]);
	clone[3] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[4]);
	mul_and_doubleadd(r2, r1, r0, y[1], y[3]);
	mul_and_add(r2, r1, r0, y[2], y[2]);
	clone[4] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[5]);
	mul_and_doubleadd(r2, r1, r0, y[1], y[4]);
	mul_and_doubleadd(r2, r1, r0, y[2], y[3]);
	clone[5] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[6]);
	mul_and_doubleadd(r2, r1, r0, y[1], y[5]);
	mul_and_doubleadd(r2, r1, r0, y[2], y[4]);
	mul_and_add(r2, r1, r0, y[3], y[3]);
	clone[6] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[0], y[7]);
	mul_and_doubleadd(r2, r1, r0, y[1], y[6]);
	mul_and_doubleadd(r2, r1, r0, y[2], y[5]);
	mul_and_doubleadd(r2, r1, r0, y[3], y[4]);
	clone[7] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[1], y[7]);
	mul_and_doubleadd(r2, r1, r0, y[2], y[6]);
	mul_and_doubleadd(r2, r1, r0, y[3], y[5]);
	mul_and_add(r2, r1, r0, y[4], y[4]);
	clone[8] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[2], y[7]);
	mul_and_doubleadd(r2, r1, r0, y[3], y[6]);
	mul_and_doubleadd(r2, r1, r0, y[4], y[5]);
	clone[9] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[3], y[7]);
	mul_and_doubleadd(r2, r1, r0, y[4], y[6]);
	mul_and_add(r2, r1, r0, y[5], y[5]);
	clone[10] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[4], y[7]);
	mul_and_doubleadd(r2, r1, r0, y[5], y[6]);
	clone[11] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[5], y[7]);
	mul_and_add(r2, r1, r0, y[6], y[6]);
	clone[12] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_doubleadd(r2, r1, r0, y[6], y[7]);
	clone[13] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	mul_and_add(r2, r1, r0, y[7], y[7]);
	clone[14] = r0;
	r0 = r1;
	r1 = r2;
	r2 = 0;

	clone[15] = r0;

#undef mul_and_add
#undef mul_and_doubleadd

// reduction */
	mod(x, clone);
}

//x = y^(-1) mod P */
void inv(big x, big y)
{
	small u[DIG_LEN], v[DIG_LEN], a[DIG_LEN]={1}, c[DIG_LEN]={0};
	small sum, carry;
	int i, flagu, flag;

	for (i=0; i<DIG_LEN; i++)
	{
		u[i] = y[i];
		v[i] = P[i];
	}

	for (flagu=1,i=0; i<DIG_LEN; i++)
	{
		if (u[i] != 0)
		{
			flagu = 0;
			break;
		}  // u == 0 ?
	}

	while (!flagu)
	{
		while (u[0] % 2 == 0)
		{
			for (i=0; i<DIG_LEN-1; i++)
			{
				u[i] = (u[i] >> 1) | (u[i+1] << 31);
			}
			u[DIG_LEN-1] = u[DIG_LEN-1] >> 1;  //u=u>>1

			if (a[0] % 2 == 0)
			{
				for (i=0; i<DIG_LEN-1; i++)
				{
					a[i] = (a[i] >> 1)|(a[i+1] << 31);
				}
				a[DIG_LEN-1] = a[DIG_LEN-1] >> 1;  //a=a>>1
			}
			else
			{
				carry = 0;
				for (i=0; i<DIG_LEN; i++)
				{
					sum = a[i] + P[i] + carry;
					if (sum < a[i])
					{
						carry = 1;
					}
					else if (sum > a[i])
					{
						carry = 0;
					}
					a[i] = sum;
				}
				for (i=0; i<DIG_LEN-1; i++)
				{
					a[i] = (a[i] >> 1) | (a[i+1] << 31);
				}
				a[DIG_LEN-1] = (a[DIG_LEN-1] >> 1) | (carry << 31);
			} //a=(a+P)>>1
		}// end while (u[0] % 2 == 0)

		while (v[0] % 2 == 0)
		{
			for (i=0; i<DIG_LEN-1; i++)
			{
				v[i] = (v[i] >> 1) | (v[i+1] << 31);
			}
			v[DIG_LEN-1] = v[DIG_LEN-1] >> 1;  // v=v>>1

			if (c[0] % 2 == 0)
			{
				for (i=0; i<DIG_LEN-1; i++)
				{
					c[i] = (c[i] >> 1) | (c[i+1] << 31);
				}
				c[DIG_LEN-1] = c[DIG_LEN-1] >> 1;  // c=c>>1
			}
			else
			{
				carry = 0;
				for (i=0; i<DIG_LEN; i++)
				{
					sum = c[i] + P[i] + carry;
					if (sum < c[i])
					{
						carry = 1;
					}
					else if (sum > c[i])
					{
						carry = 0;
					}
					c[i] = sum;
				}
				for (i=0; i<DIG_LEN-1; i++)
				{
					c[i] = (c[i] >> 1) | (c[i+1] << 31);
				}
				c[DIG_LEN-1] = (c[DIG_LEN-1] >> 1) | (carry << 31);
			}// c=(c+P)>>1
		}// end while (v[0]%2 == 0)

		flag = 0;
		i = DIG_LEN;
		while (i--)
		{
			if (u[i] > v[i])
			{
				flag = 1;
				break;
			}
			else if (u[i] < v[i])
			{
				flag = -1;
				break;
			}
		}// u >= v?;

		if (flag >= 0)
		{
			sub(u, u, v);
			sub(a, a, c);
		}
		else
		{
			sub(v, v, u);
			sub(c, c, a);
		}

		for (flagu=1,i=0; i<DIG_LEN; i++)
		{
			if (u[i] != 0)
			{
				flagu = 0;
				break;
			}// u == 0 ?
		}
	}

	for (i=0; i<DIG_LEN; i++)
	{
		x[i] = c[i];
	}
}

//==============================================================
//operation of big int
//r = x mod N using barrett reduction, N is order of base point
void modorder(big r, big x)
{
	small order[DIG_LEN+1] = {
		0x39D54123, 0x53BBF409, 0x21C6052B, 0x7203DF6B,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
		0x00000000,
	};
	// 2^(32*(DIG_LEN*2))/order; order=N
	small mu[DIG_LEN+1] = {
		0xf15149a0, 0x12ac6361, 0xfa323c01, 0x8dfc2096,
		0x00000001, 0x00000001, 0x00000001, 0x00000001,
		0x00000001,
	};

	small r0, r1, r2;
	small x0, x1, y0, y1;
	small s1, s2, s3, s4;
	small *q1, q2[DIG_LEN+1], clone[DIG_LEN+2];
	small borrow, sum;
	int i, k, flag;
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

// multiplication(k=DIG_LEN) */
	r0 = 0;
	r1 = 0;
	r2 = 0;
	for (k=0; k<=DIG_LEN; k++)
	{
		for (i=k; i<=DIG_LEN;i++)
		{
			if (mu[i] != 0)
			{
				mul_and_add(r2, r1, r0, mu[i], x[2*DIG_LEN-1+k-i]);
			}
		}
		clone[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	clone[DIG_LEN+1] = r0;
	q1 = clone + 1; // q1 = ((x/b^(k-1))*mu)/b^(k+1)

	r0 = 0;
	r1 = 0;
	r2 = 0;
	for (k=0; k<DIG_LEN+1; k++)
	{
		for (i=0; i<=k; i++)
		{
			mul_and_add(r2, r1, r0, q1[i], order[k-i]);
		}
		q2[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}// q2 = q1*order mod b^(k+1)

	borrow = 0;
	for (i=0; i<DIG_LEN+1; i++)
	{
		sum = x[i] - q2[i] - borrow;
		if (x[i] < q2[i])
		{
			borrow = 1;
		}
		else if (x[i] > q2[i])
		{
			borrow = 0;
		}
		q1[i] = sum;
	}// q1 = x- q2


	flag = 0;
	i = DIG_LEN + 1;
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
		for (i=0; i<DIG_LEN+1; i++)
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
		i = DIG_LEN + 1;
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
	for (i=0; i<DIG_LEN; i++)
	{
		r[i] = q1[i];  // r = q1
	}

#undef mul_and_add
}

//r = x mod p
void modp(big r, big x, big p) {
	int flag, i;
	small sum, borrow;

	flag = 0;
	i = DIG_LEN;
	while (i--)	{
		if (x[i] > p[i]) {
			flag = 1;
			break;
		}
		else if (x[i] < p[i]) {
			flag = -1;
			break;
		}
	}// x >= p?

	if (flag < 0) {
		for (i=0; i<DIG_LEN; i++) {
			r[i] = x[i];
		}
		return;
	}
	else {
		borrow = 0;
		for (i=0; i<DIG_LEN; i++) {
			sum = x[i] - p[i] - borrow;
			if (x[i] < p[i]) {
				borrow = 1;
			}
			else if (x[i] > p[i]) {
				borrow = 0;
			}
			r[i] = sum;
		}
	}
}

// w = u + v mod p */
void modadd(big w, big u, big v, big p)
{
	int i, flag;
	small sum, borrow, carry;

    // addition */
	carry = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		sum = u[i] + v[i] + carry;
		if (sum < u[i])
		{
			carry = 1;
		}
		else if (sum > u[i])
		{
			carry = 0;
		}
		w[i] = sum;
	}

    // reduction */
	if (carry)
	{
		borrow = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = w[i] - p[i] - borrow;
			if (w[i] < p[i])
			{
				borrow = 1;
			}
			else if (w[i] > p[i])
			{
				borrow = 0;
			}
			w[i] = sum;
		}

		return;
	}// w = w - p

	flag = 0;
	i = DIG_LEN;
	while (i--)
	{
		if (w[i] > p[i])
		{
			flag = 1;
			break;
		}
		else if (w[i] < p[i])
		{
			flag = -1;
			break;
		}
	}// w >= p?

	if (flag < 0)
	{
		return;
	}
	else
	{
		borrow = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = w[i] - p[i] - borrow;
			if (w[i] < p[i])
			{
				borrow = 1;
			}
			else if (w[i] > p[i])
			{
				borrow = 0;
			}
			w[i] = sum;
		}
	}
}

// Compute w = u - v mod p */
void modsub(big w, big u, big v, big p)
{
	int i;
	small sum, borrow, carry;

    // subtraction */
	borrow = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		sum = u[i] - v[i] - borrow;
		if (u[i] < v[i])
		{
			borrow = 1;
		}
		else if (u[i] > v[i])
		{
			borrow = 0;
		}
		w[i] = sum;
	}

    // reduction */
	if (borrow)
	{
		carry = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = w[i] + p[i] + carry;
			if (sum < p[i])
			{
				carry = 1;
			}
			else if (sum > p[i])
			{
				carry = 0;
			}
			w[i] = sum;
		}
	}
}

// computes x = y^(-1) mod p */
void modinv(big x, big y, big p)
{
	small u[DIG_LEN], v[DIG_LEN], a[DIG_LEN]={1}, c[DIG_LEN]={0};
	small sum, carry;
	int i, flagu, flag;

	for (i=0; i<DIG_LEN; i++)
	{
		u[i] = y[i];
		v[i] = p[i];
	}

	for (flagu=1,i=0; i<DIG_LEN; i++)
	{
		if (u[i] != 0)
		{
			flagu = 0;
			break;
		}// u == 0 ?
	}

	while (! flagu)
	{
		while (u[0] % 2 == 0)
		{
			for (i=0; i<DIG_LEN-1; i++)
			{
				u[i] = (u[i] >> 1) | (u[i+1] << 31);
			}
			u[DIG_LEN-1] = u[DIG_LEN-1] >> 1;  // u=u>>1

			if (a[0] % 2 == 0)
			{
				for (i=0; i<DIG_LEN-1; i++)
				{
					a[i] = (a[i] >> 1) | (a[i+1] << 31);
				}
				a[DIG_LEN-1] = a[DIG_LEN-1] >> 1;  //a=a>>1
			}
			else
			{
				carry = 0;
				for (i=0; i<DIG_LEN; i++)
				{
					sum = a[i] + p[i] + carry;
					if (sum < a[i])
					{
						carry = 1;
					}
					else if (sum > a[i])
					{
						carry = 0;
					}
					a[i] = sum;
				}
				for (i=0; i<DIG_LEN-1; i++)
				{
					a[i] = (a[i] >> 1) | (a[i+1] << 31);
				}
				a[DIG_LEN-1] = (a[DIG_LEN-1] >> 1) | (carry << 31);
			}// a=(a+P)>>1
		}
		while (v[0] % 2 == 0)
		{
			for (i=0; i<DIG_LEN-1; i++)
			{
				v[i] = (v[i] >> 1) | (v[i+1] << 31);
			}
			v[DIG_LEN-1] = v[DIG_LEN-1] >> 1;  // v=v>>1

			if (c[0] % 2 == 0)
			{
				for (i=0; i<DIG_LEN-1; i++)
				{
					c[i] = (c[i] >> 1) | (c[i+1] << 31);
				}
				c[DIG_LEN-1] = c[DIG_LEN-1] >> 1;  // c=c>>1
			}
			else
			{
				carry = 0;
				for (i=0; i<DIG_LEN; i++)
				{
					sum = c[i] + p[i] + carry;
					if (sum < c[i])
					{
						carry = 1;
					}
					else if (sum > c[i])
					{
						carry = 0;
					}
					c[i] = sum;
				}
				for (i=0; i<DIG_LEN-1; i++)
				{
					c[i] = (c[i] >> 1) | (c[i+1] << 31);
				}
				c[DIG_LEN-1] = (c[DIG_LEN-1] >> 1) | (carry << 31);
			}// c=(c+P)>>1
		}

		flag = 0;
		i = DIG_LEN;
		while (i--)
		{
			if (u[i] > v[i])
			{
				flag = 1;
				break;
			}
			else if (u[i] < v[i])
			{
				flag = -1;
				break;
			}
		}// compare(flag, u, v);

		if (flag >= 0)
		{
			modsub(u, u, v, p);
			modsub(a, a, c, p);
		}
		else
		{
			modsub(v, v, u, p);
			modsub(c, c, a, p);
		}

		for (flagu=1,i=0; i<DIG_LEN; i++)
		{
			if (u[i] != 0)
			{
				flagu = 0;
				break;
			}// u == 0 ?
		}
	}

	for (i=0; i<DIG_LEN; i++)
	{
		x[i] = c[i];
	}
}

/* Computes product w = u * v mod N */
void mulmodorder(big w, big u, big v)
{
	small r0=0, r1=0, r2=0;
	small x0, x1, y0, y1;
	small s1, s2, s3, s4;
	small clone[2*DIG_LEN];
	int i, j, k;

// add x*y to (r2, r1, r0) */
#define mul_and_add(r2, r1, r0, x, y)			\
do {							\
	x0 = LOHALF(x); x1 = HIHALF(x);			\
	y0 = LOHALF(y); y1 = HIHALF(y);			\
	s1 = x0*y0+LOHALF(r0);				\
	s2 = x0*y1+HIHALF(r0);				\
	s3 = x1*y0+LOHALF(s2)+HIHALF(s1);		\
	s4 = x1*y1+HIHALF(s2)+HIHALF(s3);		\
	r0 = TOHIGH(s3)|LOHALF(s1);			\
	r1 += s4;					\
	r2 += (r1 < s4);				\
} while (0)


	/* multiplication */
	for (k=0; k<=2*(DIG_LEN-1); k++)
	{
		i = (k-(DIG_LEN-1) <= 0) ? 0: (k-(DIG_LEN-1));
		j = (k <= DIG_LEN-1) ? k : (DIG_LEN-1);
	//	i = MAX(0, k-(DIG_LEN-1)); j=MIN(k, DIG_LEN-1);
		while (i <= j)
		{
			mul_and_add(r2, r1, r0, u[i], v[k-i]);
			i++;
		}
		clone[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	clone[2*DIG_LEN-1] = r0;
	modorder(w, clone);

#undef mul_and_add
}

//==============================================================
//operation of ECC point

void projpointdouble(point p, point q)
{
	small t1[DIG_LEN], t2[DIG_LEN], t3[DIG_LEN], t4[DIG_LEN], t5[DIG_LEN];
	int i, flag1, flag2;

	for (i=0; i<DIG_LEN; i++)
	{
		t1[i] = q->x[i];
		t2[i] = q->y[i];
		t3[i] = q->z[i];
	}

	for (flag1=1,i=0; i<DIG_LEN; i++)
	{
		if (t2[i] != 0)
		{
			flag1 = 0;
			break;
		}
	}// t2==0 ?
	for (flag2=1,i=0; i<DIG_LEN; i++)
	{
		if (t3[i] != 0)
		{
			flag2 = 0;
			break;
		}
	}// t3==0 ?

	if (flag1 || flag2)
	{
		for (i=1; i<DIG_LEN; i++)
		{
			p->x[i] = 0;
			p->y[i] = 0;
			p->z[i]=0;
		}
		p->x[0] = 1;
		p->y[0] = 1;
		p->z[0] = 0;

		return;
	}// if q=O or q = -q, output (1, 1, 0), return;

	//Let A=-3
	squ(t4, t3);
	sub(t5, t1, t4);
	add(t4, t1, t4);
	mul(t5, t4, t5);
	add(t4, t5, t5);
	add(t4, t4, t5);  // computes M
	mul(t3, t2, t3);
	add(t3, t3, t3);

	for (i=0; i<DIG_LEN; i++)
	{
		p->z[i] = t3[i];
	}// p->z = t3; compute p->z

	squ(t2, t2);
	mul(t5, t1, t2);
	add(t5, t5, t5);
	add(t5, t5, t5);  // compute S
	squ(t1, t4);
	add(t3, t5, t5);
	sub(t1, t1, t3);  // compute p->x
	squ(t2, t2);
	add(t2, t2, t2);
	add(t2, t2, t2);
	add(t2, t2, t2);  // compute T
	sub(t5, t5, t1);
	mul(t5, t4, t5);
	sub(t2, t5, t2);  // compute p->y

	for (i=0; i<DIG_LEN; i++)
	{
		p->x[i] = t1[i];
		p->y[i] = t2[i];
	}// p->x = t1, p->y = t2;
}

void mixpointadd(point r, point p, epoint q)
{
	small t1[DIG_LEN], t2[DIG_LEN], t3[DIG_LEN], t4[DIG_LEN];
	small t5[DIG_LEN], t6[DIG_LEN];
	small sum, carry;
	int i, flag1, flag2;

	for (i=0; i<DIG_LEN; i++)
	{
		t1[i] = p->x[i];
		t2[i] = p->y[i];
		t3[i] = p->z[i];
		t4[i] = q->x[i];
		t5[i] = q->y[i];
	}

	for (flag1=1,i=0; i<DIG_LEN; i++)
	{
		if (t3[i] != 0)
		{
			flag1 = 0;
			break;
		}
	}// p->z==0 ?
	if (flag1)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			r->x[i] = q->x[i];
			r->y[i] = q->y[i];
			r->z[i] = 0;
		}
		r->z[0] = 1;

		return;
	}// if p->z==0, r=q, return;

	for (flag2=1,i=0; i<DIG_LEN; i++)
	{
		if ((t4[i] != 0) || (t5[i] != 0))
		{
			flag2 = 0;
			break;
		}
	}// q==(0,0) ?

	if (flag2)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			r->x[i] = p->x[i];
			r->y[i] = p->y[i];
			r->z[i] = p->z[i];
		}

		return;
	}// if q==(0,0), r=p, return;

	squ(t6, t3);
	mul(t4, t4, t6);  // compute U1
	mul(t6, t3, t6);
	mul(t5, t5, t6);  // compute S1
	sub(t4, t1, t4);  // compute W
	sub(t5, t2, t5);  // compute R

	for (flag1=1,i=0; i<DIG_LEN; i++)
	{
		if (t4[i] != 0)
		{
			flag1 = 0;
			break;
		}
	}// t4 == 0?
	for (flag2=1,i=0; i<DIG_LEN; i++)
	{
		if (t5[i] != 0)
		{
			flag2 = 0;
			break;
		}
	}// t5 == 0?
	if (flag1)
	{
		if (flag2)
		{
			projpointdouble(r, p);
			return;
		}
		else
		{
			for (i=1; i<DIG_LEN; i++)
			{
				r->x[i] = 0;
				r->y[i] = 0;
				r->z[i] = 0;
			}
			r->x[0] = 1;
			r->y[0] = 1;
			r->z[0] = 0;

			return;  // if q = -p, output (1, 1, 0)
		}
	}

	add(t1, t1, t1);
	sub(t1, t1, t4);  // comput T
	add(t2, t2, t2);
	sub(t2, t2, t5);  // comput M
	mul(t3, t3, t4);

	for (i=0; i<DIG_LEN; i++)
	{
		r->z[i] = t3[i];
	}// r->z = t3; compute r->z

	squ(t6, t4);
	mul(t4, t4, t6);
	mul(t6, t1, t6);
	squ(t1, t5);
	sub(t1, t1, t6);

	for (i=0; i<DIG_LEN; i++)
	{
		r->x[i] = t1[i];
	}// p->x = t1; compute p->x

	add(t1, t1, t1);
	sub(t6, t6, t1);  // compute V
	mul(t5, t5, t6);
	mul(t4, t2, t4);
	sub(t2, t5, t4);

	if (t2[0] % 2 == 0)
	{
		for (i=0; i<DIG_LEN-1; i++)
		{
			r->y[i] = (t2[i] >> 1) | (t2[i+1] << 31);
		}
		r->y[DIG_LEN-1] = t2[DIG_LEN-1] >> 1;  // r->y=t2>>1

	}
	else
	{
		carry = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = t2[i] + P[i] + carry;
			if (sum < t2[i])
			{
				carry = 1;
			}
			else if (sum > t2[i])
			{
				carry = 0;
			}
			t2[i] = sum;
		}
		for (i=0; i<DIG_LEN-1; i++)
		{
			r->y[i] = (t2[i] >> 1) | (t2[i+1] << 31);
		}
		r->y[DIG_LEN-1] = (t2[DIG_LEN-1] >> 1) | (carry << 31);
		// r->y = (t2+P)/2
	}// compute r->y = t2/2
}

void projpointadd(point r, point p, point q)
{
	small t1[DIG_LEN], t2[DIG_LEN], t3[DIG_LEN], t4[DIG_LEN];
	small t5[DIG_LEN], t6[DIG_LEN], t7[DIG_LEN];
	small sum, carry;
	int i, flag1, flag2;

	for (i=0; i<DIG_LEN; i++)
	{
		t1[i] = p->x[i];
		t2[i] = p->y[i];
		t3[i] = p->z[i];
		t4[i] = q->x[i];
		t5[i] = q->y[i];
		t6[i] = q->z[i];
	}

	for (flag1=1,i=0; i<DIG_LEN; i++)
	{
		if (t3[i] != 0)
		{
			flag1 = 0;
			break;
		}
	}// p->z==0 ?
	if (flag1)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			r->x[i] = q->x[i];
			r->y[i] = q->y[i];
			r->z[i] = q->z[i];
		}

		return;
	}// if p->z==0, r=q, return;

	for (flag2=1,i=0; i<DIG_LEN; i++)
	{
		if (t6[i] != 0)
		{
			flag2 = 0;
			break;
		}
	}// q->z==0 ?
	if (flag2)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			r->x[i] = p->x[i];
			r->y[i] = p->y[i];
			r->z[i] = p->z[i];
		}

		return;
	}// if q->z==0, r=p, return;

	squ(t7, t6);
	mul(t1, t1, t7);  // compute U0
	mul(t7, t6, t7);
	mul(t2, t2, t7);  // compute S0

	squ(t7, t3);
	mul(t4, t4, t7);  // compute U1
	mul(t7, t3, t7);
	mul(t5, t5, t7);  // compute S1
	sub(t4, t1, t4);  // compute W
	sub(t5, t2, t5);  // compute R

	for (flag1=1,i=0; i<DIG_LEN; i++)
	{
		if (t4[i] != 0)
		{
			flag1 = 0;
			break;
		}
	}// t4 == 0?
	for (flag2=1,i=0; i<DIG_LEN; i++)
	{
		if (t5[i] != 0)
		{
			flag2 = 0;
			break;
		}
	}// t5 == 0?

	if (flag1)
	{
		if (flag2)
		{
			projpointdouble(r, p);
			return;
		}
		else
		{
			for (i=1; i<DIG_LEN; i++)
			{
				r->x[i] = 0;
				r->y[i] = 0;
				r->z[i]=0;
			}
			r->x[0] = 1;
			r->y[0] = 1;
			r->z[0] = 0;

			return;  // if q = -p, output (1, 1, 0)
		}
	}

	add(t1, t1, t1);
	sub(t1, t1, t4);  // comput T
	add(t2, t2, t2);
	sub(t2, t2, t5);  // comput M
	mul(t3, t3, t6);  // if t6 != 1
	mul(t3, t3, t4);

	for (i=0; i<DIG_LEN; i++)
	{
		r->z[i] = t3[i];
	}// r->z = t3; compute r->z

	squ(t7, t4);
	mul(t4, t4, t7);
	mul(t7, t1, t7);
	squ(t1, t5);
	sub(t1, t1, t7);

	for (i=0; i<DIG_LEN; i++)
	{
		r->x[i] = t1[i];
	}// r->x = t1; compute r->x

	add(t1, t1, t1);
	sub(t7, t7, t1);  // compute V
	mul(t5, t5, t7);
	mul(t4, t2, t4);
	sub(t2, t5, t4);

	if (t2[0] % 2 == 0)
	{
		for (i=0; i<DIG_LEN-1; i++)
		{
			r->y[i] = (t2[i] >> 1) | (t2[i+1] << 31);
		}
		r->y[DIG_LEN-1] = t2[DIG_LEN-1] >> 1;  // r->y = t2>>1

	}
	else
	{
		carry = 0;
		for (i=0; i<DIG_LEN; i++)
		{
			sum = t2[i] + P[i] + carry;
			if (sum < t2[i])
			{
				carry = 1;
			}
			else if (sum > t2[i])
			{
				carry = 0;
			}
			t2[i] = sum;
		}
		for (i=0; i<DIG_LEN-1; i++)
		{
			r->y[i] = (t2[i] >> 1) | (t2[i+1] << 31);
		}
		r->y[DIG_LEN-1] = (t2[DIG_LEN-1] >> 1) | (carry << 31);
		// r->y = (t2+P)/2
	}// compute r->y = t2/2
}

// compute r = p + q */
void pointadd(epoint r, epoint p, epoint q)
{
	int i, flag;
	small t1[DIG_LEN], t2[DIG_LEN], lambda[DIG_LEN];

	for (flag=1,i=0; i<DIG_LEN; i++)
	{
		if ((p->x[i] != 0) || (p->y[i] != 0))
		{
			flag = 0;
			break;
		}
	}// p==(0,0) ?
	if (flag)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			r->x[i] = q->x[i];
			r->y[i] = q->y[i];
		}
		return;
	}// if p==(0,0), r=q, return;

	for (flag=1,i=0; i<DIG_LEN; i++)
	{
		if ((q->x[i] != 0) || (q->y[i] != 0))
		{
			flag = 0;
			break;
		}
	}// q==(0,0) ?
	if (flag)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			r->x[i] = p->x[i];
			r->y[i] = p->y[i];
		}
		return;
	}// if q==(0,0), r=p, return;

	for (flag=1,i=0; i<DIG_LEN; i++)
	{
		if (p->x[i] != q->x[i])
		{
			flag = 0;
			break;
		}
	}// p->x == q->x ?
	if (flag)
	{
		for (flag=1,i=0; i<DIG_LEN; i++)
		{
			if (p->y[i] != q->y[i])
			{
				flag = 0;
				break;
			}
		}// p->y == q->y ?
		if (! flag)
		{
			for (i=0; i<DIG_LEN; i++)
			{
				r->x[i] = 0;
				r->y[i] = 0;
			}
			return;
		}// if p->y != q->y, r=(0,0), return;

		for (flag=1,i=0; i<DIG_LEN; i++)
		{
			if (p->y[i] != 0)
			{
				flag = 0;
				break;
			}
		}// p->y == 0 ?
		if (flag)
		{
			for (i=0; i<DIG_LEN; i++)
			{
				r->x[i] = 0;
				r->y[i] = 0;
			}
			return;
		}// if p->y == 0, r=(0,0), return;

		squ(t1, q->x);
		add(t2, t1, t1);
		add(t1, t1, t2);
		add(t1, t1, A);
		add(t2, q->y, q->y);
		inv(t2, t2);
		mul(lambda, t1, t2);  // lambda=(3x1^2 + a)/(2y1)
	}
	else
	{
		sub(t1, p->x, q->x);
		inv(t1, t1);
		sub(t2, p->y, q->y);
		mul(lambda, t1, t2);  // lambda=(y0 - y1)/(x0 - x1)
	}
	squ(t1, lambda);
	sub(t1, t1, p->x);
	sub(t1, t1, q->x);  // x2 = t1 = lambda^2 - x0 - x1
	sub(t2, q->x, t1);
	mul(t2, t2, lambda);
	sub(t2, t2, q->y);  // y2 = t2 = (x1-x2)lambda - y1

	for (i=0; i<DIG_LEN; i++)
	{
		r->x[i] = t1[i];
		r->y[i] = t2[i];
	}// r = (t1, t2);
}

// compute p = nq with N > n >= 0
//    using window NAF method with window width w = 5
void pointmul(epoint p, epoint q, big n)
{
	int naf[M + 1]={0};  // the lenth of naf is at most lenth(N)+1;
	int lnaf, flag, i, j, l;
	int flag1, flag2;
	small carry, k[DIG_LEN];
	projpoint pre[8], temp;
	projpoint clone={{1},{1},{0}};

	for (flag1=1,i=0; i<DIG_LEN; i++)
	{
		if (n[i] != 0)
		{
			flag1 = 0;
			break;
		}
	}// n==0 ?
	for (flag2=1,i=0; i<DIG_LEN; i++)
	{
		if ((q->x[i] != 0) || (q->y[i] != 0))
		{
			flag2 = 0;
			break;
		}
	}// q==(0,0) ?
	if (flag1 || flag2)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			p->x[i] = 0;
			p->y[i] = 0;
		}
		return;
	}// if n=0 or q=(0,0), return the point of infinity (0,0)

	/* compute the w-NAF of n*/
	for (i=0; i<DIG_LEN; i++)
	{
		k[i] = n[i];
	}
	i = 0;
	while (! flag1)
	{
		if (k[0] % 2 == 0)
		{
			naf[i++] = 0;

			for (j=0; j<DIG_LEN-1; j++)
			{
				k[j] = (k[j] >> 1) | (k[j+1] << 31);
			}
			k[DIG_LEN-1] = k[DIG_LEN-1] >> 1;  // k=k>>1
		}
		else
		{
			flag = k[0] & 0x1f; // w[0]^0x1f is the last 5 bit of k

			for (j=0; j<DIG_LEN-1; j++)
			{
				k[j] = (k[j] >> 5) | (k[j+1] << 27);
			}
			k[DIG_LEN-1] = k[DIG_LEN-1] >> 5;  // k=k>>5

			for (flag1=1,j=0; j<DIG_LEN; j++)
			{
				if (k[j] !=0 )
				{
					flag1 = 0;
					break;
				}
			}// k==0 ?

			if (flag < 16)
			{
				naf[i++] = flag;
				if (! flag1)
				{
					naf[i++] = 0;
					naf[i++] = 0;
					naf[i++] = 0;
					naf[i++] = 0;
				}
			}
			else
			{
				naf[i++] = flag - 32;

				carry = 1;
				j = 0;
				while (carry)
				{
					k[j]++;
					if (k[j] == 0)
					{
						carry=1;
					}
					else
					{
						carry = 0;
					}
					j++;
				}// k++;
				flag1 = 0;
				naf[i++] = 0;
				naf[i++] = 0;
				naf[i++] = 0;
				naf[i++] = 0;
			}
		}
	}// end while (! flag1)
	lnaf = i;
	while (1)
	{
		if (naf[lnaf-1] == 0)
		{
			lnaf--;
		}
		else
		{
			break;
		}
	}// the length of naf, finish NAF

	/* precompute of iP (i = 1, 3, 5, .... , 2^(w-1) - 1) */
	for (i=0; i<DIG_LEN; i++)
	{
		pre[0].x[i] = q->x[i];
		pre[0].y[i] = q->y[i];
		pre[0].z[i]=0;
	}
	pre[0].z[0] = 1;// pre[0] = q

	projpointdouble(&temp, &pre[0]);
	for (i=0; i<7; i++)
	{
		projpointadd(&pre[i+1], &pre[i], &temp);
	}// finish precompute

	for (j=lnaf-1; j>=0; j--)
	{
		projpointdouble(&clone, &clone);
		if (naf[j] != 0)
		{
			if (naf[j] > 0)
			{
				i = (naf[j]-1)/2;
				projpointadd(&clone, &clone, &pre[i]);
			}
			if (naf[j] < 0)
			{
				i = (-naf[j]-1)/2;

				sub(temp.y, P, pre[i].y);
				for (l=0; l<DIG_LEN; l++)
				{
					temp.x[l] = pre[i].x[l];
					temp.z[l] = pre[i].z[l];
				}// temp = -pre[i];

				projpointadd(&clone, &clone, &temp);
			}
		}
	}

	for (flag=1,i=0; i<DIG_LEN; i++)
	{
		if (clone.z[i] !=0 )
		{
			flag = 0;
			break;
		}
	}// clone.z==0 ?
	if (flag)
	{
		for (i=0; i<DIG_LEN; i++)
		{
			p->x[i] = 0;
			p->y[i] = 0;
		}
		return;
	}// if clone.z==0, p=(0,0),return;
	else
	{
		squ(p->x, clone.z);        // Z^2
		mul(p->x, p->x, clone.z);  // Z^3
		inv(p->x, p->x);           // Z^(-3)
		mul(p->y, clone.y, p->x);  // y = Y/Z^3
		mul(p->x, clone.z, p->x);
		mul(p->x, clone.x, p->x);  // x = X/Z^2
	}// convert projective to affine
}


// compute p = nG, where G is the basepoint
void basepointmul(epoint p, big n)
{
	pointmul(p, &G, n);
}

//==============================================================
//operation of ECC API
// Sign a message
//  All length is byte length of data
int EccSign(unsigned char *hash, unsigned int hash_len,
			unsigned char *random, unsigned int random_len,
			unsigned char *sk, unsigned int sk_len,
			unsigned char *sign, unsigned int *sign_len)
{
	int i, j;
	unsigned int prikey[DIG_LEN]={0}, rand[DIG_LEN]={0}, digest[DIG_LEN]={0};
	unsigned int r[DIG_LEN]={0}, s[DIG_LEN]={0}, e[DIG_LEN]={0};
	unsigned int con1[DIG_LEN]={1};
	affpoint kg;
	unsigned int x;

	if (hash_len != 4*DIG_LEN) 	{
		return(-1);  //hash length is not valid, must be 4*DIG_LEN bytes
	}
	if (random_len != 4*DIG_LEN) 	{
		return(-1);  //random length is not valid
	}
	if (sk_len!=4*DIG_LEN) 	{
		return(-1);  //privite key is not valid
	}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)	{
		prikey[DIG_LEN-1-i] = (((unsigned int)(sk[j]) << 24) | ((unsigned int)(sk[j+1]) << 16)
					| ((unsigned int)(sk[j+2]) << 8) | ((unsigned int)(sk[j+3])));
	}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)	{
		rand[DIG_LEN-1-i] = (((unsigned int)(random[j]) << 24) | ((unsigned int)(random[j+1]) << 16)
					| ((unsigned int)(random[j+2]) << 8) | ((unsigned int)(random[j+3])));
	}
	//x = 0;
	//for (i=0; i<DIG_LEN; i++) {
	//	x |= rand[i];
	//}
	//if (x == 0) {
	//	return(-1);  // rand==0，unvalid
	//}
	//flag = 0;
	//i = DIG_LEN;
	//while (i--)	{
	//	if (rand[i] > N[i])
	//	{
	//		flag = 1;
	//		break;
	//	}
	//	else if (rand[i] < N[i])
	//	{
	//		flag = -1;
	//		break;
	//	}
	//}// rand >= N?
	//if (flag >= 0) {
	//	return(-1);  // rand>=N，unvalid
	//}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)
	{
		digest[DIG_LEN-1-i] = (((unsigned int)(hash[j]) << 24) | ((unsigned int)(hash[j+1]) << 16)
					| ((unsigned int)(hash[j+2]) << 8) | ((unsigned int)(hash[j+3])));
	}

	basepointmul(&kg, rand);

	modp(s, kg.x, N); //x1
	modp(e, digest, N); //e

	modadd(r, s, e, N); //r=e+x1
	x = 0;
	for (i=0; i<DIG_LEN; i++) {
		x |= r[i];
	}
	if (x == 0) {
		return(-1);  // r==0，unvalid
	}

	modadd(s, r, rand, N); //s=r+k
	x = 0;
	for (i=0; i<DIG_LEN; i++) {
		x |= s[i];
	}
	if (x == 0) {
		return(-1);  // r+k==N，unvalid
	}

	mulmodorder(s, r, prikey); //r*dA
	modsub(e, rand, s, N); //k-r*dA
	modadd(s, con1, prikey, N);//1+dA
	modinv(s, s, N);//(1+dA)^-1
	mulmodorder(s, s, e);//(1+dA)^-1(k-r*dA) mod N

	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= s[i];
	}
	if (x == 0)
	{
		return(-1);  // s==0，s is not valid, sign error
	}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4) {
		sign[j] = (unsigned char)((r[DIG_LEN-1-i] >> 24) & 0xff);
		sign[j+1] = (unsigned char)((r[DIG_LEN-1-i] >> 16) & 0xff);
		sign[j+2] = (unsigned char)((r[DIG_LEN-1-i] >> 8)&0xff);
		sign[j+3] = (unsigned char)((r[DIG_LEN-1-i]) & 0xff);

		sign[j+4*DIG_LEN] = (unsigned char)((s[DIG_LEN-1-i] >> 24) & 0xff);
		sign[j+1+4*DIG_LEN] = (unsigned char)((s[DIG_LEN-1-i] >> 16) & 0xff);
		sign[j+2+4*DIG_LEN] = (unsigned char)((s[DIG_LEN-1-i] >> 8) & 0xff);
		sign[j+3+4*DIG_LEN] = (unsigned char)((s[DIG_LEN-1-i]) & 0xff);
	}// sign=(r,s)

	*sign_len = 8 * DIG_LEN;

	return(0);
}

// ecc verify a sign
int EccVerify(unsigned char *hash, unsigned int hash_len,
			  unsigned char *pk, unsigned int pk_len,
			  unsigned char *sign, unsigned int sign_len)
{
	int i, j, flag;
	unsigned int digest[DIG_LEN]={0}, r[DIG_LEN]={0}, s[DIG_LEN]={0}, e[DIG_LEN]={0};
	affpoint p1, p2, p3;
	unsigned int x;

	if (hash_len != 4*DIG_LEN)
	{
		return(-1);  // hash unvalid
	}
	if (pk_len != 8*DIG_LEN)
	{
		return(-1);  // public key unvalid
	}
	if (sign_len != 8*DIG_LEN)
	{
		return(-1);  // sign unvalid
	}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)
	{
		p1.x[DIG_LEN-1-i] = (((unsigned int)(pk[j]) << 24) | ((unsigned int)(pk[j+1]) << 16)
					| ((unsigned int)(pk[j+2])<<8) | ((unsigned int)(pk[j+3])));
		p1.y[DIG_LEN-1-i] = (((unsigned int)(pk[j+4*DIG_LEN]) << 24) | ((unsigned int)(pk[j+1+4*DIG_LEN]) << 16)
					| ((unsigned int)(pk[j+2+4*DIG_LEN]) << 8) | ((unsigned int)(pk[j+3+4*DIG_LEN])));
	}// p1=pk

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)
	{
		r[DIG_LEN-1-i] = (((unsigned int)(sign[j]) << 24) | ((unsigned int)(sign[j+1]) << 16)
					| ((unsigned int)(sign[j+2]) << 8) | ((unsigned int)(sign[j+3])));
		s[DIG_LEN-1-i] = (((unsigned int)(sign[j+4*DIG_LEN]) << 24)|((unsigned int)(sign[j+1+4*DIG_LEN]) << 16)
					| ((unsigned int)(sign[j+2+4*DIG_LEN]) << 8)|((unsigned int)(sign[j+3+4*DIG_LEN])));
	}// (r,s)=sign

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)
	{
		digest[DIG_LEN-1-i] = (((unsigned int)(hash[j]) << 24)|((unsigned int)(hash[j+1]) << 16)
					| ((unsigned int)(hash[j+2]) << 8) | ((unsigned int)(hash[j+3])));
	}

	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= r[i];
	}
	if (x == 0)
	{
		return(-1);  // r==0, r unvalid
	}
	flag = 0;
	i = DIG_LEN;
	while (i--)
	{
		if (r[i] > N[i])
		{
			flag = 1;
			break;
		}
		else if (r[i] < N[i])
		{
			flag = -1;
			break;
		}
	}// r >= N?
	if (flag >= 0)
	{
		return(-1);  // r>=N, r unvalid
	}

	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= s[i];
	}
	if (x == 0)
	{
		return(-1);  // s==0, s unvalid
	}
	flag = 0;
	i = DIG_LEN;
	while (i--)
	{
		if (s[i] > N[i])
		{
			flag = 1;
			break;
		}
		else if (s[i] < N[i])
		{
			flag = -1;
			break;
		}
	}// s >= N?
	if (flag >= 0)
	{
		return(-1);  // s>=N, s unvalid
	}

	modadd(e, r, s, N); //t=r'+s' mod N
	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= e[i];
	}
	if (x == 0)
	{
		return(-1);  // t==0, unvalid
	}

	basepointmul(&p2, s);
	pointmul(&p3, &p1, e);
	pointadd(&p1, &p2, &p3);   //p1 = sG + tPA

	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= (p1.x[i] | p1.y[i]);
	}
	if (x == 0)
	{
		return(-1);  // if p1==(0,0), p1 is infinity, unvalid
	}

	modp(s, p1.x, N);
	modp(e, digest, N);

	modadd(s, s, e, N); //R=e+x1

	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= (s[i] ^ r[i]);
	}
	if (x != 0)
	{
		return(1);  // verify fail, R!=r
	}

	return(0);
}














int EccMakeKey(unsigned char *sk, unsigned int sk_len,
			   unsigned char *pk, unsigned int *pk_len)
{
	int i, j;
	unsigned int prikey[DIG_LEN]={0}, x;
	affpoint pubkey;

	if (sk_len != 4*DIG_LEN)
	{
		return(-1);  //privite key is not valid
	}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)
	{
		prikey[DIG_LEN-1-i] = (((unsigned int)(sk[j]) << 24) | ((unsigned int)(sk[j+1]) << 16)
					| ((unsigned int)(sk[j+2]) << 8) | ((unsigned int)(sk[j+3])));
	}

	basepointmul(&pubkey, prikey);  //compute public key

	x = 0;
	for (i=0; i<DIG_LEN; i++)
	{
		x |= (pubkey.x[i] | pubkey.y[i]);
	}
	if (x == 0)
	{
		return(-1); //privite key is not valid
	}

	for (i=0,j=0; i<DIG_LEN; i++,j+=4)
	{
		pk[j] = (unsigned char)((pubkey.x[DIG_LEN-1-i] >> 24) & 0xff);
		pk[j+1] = (unsigned char)((pubkey.x[DIG_LEN-1-i] >> 16) & 0xff);
		pk[j+2] = (unsigned char)((pubkey.x[DIG_LEN-1-i] >> 8) & 0xff);
		pk[j+3] = (unsigned char)((pubkey.x[DIG_LEN-1-i]) & 0xff);

		pk[j+4*DIG_LEN] = (unsigned char)((pubkey.y[DIG_LEN-1-i] >> 24) & 0xff);
		pk[j+1+4*DIG_LEN] = (unsigned char)((pubkey.y[DIG_LEN-1-i] >> 16) & 0xff);
		pk[j+2+4*DIG_LEN] = (unsigned char)((pubkey.y[DIG_LEN-1-i] >> 8) & 0xff);
		pk[j+3+4*DIG_LEN] = (unsigned char)((pubkey.y[DIG_LEN-1-i]) & 0xff);
	}

	*pk_len = 8*DIG_LEN;

	return(0);
}
