
function void sm2_test;

	reg		[DWIDTH-1 : 0]	sk		;
	reg		[DWIDTH-1 : 0]	pk_x	;
	reg		[DWIDTH-1 : 0]	pk_y	;
	reg		[DWIDTH-1 : 0]	hash	;
	reg		[DWIDTH-1 : 0]	random	;

	reg		[DWIDTH-1 : 0]	r		;
	reg		[DWIDTH-1 : 0]	s		;

	int		ret;

	sk = DWIDTH'h128B2FA8_BD433C6C_068C8D80_3DFF7979_2A519A55_171B1B65_0C23661D_15897263;
	hash = DWIDTH'hB524F552_CD82B8B0_28476E00_5C377FB1_9A87E6FC_682D48BB_5D42E3D9_B9EFFE76;
	random = DWIDTH'h6CB28D99_385C175C_94F94E93_4817663F_C176D925_DD72B727_260DBAAE_1FB2F96F;

	pointmul(G_X, G_Y, sk, pk_x, pk_y);
	ret = sm2_sign(hash, random, sk, r, s);
	if (ret != 0) $display("sign error");
	ret = sm2_verify(hash, pk_x, pk_y, r, s);
	if (ret != 0) $display("verify error");

endfunction
