
parameter DWIDTH=256;
parameter P=DWIDTH'hFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_00000000_FFFFFFFF_FFFFFFFF;
parameter A=DWIDTH'hFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_00000000_FFFFFFFF_FFFFFFFC;
parameter B=DWIDTH'h28E9FA9E_9D9F5E34_4D5A9E4B_CF6509A7_F39789F5_15AB8F92_DDBCBD41_4D940E93;
parameter N=DWIDTH'hFFFFFFFE_FFFFFFFF_FFFFFFFF_FFFFFFFF_7203DF6B_21C6052B_53BBF409_39D54123;
parameter G_X=DWIDTH'h32C4AE2C_1F198119_5F990446_6A39C994_8FE30BBF_F2660BE1_715A4589_334C74C7;
parameter G_Y=DWIDTH'hBC3736A2_F4F6779C_59BDCEE3_6B692153_D0A9877C_C62A4740_02DF32E5_2139F0A0;

// computer (a+b) mod p
// 0<a,b<p
function [DWIDTH-1 : 0] mod_add (
	input	[DWIDTH-1 : 0]	a	,
	input	[DWIDTH-1 : 0]	b	,
	input	[DWIDTH-1 : 0]	p	
);

	reg		[DWIDTH : 0]	sum	;

	sum = a+b;
	if (sum > p) sum = sum-p;
	return sum[DWIDTH-1 : 0];

endfunction

// computer (a-b) mod p
// 0<a,b<p
function [DWIDTH-1 : 0] mod_sub (
	input	[DWIDTH-1 : 0]	a	,
	input	[DWIDTH-1 : 0]	b	,
	input	[DWIDTH-1 : 0]	p	
);

	reg 	[DWIDTH : 0]	result	;

	if (a<b) result = a+p;
	else result = a;
	result = result - b;
	return result[DWIDTH-1 : 0];

endfunction

// computer (a*b) mod p
// 0<a,b<p
function [DWIDTH-1 : 0] mod_mul (
	input	[DWIDTH-1 : 0]	a	,
	input	[DWIDTH-1 : 0]	b	,
	input	[DWIDTH-1 : 0]	p	
);

	reg 	[2*DWIDTH-1 : 0]	result	;

	result = a * b;
	result = result % p;
	return result[DWIDTH-1 : 0];

endfunction

// computer (a^-1) mod p
// 0<a<p
function [DWIDTH-1 : 0] mod_inv (
	input	[DWIDTH-1 : 0]	a	,
	input	[DWIDTH-1 : 0]	p	
);
	//use gcd
	reg 	[DWIDTH : 0]	r	;
	reg 	[DWIDTH : 0]	s	;
	reg 	[DWIDTH : 0]	u	;
	reg 	[DWIDTH : 0]	v	;

	if (a==0) return 0;

	r = 1;
	s = 0;
	u = a;
	v = p;

	while (v!=1) begin
		while (u[0] == 0) begin
			u = u>>1;
			if (r[0] == 0) r=r>>1;
			else r=(r+p)>>1;
		end

		while (v[0] == 0) begin
			v = v>>1;
			if (s[0] == 0) s=s>>1;
			else s=(s+p)>>1;
		end

		if (u>v) begin
			u = mod_sub(u, v, p);
			r = mod_sub(r, s, p);
		end
		else begin
			v = mod_sub(v, u, p);
			s = mod_sub(s, r, p);
		end
	end
	return s[DWIDTH-1 : 0];
endfunction

// computer a mod p
function [DWIDTH-1 : 0] mod (
	input	[DWIDTH-1 : 0]	a	,
	input	[DWIDTH-1 : 0]	p	
);
	reg 	[DWIDTH-1 : 0]	result	;

	result = a;
	while (result > p) begin
		result = result - p;
	end
	return result;
endfunction

//	Q=2P
function void Affine_double (
	input	[DWIDTH-1 : 0]	p_x	,
	input	[DWIDTH-1 : 0]	p_y	,
	output	[DWIDTH-1 : 0]	q_x	,
	output	[DWIDTH-1 : 0]	q_y	
);

	reg		[DWIDTH-1 : 0]	r1;
	reg		[DWIDTH-1 : 0]	r2;
	reg		[DWIDTH-1 : 0]	r3;
	reg		[DWIDTH-1 : 0]	x3;

	if ((p_x == 0) && (p_y == 0)) begin
		q_x = 0;
		q_y = 0;
		return;
	end

`ifdef A_EQ_M3
	r1 = mod_mul(p_x, p_x, P);
	r1 = mod_sub(r1, 1, P);
	r2 = mod_add(r1, r1, P);
	r1 = mod_add(r2, r1, P);
`else
	r1 = mod_mul(p_x, p_x, P);
	r2 = mod_add(r1, r1, P);
	r1 = mod_add(r2, r1, P);
	r1 = mod_add(r1, a, P);
