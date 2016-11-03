
	AREA BIGINTMUL,CODE,READONLY
	CODE32
	ENTRY

	EXPORT bigint_mul

bigint_mul FUNCTION
	PUSH	{r3-r10, lr}
	MOV		r3, r1 ;ap
	MOV		r4, r2 ;bp

;r9,r8,r7 
;a[0]*b[0]
	MOV		r9, #0
	LDR		r5, [r3], #4 ;a0->a1
	LDR		r6, [r4], #0 ;b0
	UMULL	r7, r8, r5, r6 ;(r8,r7)=r5*r6
	STR		r7, [r0], #4 ; store r[0]

;r7,r9,r8 
;a[1]*b[0]
;a[0]*b[1]
	MOV		r7, #0 
	MOV		r12, #2
loop_2_2
	LDR		r5, [r3], #-4 ;a1->a0
	LDR		r6, [r4], #4 ;b0->b1
	MOV		r10, r9
	UMLAL	r8, r9, r5, r6 ;a[1]*b[0]
	CMP		r9, r10
	ADDCC	r7, r7, #1 ;r7++ if carry
	SUBS	r12, r12, #1
	BNE		loop_2_2

	STR		r8, [r0], #4 ; store r[1]

;a[-1] b[2]	
;r8,r7,r9 
;a[0]*b[2]
;a[1]*b[1]
;a[2]*b[0]
	MOV		r8, #0 
	MOV		r12, #3
	ADD		r3, r3, #4 ;a[0]
loop_3_3
	LDR		r5, [r3], #4 ;a0->a1
	LDR		r6, [r4], #-4 ;b2->b1
	MOV		r10, r7
	UMLAL	r9, r7, r5, r6 ;a[0]*b[2]
	CMP		r7, r10
	ADDCC	r8, r8, #1 ;r8++ if carry
	SUBS	r12, r12, #1
	BNE		loop_3_3

	STR		r9, [r0], #4 ; store r[2]

;a[3], b[-1]
;r9,r8,r7 
;a[3]*b[0]
;a[2]*b[1]
;a[1]*b[2]
;a[0]*b[3]
	MOV		r9, #0 
	MOV		r12, #4
	ADD		r4, r4, #4 ;b[0]
loop_4_4
	LDR		r5, [r3], #-4 ;a3->a2
	LDR		r6, [r4], #4 ;b0->b1
	MOV		r10, r8
	UMLAL	r7, r8, r5, r6 ;a[3]*b[0]
	CMP		r8, r10
	ADDCC	r9, r9, #1 ;r9++ if carry
	SUBS	r12, r12, #1
	BNE		loop_4_4

	STR		r7, [r0], #4 ; store r[3]


;a[-1], b[4]
;r7,r9,r8 
;a[0]*b[4]
;a[1]*b[3]
;a[2]*b[2]
;a[3]*b[1]
;a[4]*b[0]
	MOV		r7, #0 
	MOV		r12, #5
	ADD		r3, r3, #4 ;a[0]
loop_5_5
	LDR		r5, [r3], #4 ;a0->a1
	LDR		r6, [r4], #-4 ;b4->b3
	MOV		r10, r9
	UMLAL	r8, r9, r5, r6 ;a[0]*b[4]
	CMP		r9, r10
	ADDCC	r7, r7, #1 ;r7++ if carry
	SUBS	r12, r12, #1
	BNE		loop_5_5

	STR		r8, [r0], #4 ; store r[4]

;a[5], b[-1]
;r8,r7,r9 
;a[5]*b[0]
;a[4]*b[1]
;a[3]*b[2]
;a[2]*b[3]
;a[1]*b[4]
;a[0]*b[5]
	MOV		r8, #0 
	MOV		r12, #6
	ADD		r4, r4, #4 ;b[0]
loop_6_6
	LDR		r5, [r3], #-4 ;a5->a4
	LDR		r6, [r4], #4 ;b0->b1
	MOV		r10, r7
	UMLAL	r9, r7, r5, r6 ;a[5]*b[0]
	CMP		r7, r10
	ADDCC	r8, r8, #1 ;r8++ if carry
	SUBS	r12, r12, #1
	BNE		loop_6_6

	STR		r9, [r0], #4 ; store r[5]


;a[-1], b[6]
;r9,r8,r7 
;a[0]*b[6]
;a[1]*b[5]
;a[2]*b[4]
;a[3]*b[3]
;a[4]*b[2]
;a[5]*b[1]
;a[6]*b[0]
	MOV		r9, #0 
	MOV		r12, #7
	ADD		r3, r3, #4 ;a[0]
loop_7_7
	LDR		r5, [r3], #4 ;a0->a1
	LDR		r6, [r4], #-4 ;b6->b5
	MOV		r10, r8
	UMLAL	r7, r8, r5, r6 ;a[0]*b[6]
	CMP		r8, r10
	ADDCC	r9, r9, #1 ;r9++ if carry
	SUBS	r12, r12, #1
	BNE		loop_7_7

	STR		r7, [r0], #4 ; store r[6]

;a[7], b[-1]
;r7,r9,r8 
;a[7]*b[0]
;a[6]*b[1]
;a[5]*b[2]
;a[4]*b[3]
;a[3]*b[4]
;a[2]*b[5]
;a[1]*b[6]
;a[0]*b[7]
	MOV		r7, #0 
	MOV		r12, #8
	ADD		r4, r4, #4 ;b[0]
