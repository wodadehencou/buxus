
.section .text, "x"
.align 4
.global bigint_sqr
.type bigint_sqr, @function

bigint_sqr:

	LDP		x3, x4, [x1], #0x10
	LDP		x5, x6, [x1] 

	MUL		x11, x3, x3
	UMULH	x12, x3, x3
	MOV		x13, #0

	MUL		x1, x4, x3
	UMULH	x2, x4, x3
	ADDS	x12, x12, x1
	ADCS	x13, x13, x2
	CSET	x14, CS
	ADDS	x12, x12, x1
	ADCS	x13, x13, x2
	CINC	x14, x14, CS

	MUL		x1, x5, x3
	UMULH	x2, x5, x3
	ADDS	x13, x13, x1
	ADCS	x14, x14, x2
	CSET	x15, CS
	ADDS	x13, x13, x1
	ADCS	x14, x14, x2
	CINC	x15, x15, CS

	MUL		x1, x4, x4
	UMULH	x2, x4, x4
	ADDS	x13, x13, x1
	ADCS	x14, x14, x2
	CINC	x15, x15, CS

	MUL		x1, x6, x3
	UMULH	x2, x6, x3
	ADDS	x14, x14, x1
	ADCS	x15, x15, x2
	CSET	x16, CS
	ADDS	x14, x14, x1
	ADCS	x15, x15, x2
	CINC	x16, x16, CS

	MUL		x1, x5, x4
	UMULH	x2, x5, x4
	ADDS	x14, x14, x1
	ADCS	x15, x15, x2
	CINC	x16, x16, CS
	ADDS	x14, x14, x1
	ADCS	x15, x15, x2
	CINC	x16, x16, CS

	MUL		x1, x6, x4
	UMULH	x2, x6, x4
	ADDS	x15, x15, x1
	ADCS	x16, x16, x2
	CSET	x17, CS
	ADDS	x15, x15, x1
	ADCS	x16, x16, x2
	CINC	x17, x17, CS

	MUL		x1, x5, x5
	UMULH	x2, x5, x5
	ADDS	x15, x15, x1
	ADCS	x16, x16, x2
	CINC	x17, x17, CS

	MUL		x1, x6, x5
	UMULH	x2, x6, x5
	ADDS	x16, x16, x1
	ADCS	x17, x17, x2
	CSET	x18, CS
	ADDS	x16, x16, x1
	ADCS	x17, x17, x2
	CINC	x18, x18, CS

	MUL		x1, x6, x6
	UMULH	x2, x6, x6
	ADDS	x17, x17, x1
	ADCS	x18, x18, x2

	STP		x11, x12, [x0], #0x10
	STP		x13, x14, [x0], #0x10
	STP		x15, x16, [x0], #0x10
	STP		x17, x18, [x0]

	RET