`endif

	//if (r1 == 0) begin
	//	q_x = 0;
	//	q_y = 0;
	//	return;
	//end
	r2 = mod_add(p_y, p_y, P);
	r2 = mod_inv(r2, P);
	r1 = mod_mul(r1, r2, P); //lamada
	r2 = mod_mul(r1, r1, P);
	r3 = mod_add(p_x, p_x, P);
	x3 = mod_sub(r2, r3, P);
	q_x = x3;
	r3 = mod_sub(p_x, x3, P);
	r2 = mod_mul(r1, r3, P);
	q_y = mod_sub(r2, p_y, P);
	return;

endfunction


//	Q=P1+P2
function void Affine_add (
	input	[DWIDTH-1 : 0]	p1_x,
	input	[DWIDTH-1 : 0]	p1_y,
	input	[DWIDTH-1 : 0]	p2_x,
	input	[DWIDTH-1 : 0]	p2_y,
	output	[DWIDTH-1 : 0]	q_x	,
	output	[DWIDTH-1 : 0]	q_y	
);

	reg		[DWIDTH-1 : 0]	r1;
	reg		[DWIDTH-1 : 0]	r2;
	reg		[DWIDTH-1 : 0]	r3;
	reg		[DWIDTH-1 : 0]	x3;

	if ((p1_x == 0) && (p1_y == 0)) begin
		q_x = p2_x;
		q_y = p2_y;
		return;
	end
	if ((p2_x == 0) && (p2_y == 0)) begin
		q_x = p1_x;
		q_y = p1_y;
		return;
	end
	if (p1_x == p2_x) begin
		if (p1_y == p2_y) begin
			Affine_double(p1_x, p1_y, q_x, q_y);
			return;
		end
		else begin
			q_x = 0;
			q_y = 0;
			return;
		end
	end

	r1 = mod_sub(p2_x, p1_x, P);
	r2 = mod_sub(p2_y, p1_y, P);
	r2 = mod_inv(r2, P);
	r1 = mod_mul(r1, r2, P); //lamada
	r2 = mod_mul(r1, r1, P);
	r3 = mod_add(p1_x, p2_x, P);
	x3 = mod_sub(r2, r3, P);
	q_x = x3;
	r3 = mod_sub(p1_x, x3, P);
	r2 = mod_mul(r1, r3, P);
	q_y = mod_sub(r2, p1_y, P);
	return;
endfunction


function void Jpointdouble (
	input	[DWIDTH-1 : 0]	p	,
	input	[DWIDTH-1 : 0]	p_x	,
	input	[DWIDTH-1 : 0]	p_y	,
	input	[DWIDTH-1 : 0]	p_z	,
	output	[DWIDTH-1 : 0]	q_x	,
	output	[DWIDTH-1 : 0]	q_y	,
	output	[DWIDTH-1 : 0]	q_z	
);
	
	reg		[DWIDTH-1 : 0]	t1	;
	reg		[DWIDTH-1 : 0]	t2	;
	reg		[DWIDTH-1 : 0]	t3	;
	reg		[DWIDTH-1 : 0]	t4	;
	reg		[DWIDTH-1 : 0]	t5	;

	if ((p_y == 0) || (p_z == 0)) begin
		q_x = 1;
		q_y = 1;
		q_z = 0;
		return;
	end

	t1 = p_x;
	t2 = p_y;
	t3 = p_z;
	//let a=-3
	t4 = mod_mul(t3, t3, p);
	t5 = mod_sub(t1, t4, p);
	t4 = mod_add(t1, t4, p);
	t5 = mod_mul(t4, t5, p);
	t4 = mod_add(t5, t5, p);
	t4 = mod_add(t4, t5, p);
	t3 = mod_mul(t2, t3, p);
	t3 = mod_add(t3, t3, p);
	q_z = t3;

	t2 = mod_mul(t2, t2, p);
	t5 = mod_mul(t1, t2, p);
	t5 = mod_add(t5, t5, p);
	t5 = mod_add(t5, t5, p);
	t1 = mod_mul(t4, t4, p);
	t3 = mod_add(t5, t5, p);
	t1 = mod_sub(t1, t3, p);
	t2 = mod_mul(t2, t2, p);
	t2 = mod_add(t2, t2, p);
	t2 = mod_add(t2, t2, p);
	t2 = mod_add(t2, t2, p);
	t5 = mod_sub(t5, t1, p);
	t5 = mod_mul(t4, t5, p);
	t2 = mod_sub(t5, t2, p);
	q_x = t1;
	q_y = t2;
	return;
endfunction

function void pointmul (
	input	[DWIDTH-1 : 0]	p_x	,
	input	[DWIDTH-1 : 0]	p_y	,
	input	[DWIDTH-1 : 0]	k	,
	output	[DWIDTH-1 : 0]	q_x	,
	output	[DWIDTH-1 : 0]	q_y	
);
	reg		[DWIDTH-1 : 0] x;
	reg		[DWIDTH-1 : 0] y;

	if ((k==0) || ((p_x==0) && (p_y==0))) begin
		q_x = 0;
		q_y = 0;
		return;
	end

	x = 0;
	y = 0;
	for (int i=DWIDTH-1; i>=0; i--) begin
		Affine_double(x, y, x, y);
		if (k[i] == 1) begin
			Affine_add(x, y, p_x, p_y, x, y);
		end
	end
	q_x = x;
	q_y = y;
	return;
endfunction

function void pointadd (
	input	[DWIDTH-1 : 0]	p1_x,
	input	[DWIDTH-1 : 0]	p1_y,
	input	[DWIDTH-1 : 0]	p2_x,
	input	[DWIDTH-1 : 0]	p2_y,
	output	[DWIDTH-1 : 0]	q_x	,
	output	[DWIDTH-1 : 0]	q_y	
);
	Affine_add(p1_x, p1_y, p2_x, p2_y, q_x, q_y);
	return;
endfunction

//  0<random<N
function int sm2_sign (
	input	[DWIDTH-1 : 0]		hash	,
	input	[DWIDTH-1 : 0]		random	,
	input	[DWIDTH-1 : 0]		sk		,
	output	[DWIDTH-1 : 0]		sign_r	,	
	output	[DWIDTH-1 : 0]		sign_s		
);
	reg		[DWIDTH-1 : 0]		kg_x	;
	reg		[DWIDTH-1 : 0]		kg_y	;
	reg		[DWIDTH-1 : 0]		e		;
	reg		[DWIDTH-1 : 0]		x1		;
	reg		[DWIDTH-1 : 0]		r		;
	reg		[DWIDTH-1 : 0]		s		;

	reg		[DWIDTH-1 : 0]		tmp1	;
	reg		[DWIDTH-1 : 0]		tmp2	;

	pointmul(G_X, G_Y, random, kg_x, kg_y);
	e = mod(hash, N);
	x1 = mod(kg_x, N);
	r = mod_add(e, x1, N);
	if (r==0) return -1;
	if（mod_add(r, random, N) == 0) return -1;

	tmp1 = mod_mul(r, sk, N)
	tmp2 = mod_sub(random, tmp1, N);
	tmp1 = mod_add(sk, 1, N);
	tmp1 = mod_inv(tmp1, N);
	s = mod_mul(tmp1, tmp2, N);
	if (r==0) return -1;

	sign_r = r;
	sign_s = s;
	return 0;
endfunction

function int sm2_verify (
	input	[DWIDTH-1 : 0]		hash	,
	input	[DWIDTH-1 : 0]		pk_x	,
	input	[DWIDTH-1 : 0]		pk_y	,
	input	[DWIDTH-1 : 0]		sign_r	,	
	input	[DWIDTH-1 : 0]		sign_s	
);
	
	reg		[DWIDTH-1 : 0]		t		;
	reg		[DWIDTH-1 : 0]		sg_x	;
	reg		[DWIDTH-1 : 0]		sg_y	;
	reg		[DWIDTH-1 : 0]		tpa_x	;
	reg		[DWIDTH-1 : 0]		tpa_y	;
	reg		[DWIDTH-1 : 0]		x		;
	reg		[DWIDTH-1 : 0]		y		;
	reg		[DWIDTH-1 : 0]		e		;
	reg		[DWIDTH-1 : 0]		x1		;
	reg		[DWIDTH-1 : 0]		result	;

	if (sign_r == 0) return -1;
	if (sign_r >= N) return -1;
	if (sign_s == 0) return -1;
	if (sign_s >= N) return -1;

	t = mod_add(sign_r, sign_s, N);
	if (t == 0) return -1;
	pointmul(G_X, G_Y, sign_s, sg_x, sg_y);
	pointmul(pk_x, pk_y, t, tpa_x, tpa_y);
	pointadd(sg_x, sg_y, tpa_x, tpa_y, x, y);
	if ((x==0) && (y==0)) return -1;
	e = mod(hash, N);
	x1 = mod(x, N);
	result = mod_add(e, x1, N);
	if (result == sign_r) return 0;
	else return 1;
endfunction

