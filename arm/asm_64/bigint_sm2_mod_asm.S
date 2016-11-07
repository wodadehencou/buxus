
.section .text, "x"
.align 4
.global bigint_sm2_mod
.type bigint_sm2_mod, @function

bigint_sm2_mod:
//r: x10 x11 x12 x13 x14 x15 x16 x17
//w: x6 x7 x8 x9 
//carry: x18

	LDP		x10, x11, [x1], #0x10
	LDP		x12, x13, [x1], #0x10
	LDP		x14, x15, [x1], #0x10
	LDP		x16, x17, [x1]

loop_w0:
	ADDS	x6, x10, x14
	CSET	x7, CS

	//LSR		x2, x14, #32 //a4>>32
	//ORR		x2, x2, x15, LSL #32 //x2=a4|a5
	EXTR	x2, x15, x14, #32
	ADDS	x6, x6, x2 
	CINC	x7, x7, CS

	ADDS	x6, x6, x15
	CINC	x7, x7, CS

	//LSR		x3, x15, #32
	//ORR		x3, x3, x16, LSL #32 //x3=a5|a6
	EXTR	x3, x16, x15, #32
	ADDS	x6, x6, x3 
	CINC	x7, x7, CS

	ADDS	x6, x6, x16 
	CINC	x7, x7, CS

	//LSR		x4, x16, #32
	//ORR		x4, x4, x17, LSL #32 //x4=a6|a7
	EXTR	x4, x17, x16, #32
	ADDS	x6, x6, x4 
	CINC	x7, x7, CS
	ADDS	x6, x6, x4 
	CINC	x7, x7, CS

	ADDS	x6, x6, x17 
	CINC	x7, x7, CS
	ADDS	x6, x6, x17 
	CINC	x7, x7, CS

	LSR		x18, x17, #32
	ADDS	x6, x6, x18, LSL #1
	CINC	x7, x7, CS

loop_w1:
	ADDS	x7, x7, x11
	CSET	x8, CS

	ADDS	x7, x7, x14, LSL #32
	CINC	x8, x8, CS

	AND		x18, x15, 0xffffffff00000000
	ADDS	x7, x7, x18
	CINC	x8, x8, CS

	ADDS	x7, x7, x16, LSL #32
	CINC	x8, x8, CS
	
	AND		x18, x16, 0xffffffff00000000
	ADDS	x7, x7, x18
	CINC	x8, x8, CS
	ADDS	x7, x7, x18
	CINC	x8, x8, CS

	ADDS	x7, x7, x17, LSL #32
	CINC	x8, x8, CS
	
	AND		x18, x17, 0xffffffff00000000
	ADDS	x7, x7, x18
	CINC	x8, x8, CS

loop_w2:
	ADDS	x8, x8, x12
	CSET	x9, CS

	ADDS	x8, x8, x2
	CINC	x9, x9, CS

	ADDS	x8, x8, x16
	CINC	x9, x9, CS

	ADDS	x8, x8, x4
	CINC	x9, x9, CS

	ADDS	x8, x8, x17
	CINC	x9, x9, CS
	ADDS	x8, x8, x17
	CINC	x9, x9, CS

	ADDS	x8, x8, x17, LSR #32
	CINC	x9, x9, CS

loop_w3:
	ADDS	x9, x9, x13
	CSET	x18, CS

	ADDS	x9, x9, x14, LSL #32
	CINC	x18, x18, CS

	AND		x5, x14, 0xffffffff00000000
	ADDS	x9, x9, x5
	CINC	x18, x18, CS

	ADDS	x9, x9, x15, LSL #32
	CINC	x18, x18, CS

	AND		x5, x15, 0xffffffff00000000
	ADDS	x9, x9, x5
	CINC	x18, x18, CS

	ADDS	x9, x9, x3
	CINC	x18, x18, CS

	ADDS	x9, x9, x16, LSL #32
	CINC	x18, x18, CS

	AND		x5, x16, 0xffffffff00000000
	ADDS	x9, x9, x5
	CINC	x18, x18, CS
	ADDS	x9, x9, x5
	CINC	x18, x18, CS

	ADDS	x9, x9, x17, LSL #32
	CINC	x18, x18, CS
	ADDS	x9, x9, x17, LSL #32
	CINC	x18, x18, CS

	ADDS	x9, x9, x17
	CINC	x18, x18, CS

	ADDS	x9, x9, x17, LSR #32
	CINC	x18, x18, CS

	AND		x5, x17, 0xffffffff00000000
	ADDS	x9, x9, x5
	CINC	x18, x18, CS
	ADDS	x9, x9, x5
	CINC	x18, x18, CS

loop_carry:
	ADDS	x6, x6, x18
	
	LSL		x5, x18, #32
	ADCS	x7, x7, x5

	ADCS	x8, x8, XZR

	LSL		x5, x18, #32
	ADCS	x9, x9, x5
	CSET	x5, CS //x5 is final carry

sub:
	AND		x2, x14, 0x00000000ffffffff
	AND		x3, x17, 0x00000000ffffffff
	ADD		x2, x2, x14, LSR #32
	ADD		x3, x3, x16, LSR #32
	ADD		x2, x2, x18
	ADD		x2, x2, x3
	SUBS	x7, x7, x2

	SBCS	x8, x8, XZR
	SBCS	x9, x9, XZR
	CSET	x4, CC //x4 is final borrow
	
//p0:
//	DCQ		0xFFFFFFFFFFFFFFFF
//p1:
//	DCQ		0xFFFFFFFF00000000
//p2:
//	DCQ		0xFFFFFFFFFFFFFFFF
//p3:
//	DCQ		0xFFFFFFFEFFFFFFFF

carry_borrow:
	MOV		x10, 0xFFFFFFFFFFFFFFFF
	MOV		x11, 0xFFFFFFFF00000000
	MOV		x12, 0xFFFFFFFFFFFFFFFF
	MOV		x13, 0xFFFFFFFEFFFFFFFF

	CMP		x5, x4

	B.HI	sub_p
	B.EQ	mod_p
	B.LS	add_p

sub_p:
	SUBS	x6, x6, x10
	SBCS	x7, x7, x11
	SBCS	x8, x8, x12
	SBCS	x9, x9, x13
	B.CC	mod_p
	B.CS	sub_p

mod_p:
	CMP		x9, x13
	B.HI	mod_sub_p
	B.LS	return
	CMP		x8, x12
	B.HI	mod_sub_p
	B.LS	return
	CMP		x7, x11
	B.HI	mod_sub_p
	B.LS	return
	CMP		x6, x10
	B.HI	mod_sub_p
	B.LS	return
	B		mod_sub_p

add_p:
	ADDS	x6, x6, x10
	ADCS	x7, x7, x11
	ADCS	x8, x8, x12
	ADCS	x9, x9, x13
	B.CS	return
	B.CC	add_p

mod_sub_p:
	SUBS	x6, x6, x10
	SBCS	x7, x7, x11
	SBCS	x8, x8, x12
	SBCS	x9, x9, x13

return:
	STP		x6, x7, [x0], #0x10
	STP		x8, x9, [x0]

	RET

