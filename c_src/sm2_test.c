#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "sm2.h"

#define TIMES 1000

/* 样本数据 */
/* 随机数产生函数(digitrand) */
void digitrand(unsigned char *x, int len)
{
	int i, j;
	static int seed = 1;

	if (seed)
	{
		srand((unsigned long)time(NULL));
		j = rand() & 0xFF;
		while (j--)
		{
			rand();
		}
		seed = 0;
	}

	for (i=0; i < len; i++)
	{
		x[i] = (rand() & 0xFF);
	}
}

/* 正确性校验 */
void correct()
{
	unsigned char DSAe[4*DIG_LEN], DSAQe[8*DIG_LEN];
	unsigned char Hash[4*DIG_LEN];
	unsigned char Random[4*DIG_LEN];
	unsigned char SignCode[8*DIG_LEN];

	unsigned char a[4*DIG_LEN];
	unsigned char b[4*DIG_LEN];
	unsigned char Qa[8*DIG_LEN], Qb[8*DIG_LEN];
	unsigned char ka[4*DIG_LEN];
	unsigned char kb[4*DIG_LEN];
	unsigned char RA[8*DIG_LEN], RB[8*DIG_LEN];
	unsigned char KeyA[4*DIG_LEN], KeyB[4*DIG_LEN];

	unsigned char Ee[4*DIG_LEN], EQe[8*DIG_LEN];
	unsigned char ERandom[4*DIG_LEN];
	unsigned char plain[64], plain1[64];
	unsigned char cipher[1024];

	unsigned char q[8*DIG_LEN],x[4*DIG_LEN],y;

	unsigned char tmp[64];
	unsigned int len;
	int i, times = TIMES;
	clock_t start, end;
	double d1;

	start = clock();
	while (times--)
	{
		digitrand(DSAe, 4*DIG_LEN);
		digitrand(Hash, 4*DIG_LEN);
		digitrand(Random, 4*DIG_LEN);
		digitrand(a, 4*DIG_LEN);
		digitrand(b, 4*DIG_LEN);
		digitrand(ka, 4*DIG_LEN);
		digitrand(kb, 4*DIG_LEN);
		digitrand(Ee, 4*DIG_LEN);
		digitrand(ERandom, 4*DIG_LEN);
		digitrand(plain, 64);

		i = EccMakeKey(DSAe, 4*DIG_LEN, DSAQe, &len);
		if (i!=0)
		{
			printf("\n签名公钥产生失败\n");
			return;
		}

		i = EccSign(Hash, 4*DIG_LEN, Random, 4*DIG_LEN, DSAe, 4*DIG_LEN, SignCode, &len);
		if (i!=0)
		{
			printf("\n签名失败\n");
			return;
		}

		i = EccVerify(Hash, 4*DIG_LEN, DSAQe, 8*DIG_LEN, SignCode, len);
		if (i!=0)
		{
			printf("拒绝签名\n");
		}


	}// 循环一次大概做4次点乘

	end = clock() - start;
	d1 = (double)end / CLOCKS_PER_SEC * 1000. / TIMES;  // 所花时间（毫秒）

	printf("\n随机跑%d次未发现程序错误，每跑一次所花费的时间为：%f毫秒\n\n", TIMES,d1);
	printf("每跑一次随机点乘大概花费时间为：%f毫秒\n\n", d1/4);
	printf("每跑一秒大概能做的随机点乘数目为：%f次\n\n", 1000./(d1/4));
}

/* 主函数 */
void main()
{
	correct();
}

