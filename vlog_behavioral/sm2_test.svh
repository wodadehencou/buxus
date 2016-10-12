
function void sm2_test;

	reg		[DWIDTH-1 : 0]	sk		;
	reg		[DWIDTH-1 : 0]	pk_x	;
	reg		[DWIDTH-1 : 0]	pk_y	;
	reg		[DWIDTH-1 : 0]	hash	;
	reg		[DWIDTH-1 : 0]	random	;

	reg		[DWIDTH-1 : 0]	r		;
	reg		[DWIDTH-1 : 0]	s		;

	int		ret;

	sk = 256'h128B2FA8_BD433C6C_068C8D80_3DFF7979_2A519A55_171B1B65_0C23661D_15897263;
	hash = 256'hB524F552_CD82B8B0_28476E00_5C377FB1_9A87E6FC_682D48BB_5D42E3D9_B9EFFE76;
	random = 256'h6CB28D99_385C175C_94F94E93_4817663F_C176D925_DD72B727_260DBAAE_1FB2F96F;

	pointmul(G_X, G_Y, sk, pk_x, pk_y);
	ret = sm2_sign(hash, random, sk, r, s);
	if (ret != 0) $display("sign error");
	ret = sm2_verify(hash, pk_x, pk_y, r, s);
	if (ret != 0) $display("verify error");

endfunction

function [DWIDTH-1:0] gen_random;
	reg [DWIDTH-1:0] r;
	repeat(DWIDTH) begin
		reg rand_bit;
		rand_bit = $urandom;
		r = (r<<1) | rand_bit;
	end
	return r;
endfunction

function void batch_test;
	reg		[DWIDTH-1 : 0]	sk		;
	reg		[DWIDTH-1 : 0]	pk_x	;
	reg		[DWIDTH-1 : 0]	pk_y	;
	reg		[DWIDTH-1 : 0]	hash	;
	reg		[DWIDTH-1 : 0]	random	;

	reg		[DWIDTH-1 : 0]	r		;
	reg		[DWIDTH-1 : 0]	s		;

	int		ret;

	sk = 256'h128B2FA8_BD433C6C_068C8D80_3DFF7979_2A519A55_171B1B65_0C23661D_15897263;
	hash = 256'hB524F552_CD82B8B0_28476E00_5C377FB1_9A87E6FC_682D48BB_5D42E3D9_B9EFFE76;
	random = 256'h6CB28D99_385C175C_94F94E93_4817663F_C176D925_DD72B727_260DBAAE_1FB2F96F;

	repeat(1000) begin
		sk = gen_random();
		sk = mod(sk, N);
		hash = gen_random();
		random = gen_random();
		random = mod(random, N);

		pointmul(G_X, G_Y, sk, pk_x, pk_y);
		ret = sm2_sign(hash, random, sk, r, s);
		if (ret != 0) $display("sign error");
		ret = sm2_verify(hash, pk_x, pk_y, r, s);
		if (ret != 0) $display("verify error");
	end
endfunction