loop_8_8
	LDR		r5, [r3], #-4 ;a7->a6
	LDR		r6, [r4], #4 ;b0->b1
	MOV		r10, r9
	UMLAL	r8, r9, r5, r6 ;a[7]*b[0]
	CMP		r9, r10
	ADDCC	r7, r7, #1 ;r7++ if carry
	SUBS	r12, r12, #1
	BNE		loop_8_8

	STR		r8, [r0], #4 ; store r[7]

;a[-1], b[8]
;r8,r7,r9 
;a[1]*b[7]
;a[2]*b[6]
;a[3]*b[5]
;a[4]*b[4]
;a[5]*b[3]
;a[6]*b[2]
;a[7]*b[1]
	MOV		r8, #0 
	MOV		r12, #7
	ADD		r3, r3, #8 ;a[1]
	SUB		r4, r4, #4 ;b[7]
loop_9_7
	LDR		r5, [r3], #4 ;a1->a2
	LDR		r6, [r4], #-4 ;b7->b6
	MOV		r10, r7
	UMLAL	r9, r7, r5, r6 ;a[1]*b[7]
	CMP		r7, r10
	ADDCC	r8, r8, #1 ;r8++ if carry
	SUBS	r12, r12, #1
	BNE		loop_9_7

	STR		r9, [r0], #4 ; store r[8]

;a[8], b[0]
;r9,r8,r7 
;a[7]*b[2]
;a[6]*b[3]
;a[5]*b[4]
;a[4]*b[5]
;a[3]*b[6]
;a[2]*b[7]
	MOV		r9, #0 
	MOV		r12, #6
	SUB		r3, r3, #4 ;a[7]
	ADD		r4, r4, #8 ;b[2]
loop_10_6
	LDR		r5, [r3], #-4 ;a7->a6
	LDR		r6, [r4], #4 ;b2->b3
	MOV		r10, r8
	UMLAL	r7, r8, r5, r6 ;a[7]*b[2]
	CMP		r8, r10
	ADDCC	r9, r9, #1 ;r9++ if carry
	SUBS	r12, r12, #1
	BNE		loop_10_6

	STR		r7, [r0], #4 ; store r[9]

;a[1], b[8]
;r7,r9,r8 
;a[3]*b[7]
;a[4]*b[6]
;a[5]*b[5]
;a[6]*b[4]
;a[7]*b[3]
	MOV		r7, #0 
	MOV		r12, #5
	ADD		r3, r3, #8 ;a[3]
	SUB		r4, r4, #4 ;b[7]
loop_11_5
	LDR		r5, [r3], #4 ;a3->a4
	LDR		r6, [r4], #-4 ;b7->b6
	MOV		r10, r9
	UMLAL	r8, r9, r5, r6 ;a[3]*b[7]
	CMP		r9, r10
	ADDCC	r7, r7, #1 ;r7++ if carry
	SUBS	r12, r12, #1
	BNE		loop_11_5

	STR		r8, [r0], #4 ; store r[10]

;a[8], b[2]
;r8,r7,r9 
;a[7]*b[4]
;a[6]*b[5]
;a[5]*b[6]
;a[4]*b[7]
	MOV		r8, #0 
	MOV		r12, #4
	SUB		r3, r3, #4 ;a[7]
	ADD		r4, r4, #8 ;b[4]
loop_12_4
	LDR		r5, [r3], #-4 ;a7->a6
	LDR		r6, [r4], #4 ;b4->b5
	MOV		r10, r7
	UMLAL	r9, r7, r5, r6 ;a[7]*b[4]
	CMP		r7, r10
	ADDCC	r8, r8, #1 ;r8++ if carry
	SUBS	r12, r12, #1
	BNE		loop_12_4

	STR		r9, [r0], #4 ; store r[11]

;a[3],b[8]
;r9,r8,r7 
;a[5]*b[7]
;a[6]*b[6]
;a[7]*b[5]
	MOV		r9, #0 
	MOV		r12, #3
	ADD		r3, r3, #8 ;a[5]
	SUB		r4, r4, #4 ;b[7]
loop_13_3
	LDR		r5, [r3], #4 ;a5->a6
	LDR		r6, [r4], #-4 ;b7->b6
	MOV		r10, r8
	UMLAL	r7, r8, r5, r6 ;a[5]*b[7]
	CMP		r8, r10
	ADDCC	r9, r9, #1 ;r9++ if carry
	SUBS	r12, r12, #1
	BNE		loop_13_3

	STR		r7, [r0], #4 ; store r[12]

;a[8], b[4]
;r7,r9,r8 
;a[7]*b[6]
;a[6]*b[7]
	MOV		r7, #0 
	MOV		r12, #2
	SUB		r3, r3, #4 ;a[5]
	ADD		r4, r4, #8 ;b[6]
loop_14_2
	LDR		r5, [r3], #-4 ;a7->a6
	LDR		r6, [r4], #4 ;b6->b7
	MOV		r10, r9
	UMLAL	r8, r9, r5, r6 ;a[7]*b[6]
	CMP		r9, r10
	ADDCC	r7, r7, #1 ;r7++ if carry
	SUBS	r12, r12, #1
	BNE		loop_14_2

	STR		r8, [r0], #4 ; store r[13]

;a[5], b[8]
;r8,r7,r9 
;a[7]*b[7]
	LDR		r5, [r3, #8] ;a7
	LDR		r6, [r4, #-4] ;b7
	UMLAL	r9, r7, r5, r6 ;a[7]*b[7]
	STR		r9, [r0], #4 ; store r[14]

	STR		r7, [r0], #0 ; store r[15]

	POP		{r3-r10, pc}
	ENDFUNC

	END





