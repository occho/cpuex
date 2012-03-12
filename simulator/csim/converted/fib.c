/*
########################################################################
## GESO ---- ASM TO C CONVERTER
## Author: Tomoyuki Saito(aocchoda@gmail.com)
########################################################################
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "oc_sim.h"
uint32_t prom[ROM_NUM];
uint32_t ram[RAM_NUM];
int32_t reg[REG_NUM];
uint32_t freg[REG_NUM];
int main(int argc, char** argv) {
	goto min_caml_start;
min_caml_create_array:
	reg[0] = reg[3]<<2U;
	reg[5] = reg[3]+reg[2];
	reg[3] = reg[2];
CREATE_ARRAY_LOOP:
	if (reg[5]<reg[2]) {
		goto CREATE_ARRAY_END;
	}
	if (reg[5]==reg[2]) {
		goto CREATE_ARRAY_END;
	}
	movl		%r13d, -0(%r12d)
	reg[3] = reg[2]+4U;
	goto CREATE_ARRAY_LOOP;
CREATE_ARRAY_END:
	return;
min_caml_create_float_array:
	reg[3] = reg[3]<<2U;
	reg[4] = reg[3]+reg[2];
	reg[3] = reg[2];
CREATE_FLOAT_ARRAY_LOOP:
	if (reg[4]<reg[2]) {
		goto CREATE_FLOAT_ARRAY_END;
	}
	if (reg[4]==reg[2]) {
		goto CREATE_FLOAT_ARRAY_END;
	}
	movss		%xmm0, -0(%r12d)
	reg[3] = reg[2]+4U;
	goto CREATE_FLOAT_ARRAY_LOOP;
CREATE_FLOAT_ARRAY_END:
	return;
min_caml_floor:
	movss		%xmm0, %xmm1
	movl		$FLOAT_ZERO, %r11d
	movss		-0(%r11d), %xmm9
	comiss		%xmm0, %xmm9
	jb		FLOOR_POSITIVE
	comiss		%xmm0, %xmm9
	je		FLOOR_POSITIVE
FLOOR_NEGATIVE:
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	movl		$FLOAT_MAGICF, %r11d
	movss		-0(%r11d), %xmm2
	comiss		%xmm2, %xmm0
	jb		FLOOR_NEGATIVE_MAIN
	comiss		%xmm2, %xmm0
	je		FLOOR_NEGATIVE_MAIN
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	return;
FLOOR_NEGATIVE_MAIN:
	addss		%xmm2, %xmm0
	subss		%xmm2, %xmm0
	movd		%xmm1, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm1
	comiss		%xmm0, %xmm1
	jb		FLOOR_RET2
	comiss		%xmm0, %xmm1
	je		FLOOR_RET2
	addss		%xmm2, %xmm0
	movl		$FLOAT_ONE, %r11d
	movss		-0(%r11d), %xmm6
	addss		%xmm6, %xmm0
	subss		%xmm2, %xmm0
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	return;
FLOOR_POSITIVE:
	movl		$FLOAT_MAGICF, %r11d
	movss		-0(%r11d), %xmm2
	comiss		%xmm2, %xmm0
	jb		FLOOR_POSITIVE_MAIN
	comiss		%xmm2, %xmm0
	je		FLOOR_POSITIVE_MAIN
	return;
FLOOR_POSITIVE_MAIN:
	movss		%xmm0, %xmm1
	addss		%xmm2, %xmm0
	movss		%xmm0, -0(%r9d)
	movl		-0(%r9d), %r13d
	subss		%xmm2, %xmm0
	movss		%xmm0, -0(%r9d)
	movl		-0(%r9d), %r13d
	comiss		%xmm1, %xmm0
	jb		FLOOR_RET
	comiss		%xmm1, %xmm0
	je		FLOOR_RET
	movl		$FLOAT_ONE, %r11d
	movss		-0(%r11d), %xmm6
	subss		%xmm6, %xmm0
FLOOR_RET:
	return;
FLOOR_RET2:
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	return;
min_caml_ceil:
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	min_caml_floor();
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	return;
min_caml_float_of_int:
	if (reg[0]<reg[3]) {
		goto ITOF_MAIN;
	}
	if (reg[0]==reg[3]) {
		goto ITOF_MAIN;
	}
	reg[3] = reg[0]-reg[3];
	ITOF_MAIN();
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	return;
ITOF_MAIN:
	movl		$FLOAT_MAGICF, %r14d
	movss		-0(%r14d), %xmm1
	movl		$FLOAT_MAGICFHX, %r14d
	movl		-0(%r14d), %r13d
	movl		$FLOAT_MAGICI, %r14d
	movl		-0(%r14d), %r14d
	if (reg[5]<reg[3]) {
		goto ITOF_BIG;
	}
	if (reg[5]==reg[3]) {
		goto ITOF_BIG;
	}
	reg[3] = reg[3]+reg[4];
	movl		%r11d, -0(%r9d)
	movss		-0(%r9d), %xmm0
	subss		%xmm1, %xmm0
	return;
ITOF_BIG:
	movl		$FLOAT_ZERO, %r13d
	movss		-0(%r13d), %xmm2
ITOF_LOOP:
	reg[3] = reg[3]-reg[5];
	addss		%xmm1, %xmm2
	if (reg[5]<reg[3]) {
		goto ITOF_LOOP;
	}
	if (reg[5]==reg[3]) {
		goto ITOF_LOOP;
	}
	reg[3] = reg[3]+reg[4];
	movl		%r11d, -0(%r9d)
	movss		-0(%r9d), %xmm0
	subss		%xmm1, %xmm0
	addss		%xmm2, %xmm0
	return;
min_caml_int_of_float:
	movl		$FLOAT_ZERO, %r11d
	movss		-0(%r11d), %xmm1
	comiss		%xmm0, %xmm1
	jb		FTOI_MAIN
	comiss		%xmm0, %xmm1
	je		FTOI_MAIN
	movd		%xmm0, %eax
	xorl		$-2147483648, %eax
	movd		%eax, %xmm0
	FTOI_MAIN();
	reg[3] = reg[0]-reg[3];
	return;
FTOI_MAIN:
	min_caml_floor();
	movl		$FLOAT_MAGICF, %r13d
	movss		-0(%r13d), %xmm2
	movl		$FLOAT_MAGICFHX, %r13d
	movl		-0(%r13d), %r13d
	comiss		%xmm0, %xmm2
	jb		FTOI_BIG
	comiss		%xmm0, %xmm2
	je		FTOI_BIG
	addss		%xmm2, %xmm0
	movss		%xmm0, -0(%r9d)
	movl		-0(%r9d), %r11d
	reg[3] = reg[3]-reg[4];
	return;
FTOI_BIG:
	movl		$FLOAT_MAGICI, %r14d
	movl		-0(%r14d), %r14d
	reg[3] = reg[0];
FTOI_LOOP:
	subss		%xmm2, %xmm0
	reg[3] = reg[3]+reg[5];
	comiss		%xmm0, %xmm2
	jb		FTOI_LOOP
	comiss		%xmm0, %xmm2
	je		FTOI_LOOP
	addss		%xmm2, %xmm0
	movss		%xmm0, -0(%r9d)
	movl		-0(%r9d), %r14d
	reg[5] = reg[5]-reg[4];
	reg[3] = reg[5]+reg[3];
	return;
min_caml_truncate:
	goto min_caml_int_of_float;
min_caml_read_int:
	reg[3] = reg[0]+0U;
	reg[4] = getchar()&255U;
	reg[3] = reg[3]+reg[4];
	reg[3] = reg[3]<<8U;
	reg[4] = getchar()&255U;
	reg[3] = reg[3]+reg[4];
	reg[3] = reg[3]<<8U;
	reg[4] = getchar()&255U;
	reg[3] = reg[3]+reg[4];
	reg[3] = reg[3]<<8U;
	reg[4] = getchar()&255U;
	reg[3] = reg[3]+reg[4];
	return;
min_caml_read_float:
	min_caml_read_int();
	movl		%r11d, -0(%r9d)
	movss		-0(%r9d), %xmm0
	return;
min_caml_start:
	reg[31] = reg[1];
	reg[31] = reg[1]-32U;
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+4294967295U;
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+0U;
	movl		%r12d, -28(%r10d)
	reg[31] = reg[31]-4U;
	reg[31] = reg[1]-4U;
	min_caml_create_array();
	reg[31] = reg[1]+4U;
	movl		-28(%r10d), %r12d
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+0U;
	movl		%r12d, -28(%r10d)
	reg[31] = reg[31]-8U;
	reg[31] = reg[1]-4U;
	min_caml_create_array();
	reg[31] = reg[1]+4U;
	movl		-28(%r10d), %r12d
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+0U;
	movl		%r12d, -28(%r10d)
	reg[31] = reg[31]-12U;
	reg[31] = reg[1]-4U;
	min_caml_create_array();
	reg[31] = reg[1]+4U;
	movl		-28(%r10d), %r12d
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+0U;
	movl		%r12d, -28(%r10d)
	reg[31] = reg[31]-16U;
	reg[31] = reg[1]-4U;
	min_caml_create_array();
	reg[31] = reg[1]+4U;
	movl		-28(%r10d), %r12d
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+1U;
	movl		%r12d, -28(%r10d)
	reg[31] = reg[31]-20U;
	reg[31] = reg[1]-4U;
	min_caml_create_array();
	reg[31] = reg[1]+4U;
	movl		-28(%r10d), %r12d
	reg[31] = reg[0]+1U;
	reg[31] = reg[0]+0U;
	movl		%r12d, -28(%r10d)
	reg[31] = reg[31]-24U;
	reg[31] = reg[1]-4U;
	min_caml_create_array();
	reg[31] = reg[1]+4U;
	movl		-28(%r10d), %r12d
	reg[31] = reg[0]+30U;
	reg[31] = reg[1]-4U;
	fib.342();
	reg[31] = reg[1]+4U;
	reg[8] = reg[3];
	reg[8] = reg[1]-4U;
	print_int.340();
	reg[8] = reg[1]+4U;
	return 0;
div_binary_search.335:
	reg[3] = reg[5]+reg[6];
	reg[3] = reg[3]>>1U;
	reg[9] = reg[4]*reg[7];
	reg[3] = reg[6]-reg[5];
	if (reg[28]<reg[3]) {
		goto jle_else.694;
	}
	reg[3] = reg[5];
	return;
jle_else.694:
	if (reg[9]<reg[8]) {
		goto jle_else.695;
	}
	if (reg[9]!=reg[8]) {
		goto jeq_else.696;
	}
	reg[3] = reg[4];
	return;
jeq_else.696:
	reg[6] = reg[4];
	goto div_binary_search.335;
jle_else.695:
	reg[5] = reg[4];
	goto div_binary_search.335;
print_int.340:
	if (reg[8]<reg[0]) {
		goto jge_else.697;
	}
	reg[7] = 99942400U|(reg[7]&65535U);
	reg[7] = (reg[7]&4294901760U)|(57600U&65535U);
	reg[5] = reg[0]+0U;
	reg[5] = reg[0]+3U;
	movl		%ebp, -0(%r9d)
	reg[5] = reg[1]-8U;
	div_binary_search.335();
	reg[5] = reg[1]+8U;
	reg[4] = 99942400U|(reg[4]&65535U);
	reg[4] = (reg[4]&4294901760U)|(57600U&65535U);
	reg[4] = reg[3]*reg[4];
	movl		-0(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.698;
	}
	reg[8] = reg[0]+0U;
	goto jle_cont.699;
jle_else.698:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.699:
	reg[7] = 9961472U|(reg[7]&65535U);
	reg[7] = (reg[7]&4294901760U)|(38528U&65535U);
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -4(%r9d)
	reg[3] = reg[1]-12U;
	div_binary_search.335();
	reg[3] = reg[1]+12U;
	reg[4] = 9961472U|(reg[4]&65535U);
	reg[4] = (reg[4]&4294901760U)|(38528U&65535U);
	reg[4] = reg[3]*reg[4];
	movl		-4(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.700;
	}
	if (reg[10]!=reg[0]) {
		goto jeq_else.702;
	}
	reg[8] = reg[0]+0U;
	goto jeq_cont.703;
jeq_else.702:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.703:
	goto jle_cont.701;
jle_else.700:
	reg[3] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.701:
	reg[7] = 983040U|(reg[7]&65535U);
	reg[7] = (reg[7]&4294901760U)|(16960U&65535U);
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -8(%r9d)
	reg[3] = reg[1]-16U;
	div_binary_search.335();
	reg[3] = reg[1]+16U;
	reg[4] = 983040U|(reg[4]&65535U);
	reg[4] = (reg[4]&4294901760U)|(16960U&65535U);
	reg[4] = reg[3]*reg[4];
	movl		-8(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.704;
	}
	if (reg[11]!=reg[0]) {
		goto jeq_else.706;
	}
	reg[8] = reg[0]+0U;
	goto jeq_cont.707;
jeq_else.706:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.707:
	goto jle_cont.705;
jle_else.704:
	reg[3] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.705:
	reg[7] = 65536U|(reg[7]&65535U);
	reg[7] = (reg[7]&4294901760U)|(34464U&65535U);
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -12(%r9d)
	reg[3] = reg[1]-20U;
	div_binary_search.335();
	reg[3] = reg[1]+20U;
	reg[4] = 65536U|(reg[4]&65535U);
	reg[4] = (reg[4]&4294901760U)|(34464U&65535U);
	reg[4] = reg[3]*reg[4];
	movl		-12(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.708;
	}
	if (reg[10]!=reg[0]) {
		goto jeq_else.710;
	}
	reg[8] = reg[0]+0U;
	goto jeq_cont.711;
jeq_else.710:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.711:
	goto jle_cont.709;
jle_else.708:
	reg[3] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.709:
	reg[3] = reg[0]+10000U;
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -16(%r9d)
	reg[3] = reg[1]-24U;
	div_binary_search.335();
	reg[3] = reg[1]+24U;
	reg[3] = reg[0]+10000U;
	reg[4] = reg[3]*reg[4];
	movl		-16(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.712;
	}
	if (reg[11]!=reg[0]) {
		goto jeq_else.714;
	}
	reg[8] = reg[0]+0U;
	goto jeq_cont.715;
jeq_else.714:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.715:
	goto jle_cont.713;
jle_else.712:
	reg[3] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.713:
	reg[3] = reg[0]+1000U;
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -20(%r9d)
	reg[3] = reg[1]-28U;
	div_binary_search.335();
	reg[3] = reg[1]+28U;
	reg[3] = reg[3]*1000U;
	movl		-20(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.716;
	}
	if (reg[10]!=reg[0]) {
		goto jeq_else.718;
	}
	reg[8] = reg[0]+0U;
	goto jeq_cont.719;
jeq_else.718:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.719:
	goto jle_cont.717;
jle_else.716:
	reg[3] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.717:
	reg[3] = reg[0]+100U;
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -24(%r9d)
	reg[3] = reg[1]-32U;
	div_binary_search.335();
	reg[3] = reg[1]+32U;
	reg[3] = reg[3]*100U;
	movl		-24(%r9d), %ebp
	reg[8] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.720;
	}
	if (reg[11]!=reg[0]) {
		goto jeq_else.722;
	}
	reg[8] = reg[0]+0U;
	goto jeq_cont.723;
jeq_else.722:
	reg[8] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.723:
	goto jle_cont.721;
jle_else.720:
	reg[3] = reg[0]+48U;
	reg[3] = reg[4]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.721:
	reg[3] = reg[0]+10U;
	reg[3] = reg[0]+0U;
	reg[3] = reg[0]+10U;
	movl		%ebp, -28(%r9d)
	reg[3] = reg[1]-36U;
	div_binary_search.335();
	reg[3] = reg[1]+36U;
	reg[3] = reg[3]*10U;
	movl		-28(%r9d), %ebp
	reg[4] = reg[8]-reg[4];
	if (reg[0]<reg[3]) {
		goto jle_else.724;
	}
	if (reg[10]!=reg[0]) {
		goto jeq_else.726;
	}
	reg[4] = reg[0]+0U;
	goto jeq_cont.727;
jeq_else.726:
	reg[4] = reg[0]+48U;
	reg[3] = reg[5]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jeq_cont.727:
	goto jle_cont.725;
jle_else.724:
	reg[3] = reg[0]+48U;
	reg[3] = reg[5]+reg[3];
	putchar(reg[3]&255U);
	reg[3] = reg[0]+1U;
jle_cont.725:
	reg[3] = reg[0]+48U;
	reg[3] = reg[3]+reg[4];
	putchar(reg[3]&255U);
	return;
jge_else.697:
	reg[3] = reg[0]+45U;
	putchar(reg[3]&255U);
	reg[8] = reg[0]-reg[8];
	goto print_int.340;
fib.342:
	if (reg[28]<reg[4]) {
		goto jle_else.728;
	}
	reg[3] = reg[4];
	return;
jle_else.728:
	reg[3] = reg[4]-1U;
	movl		%r13d, -0(%r9d)
	reg[4] = reg[3];
	reg[4] = reg[1]-8U;
	fib.342();
	reg[4] = reg[1]+8U;
	reg[5] = reg[3];
	movl		-0(%r9d), %r13d
	reg[5] = reg[4]-2U;
	movl		%r14d, -4(%r9d)
	reg[5] = reg[1]-12U;
	fib.342();
	reg[5] = reg[1]+12U;
	movl		-4(%r9d), %r14d
	reg[3] = reg[5]+reg[3];
	return;

_start:
	xorq	%rbx, %rbx
	xorq	%rcx, %rcx
	xorq	%rsi, %rsi
	xorq	%rdi, %rdi
	xorq	%rbp, %rbp
	xorq	%r8, %r8
	xorq	%r9, %r9
	xorq	%r10, %r10
	xorq	%r11, %r11
	xorq	%r12, %r12
	xorq	%r13, %r13
	xorq	%r14, %r14
	xorq	%r15, %r15
	movl	$BOTTOM, %r9d
	movl	$TOP, %r12d
	movl	$BOTTOM, %r10d
	call	min_caml_start

}
