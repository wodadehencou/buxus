
	AREA BIGINTSQR,CODE,READONLY
	CODE32
	ENTRY

	EXPORT bigint_sqr

bigint_sqr FUNCTION
	PUSH	{r5-r11, lr}

;r9,r8,r7 
;a[0]*a[0]
	LDR		r5, [r1];a[0]
	UMULL	r7, r8, r5, r5 ;a[0]*a[0]
	MOV		r9, #0

	STR		r7, [r0], #4 ; store r[0]

;r7,r9,r8 
;2*a[1]*a[0]
	MOV		r7, #0 
	LDR		r6, [r1, #4];a1
	UMULL	r10, r11, r5, r6 ;a1*a0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	STR		r8, [r0], #4 ; store r[1]

;r8,r7,r9 
	MOV		r8, #0 
	UMULL	r10, r11, r6, r6 ;a1*a1
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r6, [r1, #8];a2
	UMULL	r10, r11, r5, r6 ;a2*a0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	STR		r9, [r0], #4 ; store r[2]

;r9,r8,r7 
	MOV		r9, #0 

	LDR		r6, [r1, #12];a3
	UMULL	r10, r11, r5, r6 ;a3*a0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r5, [r1, #4];a1
	LDR		r6, [r1, #8];a2
	UMULL	r10, r11, r5, r6 ;a1*a2
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	STR		r7, [r0], #4 ; store r[3]


;r7,r9,r8 
	MOV		r7, #0 

	UMULL	r10, r11, r6, r6 ;a[2]*a[2]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r6, [r1, #12];a3
	UMULL	r10, r11, r5, r6 ;a[1]*a[3]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r5, [r1] ;a0
	LDR		r6, [r1, #16];a4
	UMULL	r10, r11, r5, r6 ;a[0]*a[4]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	STR		r8, [r0], #4 ; store r[4]

;r8,r7,r9 
	MOV		r8, #0 
	LDR		r6, [r1, #20];a5
	UMULL	r10, r11, r5, r6 ;a[0]*a[5]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r5, [r1, #4];a1
	LDR		r6, [r1, #16];a4
	UMULL	r10, r11, r5, r6 ;a[1]*a[4]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r5, [r1, #8];a2
	LDR		r6, [r1, #12];a3
	UMULL	r10, r11, r5, r6 ;a[2]*a[3]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	STR		r9, [r0], #4 ; store r[5]


;r9,r8,r7 
	MOV		r9, #0 

	UMULL	r10, r11, r6, r6 ;a[3]*a[3]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r6, [r1, #16];a3
	UMULL	r10, r11, r5, r6 ;a[2]*a[4]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r5, [r1, #4];a1
	LDR		r6, [r1, #20];a5
	UMULL	r10, r11, r5, r6 ;a[1]*a[5]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r5, [r1 ];a0
	LDR		r6, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[0]*a[6]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	STR		r7, [r0], #4 ; store r[6]

;r7,r9,r8 
	MOV		r7, #0 

	LDR		r6, [r1, #28];a7
	UMULL	r10, r11, r5, r6 ;a[0]*a[7]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r5, [r1, #4];a1
	LDR		r6, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[1]*a[6]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r5, [r1, #8];a2
	LDR		r6, [r1, #20];a5
	UMULL	r10, r11, r5, r6 ;a[2]*a[5]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r5, [r1, #12];a3
	LDR		r6, [r1, #16];a4
	UMULL	r10, r11, r5, r6 ;a[3]*a[4]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	STR		r8, [r0], #4 ; store r[7]

;r8,r7,r9 
	MOV		r8, #0 

	UMULL	r10, r11, r6, r6 ;a[4]*a[4]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r5, [r1, #12];a3
	LDR		r6, [r1, #20];a5
	UMULL	r10, r11, r5, r6 ;a[3]*a[5]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r5, [r1, #8];a2
	LDR		r6, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[2]*a[6]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r5, [r1, #4];a1
	LDR		r6, [r1, #28];a7
	UMULL	r10, r11, r5, r6 ;a[1]*a[7]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	STR		r9, [r0], #4 ; store r[8]

;r9,r8,r7 
	MOV		r9, #0 

	LDR		r5, [r1, #8];a2
	UMULL	r10, r11, r5, r6 ;a[2]*a[7]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r5, [r1, #12];a3
	LDR		r6, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[3]*a[6]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r5, [r1, #16];a4
	LDR		r6, [r1, #20];a5
	UMULL	r10, r11, r5, r6 ;a[4]*a[5]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	STR		r7, [r0], #4 ; store r[9]

;r7,r9,r8 
	MOV		r7, #0 

	UMULL	r10, r11, r6, r6 ;a[5]*a[5]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r6, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[4]*a[6]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	LDR		r5, [r1, #12];a3
	LDR		r6, [r1, #28];a7
	UMULL	r10, r11, r5, r6 ;a[3]*a[7]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	STR		r8, [r0], #4 ; store r[10]

;r8,r7,r9 
	MOV		r8, #0 

	LDR		r5, [r1, #16];a4
	UMULL	r10, r11, r5, r6 ;a[4]*a[7]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	LDR		r5, [r1, #20];a5
	LDR		r6, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[5]*a[6]
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0
	ADDS	r9, r9, r10
	ADCS	r7, r7, r11
	ADC		r8, r8, #0

	STR		r9, [r0], #4 ; store r[11]

;r9,r8,r7 
	MOV		r9, #0 

	UMULL	r10, r11, r6, r6 ;a[6]*a[6]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	LDR		r6, [r1, #28];a7
	UMULL	r10, r11, r5, r6 ;a[5]*a[7]
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0
	ADDS	r7, r7, r10
	ADCS	r8, r8, r11
	ADC		r9, r9, #0

	STR		r7, [r0], #4 ; store r[12]

;r7,r9,r8 
	MOV		r7, #0 

	LDR		r5, [r1, #24];a6
	UMULL	r10, r11, r5, r6 ;a[6]*a[7]
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0
	ADDS	r8, r8, r10
	ADCS	r9, r9, r11
	ADC		r7, r7, #0

	STR		r8, [r0], #4 ; store r[13]

;r8,r7,r9 
	UMULL	r10, r11, r6, r6 ;a[7]*a[7]
	ADDS	r9, r9, r10
	ADC		r7, r7, r11

	STR		r9, [r0], #4 ; store r[14]

	STR		r7, [r0], #0 ; store r[15]

	POP		{r5-r11, pc}
	ENDFUNC

	END





