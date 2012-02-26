
#ifndef _ASM_HEAD
#define _ASM_HEAD
#include "common.h"
#define	COL_MAX 256
#define LINE_MAX 32768



#define ASM_LOG "asmlog"
#define LABEL_MAX (128 * 1024)
#define DATA_NUM (1024 * 1024)

#define PROTO_R(name) \
	uint32_t name(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
#define PROTO_I(name) \
	uint32_t name(uint8_t, uint8_t, uint16_t);
#define PROTO_J(name) \
	uint32_t name(uint32_t);

#define DEFINE_R(name, opcode) \
	uint32_t name(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shaft, uint8_t funct) {\
		return (opcode << 26 | ((uint32_t)rs << 21) | ((uint32_t) rt << 16)\
				| ((uint32_t) rd << 11) | ((uint32_t) shaft << 6) |funct);\
	}

#define DEFINE_I(name, opcode) \
	uint32_t name(uint8_t rs, uint8_t rt, uint16_t imm) {\
		return (opcode << 26 | ((uint32_t)rs << 21) | ((uint32_t) rt << 16) | imm);\
	}
#define DEFINE_J(name, opcode) \
	uint32_t name(uint32_t target) {\
		return (opcode << 26 | target);\
	}

#define PROTO_F(name)\
	uint32_t name(uint8_t, uint8_t, uint8_t, uint8_t);

#define DEFINE_F(name, opcode, funct) \
	uint32_t name(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shaft) {\
		return (opcode << 26 | ((uint32_t)rs << 21) | ((uint32_t) rt << 16)\
				| ((uint32_t) rd << 11) | ((uint32_t) shaft << 6) |funct);\
	}

PROTO_I(mvhi);
PROTO_I(mvlo);
PROTO_I(addi);
PROTO_I(subi);
PROTO_I(muli);
PROTO_I(divi);
PROTO_I(slli);
PROTO_I(srli);
PROTO_J(jmp);
PROTO_I(jeq);
PROTO_I(jne);
PROTO_I(jlt);
PROTO_I(jle);
PROTO_J(call);
PROTO_J(_return);
PROTO_I(ldi);
PROTO_I(sti);
PROTO_I(ldlr);
PROTO_I(stlr);

PROTO_I(fldi);
PROTO_I(fsti);
PROTO_I(setl);
PROTO_I(fjeq);
PROTO_I(fjlt);

//PROTO_R(_sin);
//PROTO_R(_cos);
//PROTO_R(_atan);
//PROTO_R(_sqrt);
//PROTO_R(_int_of_float);
//PROTO_R(_float_of_int);
PROTO_F(mov);
PROTO_F(_not);
PROTO_F(input);
PROTO_F(inputw);
PROTO_F(inputf);
PROTO_F(output);
PROTO_F(outputw);
PROTO_F(outputf);
PROTO_F(nop);
PROTO_F(sll);
PROTO_F(srl);
PROTO_F(b);
PROTO_F(btmplr);
PROTO_F(nor);
PROTO_F(add);
PROTO_F(sub);
PROTO_F(ld);
PROTO_F(st);
PROTO_F(fld);
PROTO_F(fst);
PROTO_F(movlr);
PROTO_F(mul);
PROTO_I(padd);
//PROTO_F(padd);
PROTO_F(_div);
PROTO_F(_and);
PROTO_F(_or);
PROTO_F(halt);
PROTO_F(callr);

PROTO_F(fadd);
PROTO_F(fsub);
PROTO_F(fmul);
PROTO_F(fdiv);
PROTO_F(fsqrt);
PROTO_F(_fabs);
PROTO_F(fmov);
PROTO_F(fneg);

PROTO_I(_link);

#define asm_fmt_if "%s %%f%d"
#define asm_fmt_ig "%s %%g%d"
#define asm_fmt_ii "%s %d"
#define asm_fmt_il "%s %s"
#define asm_fmt_iff "%s %%f%d, %%f%d"
#define asm_fmt_ifg "%s %%f%d, %%g%d"
#define asm_fmt_igf "%s %%g%d, %%f%d"
#define asm_fmt_igg "%s %%g%d, %%g%d"
#define asm_fmt_igi "%s %%g%d, %d"
#define asm_fmt_igl "%s %%g%d, %s"
#define asm_fmt_ifff "%s %%f%d, %%f%d, %%f%d"
#define asm_fmt_iffl "%s %%f%d, %%f%d, %s"
#define asm_fmt_ifgg "%s %%f%d, %%g%d, %%g%d"
#define asm_fmt_ifgi "%s %%f%d, %%g%d, %d"
#define asm_fmt_iggg "%s %%g%d, %%g%d, %%g%d"
#define asm_fmt_iggi "%s %%g%d, %%g%d, %d"
#define asm_fmt_iggl "%s %%g%d, %%g%d, %s"

#define STDOUT stdout
#define STDERR stderr
#define warning(fmt, ...) \
	fprintf(STDERR, fmt, ##__VA_ARGS__)
void *mygets(char *dst, char *src, int n);

typedef struct label_t {
	char name[COL_MAX];
	int len;
	int line;
} label_t;
int hash_insert(label_t label);
int hash_find(label_t label);
uint32_t encode_op(char *asm_line, char *inst);
extern inline uint32_t eff_dig(unsigned dig, uint32_t num);
extern inline uint32_t shift_left_l(unsigned shiftwidth, uint32_t num);

#endif
