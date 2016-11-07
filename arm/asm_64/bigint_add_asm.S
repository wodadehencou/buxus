.section .text, "x"
.align 4
.global bigint_add
.type bigint_add, @function

bigint_add:

	LDP		x10, x11, [x1], #16
	LDP		x14, x15, [x2], #16
	ADDS	x10, x10, x14
	ADCS	x11, x11, x15
	STP		x10, x11, [x0], #16
	LDP		x12, x13, [x1]
	LDP		x16, x17, [x2]
	ADCS	x12, x12, x16
	ADCS	x13, x13, x17
	STP		x12, x13, [x0]

	CSET	x0, CS
	RET

