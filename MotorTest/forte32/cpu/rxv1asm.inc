;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
	VECT_TASK_SWITCH	.equ	1	; TRAPA Vector
	VECT_SUPERVISOR		.equ	2	; TRAPA Vector
	VECT_ENTER_CRITICAL	.equ	3	; TRAPA Vector
	VECT_LEAVE_CRITICAL	.equ	4	; TRAPA Vector
	VECT_WAIT_US		.equ	5	; TRAPA Vector
	VECT_RESTORE_SYSTEM	.equ	6	; TRAPA Vector
	VECT_DISABLE_INTERRUPT	.equ	7	; TRAPA Vector
	VECT_ENABLE_INTERRUPT	.equ	8	; TRAPA Vector
	VECT_INTERVAL_TIMER	.equ	28	; TRAPA Vector

;---------------------------------------

;---------------------------------------
StoreContext	.macro	TaskContext

	push.l	r14			;
	push.l	r15			;

	;
	; タスクのレジスタ配列のポインタ
	;
	mov.l	#TaskContext, r14	; pointer of task register array
	mov.l	[r14], r15		;

	;
	; 割り込み前のPCとPSWをレジスタ配列に保存
	;
	mov.l	8[r0], r14		; pc on stack
	mov.l	r14, 16 * 4[r15]	;
	mov.l	12[r0], r14		; psw on stack
	mov.l	r14, 17 * 4[r15]	;

	;
	; ユーザーモードのスタックポインタをレジスタ配列に保存
	;
	mvfc	usp, r14		; user mode stack pointer before interrupt
	mov.l	r14, 0 * 4[r15]		; r0

	;
	; ACC
	;
	mvfacmi	r14			;
	shll	#16, r14		;
	mov.l	r14, 18 * 4[r15]	;
	mvfachi	r14			;
	mov.l	r14, 19 * 4[r15]	;

	;
	; R1～R13をレジスタ配列に保存
	;
	mov.l	r1, 1 * 4[r15]		; r1
	mov.l	r2, 2 * 4[r15]		; r2
	mov.l	r3, 3 * 4[r15]		; r3
	mov.l	r4, 4 * 4[r15]		; r4
	mov.l	r5, 5 * 4[r15]		; r5
	mov.l	r6, 6 * 4[r15]		; r6
	mov.l	r7, 7 * 4[r15]		; r7
	mov.l	r8, 8 * 4[r15]		; r8
	mov.l	r9, 9 * 4[r15]		; r9
	mov.l	r10, 10 * 4[r15]	; r10
	mov.l	r11, 11 * 4[r15]	; r11
	mov.l	r12, 12 * 4[r15]	; r12
	mov.l	r13, 13 * 4[r15]	; r13

	mov.l	r15, r13		;

	pop	r15			;
	pop	r14			;

	;
	; R14、R15をレジスタ配列に保存
	;
	mov.l	r14, 14*4[r13]		; r14
	mov.l	r15, 15*4[r13]		; r15

		.endm

;---------------------------------------

;---------------------------------------
RestoreContext	.macro	TaskContext

	;
	; タスクのレジスタ配列のポインタ
	;
	mov.l	#TaskContext, r14	; pointer of task register array
	mov.l	[r14], r15		;

	;
	; タスクのレジスタ配列からユーザーモードのスタックポインタを戻す
	;
	mov.l	0 * 4[r15], r14		; restore user mode stack pointer
	mvtc	r14, usp		; 

	;
	; スタック上にPCとPSWをレジスタ配列から戻す
	;
	mov.l	16 * 4[r15], r14	; pc on stack
	mov.l	r14, 0[r0]		;
	mov.l	17 * 4[r15], r14	; psw on stack
	mov.l	r14, 4[r0]		;

	;
	; ACC
	;
	mov.l	18 * 4[r15], r14	;
	mvtaclo	r14			;
	mov.l	19 * 4[r15], r14	;
	mvtachi	r14			;

	;
	; R1～R13をレジスタ配列から戻す
	;	R0は自動復帰する
	;
	mov.l	1 * 4[r15], r1		; r1
	mov.l	2 * 4[r15], r2		; r2
	mov.l	3 * 4[r15], r3		; r3
	mov.l	4 * 4[r15], r4		; r4
	mov.l	5 * 4[r15], r5		; r5
	mov.l	6 * 4[r15], r6		; r6
	mov.l	7 * 4[r15], r7		; r7
	mov.l	8 * 4[r15], r8		; r8
	mov.l	9 * 4[r15], r9		; r9
	mov.l	10 * 4[r15], r10	; r10
	mov.l	11 * 4[r15], r11	; r11
	mov.l	12 * 4[r15], r12	; r12
	mov.l	13 * 4[r15], r13	; r13
	mov.l	14 * 4[r15], r14	; r14
	mov.l	15 * 4[r15], r15	; r15

		.endm
