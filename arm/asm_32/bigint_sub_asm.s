	AREA BIGINTSUB,CODE,READONLY
	CODE32
	ENTRY

	EXPORT bigint_sub

bigint_sub FUNCTION

	PUSH     {r4-r6,lr}
	;r0=rp
	;r1=ap
	;r2=bp
	MOV		r6, #0

loop0
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SUBS	r6, r4, r5
	STR		r6, [r0], #4
loop1
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4
loop2
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4
loop3
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4
loop4
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4
loop5
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4
loop6
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4
loop7
	LDR		r4, [r1], #4
	LDR		r5, [r2], #4
	SBCS	r6, r4, r5
	STR		r6, [r0], #4

	MOVCS	r0, #0
	BCS		{pc}+8
	MOV		r0, #1
    POP		{r4-r6,pc}

    ENDFUNC

	END
