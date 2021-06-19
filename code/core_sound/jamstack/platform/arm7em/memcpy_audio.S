#if defined (__ARM_ARCH_7EM__)
#include <SoundStream.h>

.cpu cortex-m4
.syntax unified
.thumb
.text

/* void memcpy_tointerleave(short *dst, short *srcL, short *srcR); */
 .global	memcpy_tointerleaveLR
.thumb_func
	memcpy_tointerleaveLR:

	@ r0: dst
	@ r1: srcL
	@ r2: srcR

#if SOUND_BLOCK_SAMPLES > 8
	push	{r4-r11,r14}
	add r14,r0,#(SOUND_BLOCK_SAMPLES*2)
	.align 2
.loopLR:
	//Load 2*4 words
	ldmia r1!, {r5,r7,r9,r11}  //1+4
	ldmia r2!, {r6,r8,r10,r12} //1+4

	pkhbt r3,r5,r6,LSL #16	//1
	pkhtb r4,r6,r5,ASR #16	//1

	pkhbt r5,r7,r8,LSL #16	//1
	pkhtb r6,r8,r7,ASR #16	//1

	pkhbt r7,r9,r10,LSL #16	//1
	pkhtb r8,r10,r9,ASR #16	//1

	pkhbt r9,r11,r12,LSL #16	//1
	pkhtb r10,r12,r11,ASR #16	//1

	//Write 8 Words
	stmia r0!, {r3,r4,r5,r6,r7,r8,r9,r10}	//1+8 -> 5+5+8+9 = 27 Cycles to interleave 32 bytes.

	cmp r14, r0
	bne .loopLR

	pop	{r4-r11,r14}
#elif SOUND_BLOCK_SAMPLES == 8
	push	{r4-r8,r14}

	ldmia r1!, {r5,r7}
	ldmia r2!, {r6,r8}

	pkhbt r3,r5,r6,LSL #16
	pkhtb r4,r6,r5,ASR #16

	pkhbt r5,r7,r8,LSL #16
	pkhtb r6,r8,r7,ASR #16

	stmia r0!, {r3,r4,r5,r6}
	pop	{r4-r8,r14}
#endif
	BX lr


/* void memcpy_tointerleaveL(short *dst, short *srcL); */
 .global	memcpy_tointerleaveL
.thumb_func
	memcpy_tointerleaveL:

	@ r0: dst
	@ r1: srcL

	mov r2, #0

#if SOUND_BLOCK_SAMPLES > 8
	push	{r4-r11}
	add r12,r0,#(SOUND_BLOCK_SAMPLES*2)
	.align 2
.loopL:

	//Load 4 words
	ldmia r1!, {r5,r7,r9,r11}  //1+4

	pkhbt r3,r5,r2	//1
	pkhtb r4,r2,r5,ASR #16	//1

	pkhbt r5,r7,r2	//1
	pkhtb r6,r2,r7,ASR #16	//1

	pkhbt r7,r9,r2	//1
	pkhtb r8,r2,r9,ASR #16	//1

	pkhbt r9,r11,r2	//1
	pkhtb r10,r2,r11,ASR #16	//1

	//Write 8 Words
	stmia r0!, {r3,r4,r5,r6,r7,r8,r9,r10}	//1+8

	cmp r12, r0
	bne .loopL

	pop	{r4-r11}
#elif SOUND_BLOCK_SAMPLES == 8
	push	{r4-r7}

	ldmia r1!, {r5,r7}

	pkhbt r3,r5,r2
	pkhtb r4,r2,r5,ASR #16

	pkhbt r5,r7,r2	//1
	pkhtb r6,r2,r7,ASR #16

	stmia r0!, {r3,r4,r5,r6}

	pop	{r4-r7}
#endif
	BX lr


/* void memcpy_tointerleaveL(short *dst, short *srcR); */
 .global	memcpy_tointerleaveR
.thumb_func
	memcpy_tointerleaveR:

	@ r0: dst
	@ r1: srcR

	mov r2, #0
#if SOUND_BLOCK_SAMPLES > 8
	push	{r4-r11}
	add r12,r0,#(SOUND_BLOCK_SAMPLES*2)
	.align 2
.loopR:

	//Load 4 words
	ldmia r1!, {r5,r7,r9,r11}

	pkhbt r3,r2,r5,LSL #16
	pkhtb r4,r5,r2

	pkhbt r5,r2,r7,LSL #16
	pkhtb r6,r7,r2

	pkhbt r7,r2,r9,LSL #16
	pkhtb r8,r9,r2

	pkhbt r9,r2,r11,LSL #16
	pkhtb r10,r11,r2

	//Write 8 Words
	stmia r0!, {r3,r4,r5,r6,r7,r8,r9,r10}

	cmp r12, r0
	bne .loopR

	pop	{r4-r11}
#elif SOUND_BLOCK_SAMPLES == 8
	push	{r4-r7}

	ldmia r1!, {r5,r7}

	pkhbt r3,r2,r5,LSL #16
	pkhtb r4,r5,r2

	pkhbt r5,r2,r7,LSL #16
	pkhtb r6,r7,r2

	stmia r0!, {r3,r4,r5,r6}

	pop	{r4-r7}

#endif
	BX lr



/* void memcpy_tointerleaveQuad(int16_t *dst, const int16_t *src1, const int16_t *src2, const int16_t *src3, const int16_t *src4) */
 .global	memcpy_tointerleaveQuad
.thumb_func
	memcpy_tointerleaveQuad:

	@ r0: dst
	@ r1: src1
	@ r2: src2
	@ r3: src3
	@ r4: src4

	push	{r4-r11}
	ldr r4, [sp, #(0+32)] //5th parameter is saved on the stack
	add r11,r0,#(SOUND_BLOCK_SAMPLES*4)
	.align 2
.loopQuad:

	ldr r5, [r1],4
	ldr r6, [r3],4
	pkhbt r7,r5,r6,LSL #16
	pkhtb r9,r6,r5,ASR #16
	ldr r5, [r2],4
	ldr r6, [r4],4
	pkhbt r8,r5,r6,LSL #16
	pkhtb r10,r6,r5,ASR #16

	stmia r0!, {r7-r10}

	cmp r11, r0
	bne .loopQuad

	pop	{r4-r11}
	BX lr
.END

#endif
