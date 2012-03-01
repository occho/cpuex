#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "oc_geso.h"

static void print_gr(int,int,int);
static void print_fr(int,int);
static char *gr_label(int,int,int);
static char *fr_label(int);
#define grl(idx) gr_label(idx, 0, 0)
#define grlq(idx) gr_label(idx, 0, 1)
#define grldst(idx) gr_label(idx, 1, 0)
#define frl(idx) fr_label(idx)


#define print_line(inst, fmt, ...) \
	myprint("\t"#inst"\t\t"fmt"\n", ##__VA_ARGS__)
#define myscan(fmt, ...) \
	(sscanf(asm_line, asm_fmt_##fmt, tmp, ##__VA_ARGS__) - 1)
#define COMMA ", "
#define GR "%s"
#define GRc "%s"COMMA
#define FR "%s"
#define FRc "%s"COMMA
#define IMM "$%d"
#define IMMc "$%d"COMMA
#define TMP "%%eax"
#define TMPc "%%eax"COMMA
#define LABEL "%s"
static inline int _inst_is(char *inst, const char *str) {
	return strcmp(inst, str) == 0;
}
#define inst_is(str) _inst_is(term0, str)
int convert_op(char *asm_line, char *term0)
{
	static int rd,rs,rt,imm;
	static char tmp[256];
	static char lname[256];

	if (inst_is("mov")) {
		if (myscan(igg, &rd, &rs) == 2) {
	 		if (is_xreg(rs) || is_xreg(rd)) {
				print_line(movl, GRc GR, grl(rs), grldst(rd));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}
	if (inst_is("mvhi")) {
		if (myscan(igi, &rs, &imm) == 2) {
			print_line(andl, IMMc GR, 0xffff, grldst(rs));
			print_line(orl, IMMc GR, ((imm&0xffff)<<16), grldst(rs));
		    return 0;
		}
	}
	if(inst_is("mvlo")) {
		if(myscan(igi, &rs, &imm) == 2) {
			print_line(andl, IMMc GR, 0xffff0000, grldst(rs));
			print_line(orl, IMMc GR, (imm&0xffff), grldst(rs));
		    return 0;
		}
	}
	if(inst_is("add")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(addl, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(addl, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}
	if(inst_is("sub")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(subl, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(subl, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}
	if(inst_is("mul")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(imull, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(imull, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}

	if(inst_is("addi")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
			if (rs == 0) {
				print_line(movl, IMMc GR, imm, grldst(rt));
	 		} else if ((rt == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(addl, IMMc GR, imm, grldst(rt));
			} else if (is_xreg(rs) || is_xreg(rt)) {
				print_line(movl, GRc GR, grl(rs), grldst(rt));
				print_line(addl, IMMc GR, imm, grldst(rt));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(addl, IMMc TMP, imm);
				print_line(movl, TMPc GR, grldst(rt));
			}

		    return 0;
		}
	}
	if(inst_is("subi")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {

	 		if ((rt == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(subl, IMMc GR, imm, grldst(rt));
			} else if (is_xreg(rs) || is_xreg(rt)) {
				print_line(movl, GRc GR, grl(rs), grldst(rt));
				print_line(subl, IMMc GR, imm, grldst(rt));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(subl, IMMc TMP, imm);
				print_line(movl, TMPc GR, grldst(rt));
			}

		    return 0;
		}
	}
	if(inst_is("muli")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
	 		if ((rt == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(imull, IMMc GR, imm, grldst(rt));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(imull, IMMc TMP, imm);
				print_line(movl, TMPc GR, grldst(rt));
			}
		    return 0;
		}
	}
	if (inst_is("input")) {
		if (myscan(ig, &rd) == 1) {
			print_line(xorl, TMPc TMP);
			print_line(call, LABEL, "InChar");
			print_line(movl, TMPc GR, grldst(rd));

		    return 0;
		}
	}
	if (inst_is("output")) {
		if (myscan(ig, &rs) == 1) {
			print_line(movl, GRc LABEL, grl(rs), "%eax");
			print_line(call, LABEL, "OutChar");
			return 0;
		}
	}
	if (inst_is("outputH")) {
		if (myscan(ig, &rs) == 1) {
			print_line(movl, GRc LABEL, grl(rs), "%eax");
			print_line(call, LABEL, "PrintHex8");
		    return 0;
		}
	}

	if (inst_is("outputF")) {
		if (myscan(if, &rs) == 1) {
			print_line(movl, FRc LABEL, frl(rs), "%eax");
			print_line(call, LABEL, "PrintHex8");
		    return 0;
		}
	}

	if (inst_is("and")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(andl, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(andl, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}

	if (inst_is("or")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(orl, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(orl, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}
	if (inst_is("not")) {
		if (myscan(igg, &rd, &rs) == 2) {

			if (rd == rs) {
				print_line(notl, GR, grl(rd));
	 		} else if (is_xreg(rs) || is_xreg(rd)) {
				print_line(movl, GRc GR, grl(rs), grldst(rd));
				print_line(notl, GR, grl(rd));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(notl, TMP);
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}
	if(inst_is("sll")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(shll, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(shll, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}
	if(inst_is("srl")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(shrl, GRc GR, grl(rt), grldst(rs));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(shrl, GRc TMP, grl(rt));
				print_line(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}

	if(inst_is("slli")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {

	 		if ((rt == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(shll, IMMc GR, imm, grldst(rt));
			} else if (is_xreg(rs) || is_xreg(rt)) {
				print_line(movl, GRc GR, grl(rs), grldst(rt));
				print_line(shll, IMMc GR, imm, grldst(rt));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(shll, IMMc TMP, imm);
				print_line(movl, TMPc GR, grldst(rt));
			}

		    return 0;
		}
	}
	if(inst_is("srli")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {

	 		if ((rt == rs) && (is_xreg(rs) || is_xreg(rt))) {
				print_line(shrl, IMMc GR, imm, grldst(rt));
			} else if (is_xreg(rs) || is_xreg(rt)) {
				print_line(movl, GRc GR, grl(rs), grldst(rt));
				print_line(shrl, IMMc GR, imm, grldst(rt));
			} else {
				print_line(movl, GRc TMP, grl(rs));
				print_line(shrl, IMMc TMP, imm);
				print_line(movl, TMPc GR, grldst(rt));
			}

		    return 0;
		}
	}
	if (inst_is("b")) {
		if (myscan(ig, &rs) == 1) {
			if (is_xreg(rs)) {
				print_line(jmp *, GR, grlq(rs));
			} else {
				print_line(movl, GRc LABEL, grl(rs), "%edx");
				print_line(jmp *, LABEL, "%rdx");
			}
		    return 0;
		}
	}
	if(strcmp(term0, "jmp") == 0){
		if(sscanf(asm_line, asm_fmt_il, tmp, lname) == 2) { 
			if (strcmp(lname, "min_caml_start") != 0) {
				OP(jmp),L(lname),NL;
				return 0;
			} else {
				return 0;
			}
		}
	}
	if(strcmp(term0, "jeq") == 0){
		if(sscanf(asm_line, asm_fmt_iggl, tmp, &rs, &rt, lname) == 4) { 
			OP(movl),G(rs),SC(%edx),NL;
			OP(cmpl),G(rt),SC(%edx),NL;
			OP(je),L(lname),NL;
		    return 0;
		}
	}
	if(strcmp(term0, "jne") == 0){
		if(sscanf(asm_line, asm_fmt_iggl, tmp, &rs, &rt, lname) == 4) { 
			if (is_xreg(rt) || is_xreg(rs)) {
				OP(cmpl),G(rt),GC(rs),NL;
				OP(jne),L(lname),NL;
			} else {
				OP(movl),G(rs),SC(%edx),NL;
				OP(cmpl),G(rt),SC(%edx),NL;
				OP(jne),L(lname),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "jlt") == 0){
		if(sscanf(asm_line, asm_fmt_iggl, tmp, &rs, &rt, lname) == 4) { 
			
			if (is_xreg(rt) || is_xreg(rs)) {
				if (is_const(rs)) {
					OP(cmpl),G(rs),GC(rt),NL;
					OP(jg),L(lname),NL;
				} else {
					OP(cmpl),G(rt),GC(rs),NL;
					OP(jl),L(lname),NL;
				}
			} else {
				OP(movl),G(rs),SC(%eax),NL;
				OP(cmpl),G(rt),SC(%eax),NL;
				OP(jl),L(lname),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "jle") == 0){
		if(sscanf(asm_line, asm_fmt_iggl, tmp, &rs, &rt, lname) == 4) { 
			OP(movl),G(rs),SC(%eax),NL;
			OP(cmp),G(rt),SC(%eax),NL;
			OP(jle),L(lname),NL;
		    return 0;
		}
	}
	if(strcmp(term0, "call") == 0){
		if(sscanf(asm_line, asm_fmt_il, tmp, lname) == 2)  { 
			OP(call),L(lname),NL;
		    return 0;
		}
	}
	if(strcmp(term0, "callR") == 0){
		if(sscanf(asm_line, asm_fmt_ig, tmp, &rs) == 2) {
			if (is_xreg(rs)) {
				OP(call *),GQ(rs),NL;
			} else {
				OP(movl),G(rs),SC(%edx),NL;
				OP(call *),S(%rdx),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "return") == 0){
			OP(ret),NL;
		    return 0;
	}

	if(strcmp(term0, "ldi") == 0){
		if(sscanf(asm_line, asm_fmt_iggi, tmp, &rt, &rs, &imm) == 4) {

			if (is_xreg(rs) && is_xreg(rt)) {
				OP(movl),
				myprint("-%d(",imm),G(rs),myprint(")"),
				GC(rt),NL;
			} else if (is_xreg(rt)) {
				OP(movl),G(rs),SC(%edx),NL;
				OP(movl),ADR(%edx,imm),GC(rt),NL;
			} else {
				OP(movl),G(rs),SC(%edx),NL;
				OP(movl),ADR(%edx,imm),SC(%eax),NL;
				OP(movl),S(%eax),GC(rt),NL;
			}

		    return 0;
		}
	}

	if(strcmp(term0, "ld") == 0){
		if(sscanf(asm_line, asm_fmt_iggg, tmp, &rd, &rs, &rt) == 4) {
			OP(movl),G(rs),SC(%edx),NL;
			OP(movl),G(rt),SC(%eax),NL;
			OP(movl),myprint("0(%%eax, %%edx, 1)"),SC(%eax),NL;
			OP(movl),S(%eax),GC(rd),NL;
		    return 0;
		}
	}
	if(strcmp(term0, "st") == 0){
		if(sscanf(asm_line, asm_fmt_iggg, tmp, &rd, &rs, &rt) == 4) {
			OP(push),S(%rbx),NL;
			OP(movl),G(rt),SC(%edx),NL;
			OP(movl),G(rs),SC(%eax),NL;
			OP(movl),G(rd),SC(%ebx),NL;
			OP(movl),S(%ebx),myprint(", 0(%%eax, %%edx, 1)"),NL;
			OP(pop), S(%rbx),NL;
		    return 0;
		}
	}

	if(strcmp(term0, "sti") == 0){
		if(sscanf(asm_line, asm_fmt_iggi, tmp, &rt, &rs, &imm) == 4) {

			if (is_xreg(rs) && is_xreg(rt)) {
				OP(movl),G(rt),
				myprint(", -%d(",imm),G(rs),myprint(")\n");
			} else if (is_xreg(rt)) {
				OP(movl),G(rs),SC(%eax),NL;
				OP(movl),G(rt),ADRC(%eax,imm),NL;
			} else {
				OP(movl),G(rs),SC(%eax),NL;
				OP(movl),G(rt),SC(%edx),NL;
				OP(movl),S(%edx),ADRC(%eax,imm),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fadd") == 0){
		if(sscanf(asm_line, asm_fmt_ifff, tmp, &rd, &rs, &rt) == 4) {

	 		if ((rd == rs) && (is_xmm(rs))) {
				OP(addss),F(rt),FC(rs),NL;
			} else if ((rd == rt) && (is_xmm(rd))) {
				OP(addss),F(rs),FC(rd),NL;
			} else {
				OP(movss),F(rs),SC(%xmm15),NL;
				OP(addss),F(rt),SC(%xmm15),NL;
				OP(movss),S(%xmm15),FC(rd),NL;
			}

		    return 0;
		}
	}
	if(strcmp(term0, "fsub") == 0){
		if(sscanf(asm_line, asm_fmt_ifff, tmp, &rd, &rs, &rt) == 4) {
	 		if ((rd == rs) && (is_xmm(rs))) {
				OP(subss),F(rt),FC(rs),NL;
			} else {
				OP(movss),F(rs),SC(%xmm15),NL;
				OP(subss),F(rt),SC(%xmm15),NL;
				OP(movss),S(%xmm15),FC(rd),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fmul") == 0){
		if(sscanf(asm_line, asm_fmt_ifff, tmp, &rd, &rs, &rt) == 4) {
	 		if ((rd == rs) && (is_xmm(rs))) {
				OP(mulss),F(rt),FC(rs),NL;
			} else if ((rd == rt) && (is_xmm(rd))) {
				OP(mulss),F(rs),FC(rd),NL;
			} else {
				OP(movss),F(rs),SC(%xmm15),NL;
				OP(mulss),F(rt),SC(%xmm15),NL;
				OP(movss),S(%xmm15),FC(rd),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fdiv") == 0){
		if(sscanf(asm_line, asm_fmt_ifff, tmp, &rd, &rs, &rt) == 4) {
			if (mathlib_flag) {
				if (is_xmm(rt)) {
					OP(movd),F(rt),SC(%eax),NL;
				} else {
					OP(movl),F(rt),SC(%eax),NL;
				}
				OP(call),S(FInv),NL;
				OP(movd),S(%eax),SC(%xmm15),NL;
				OP(mulss),F(rs),SC(%xmm15),NL;
				OP(movss),S(%xmm15),FC(rd),NL;

			} else {
				if ((rd == rs) && (is_xmm(rs))) {
					OP(divss),F(rt),FC(rs),NL;
				} else {
					OP(movss),F(rs),SC(%xmm15),NL;
					OP(divss),F(rt),SC(%xmm15),NL;
					OP(movss),S(%xmm15),FC(rd),NL;
				}
			}


		    return 0;
		}
	}
	if(strcmp(term0, "fsqrt") == 0){
		if(sscanf(asm_line, asm_fmt_iff, tmp, &rd, &rs) == 3) {
			if (mathlib_flag) {

				if (is_xmm(rs)) {
					OP(movd),F(rs),SC(%eax),NL;
				} else {
					OP(movl),F(rs),SC(%eax),NL;
				}
				OP(call),S(FSqrt),NL;
				if (is_xmm(rd)) {
					OP(movd),S(%eax),FC(rd),NL;
				} else {
					OP(movl),S(%eax),FC(rd),NL;
				}

			} else {
				if (is_xmm(rd)) {
					OP(sqrtss),F(rs),FC(rd),NL;
				} else {
					OP(movss),F(rs),SC(%xmm15),NL;
					OP(sqrtss),S(%xmm15),SC(%xmm15),NL;
					OP(movss),S(%xmm15),FC(rd),NL;
				}
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fabs") == 0){
		if(sscanf(asm_line, asm_fmt_iff, tmp, &rd, &rs) == 3) {
			OP(movss),F(rs),SC(%eax),NL;
			OP(andl),IM(0x7fffffff),SC(%eax),NL;
			OP(movss),S(%eax),FC(rd),NL;
		    return 0;
		}
	}
	if(strcmp(term0, "fmov") == 0){
		if(sscanf(asm_line, asm_fmt_iff, tmp, &rd, &rs) == 3) {
			if (is_xmm(rd)) {
				OP(movss),F(rs),FC(rd),NL;
			} else {
				if (is_xmm(rs)) {
					OP(movd),F(rs),SC(%eax),NL;
				} else {
					OP(movl),F(rs),SC(%eax),NL;
				}
				OP(movl),S(%eax),FC(rd),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fneg") == 0){
		if(sscanf(asm_line, asm_fmt_iff, tmp, &rd, &rs) == 3) {
			if (is_xmm(rs)) {
				OP(movd),F(rs),SC(%eax),NL;
			} else {
				OP(movl),F(rs),SC(%eax),NL;
			}
			OP(xorl),IM(1<<31),SC(%eax),NL;
			if (is_xmm(rd)) {
				OP(movd),S(%eax),FC(rd),NL;
			} else {
				OP(movl),S(%eax),FC(rd),NL;
			}
		    return 0;
		}
	}

	if(strcmp(term0, "fld") == 0){
		if(sscanf(asm_line, asm_fmt_ifgg, tmp, &rd, &rs, &rt) == 4) {
			OP(movl),G(rs),SC(%edx),NL;
			OP(movl),G(rt),SC(%eax),NL;
			OP(movl),myprint("0(%%eax, %%edx, 1)"),SC(%eax),NL;
			if (is_xmm(rd)) {
				OP(movd),S(%eax),FC(rd),NL;
			} else {
				OP(movl),S(%eax),FC(rd),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fst") == 0){
		if(sscanf(asm_line, asm_fmt_ifgg, tmp, &rd, &rs, &rt) == 4) {
			OP(push),S(%rbx),NL;
			OP(movl),G(rt),SC(%edx),NL;
			OP(movl),G(rs),SC(%eax),NL;
			if (is_xmm(rd)) {
				OP(movd),F(rd),SC(%ebx),NL;
			} else {
				OP(movl),F(rd),SC(%ebx),NL;
			}
			OP(movl),S(%ebx),myprint(", 0(%%eax, %%edx, 1)"),NL;
			OP(pop), S(%rbx),NL;
		    return 0;
		}
	}
	if(strcmp(term0, "fldi") == 0){
		if(sscanf(asm_line, asm_fmt_ifgi, tmp, &rt, &rs, &imm) == 4) {
			if (is_xmm(rt)) {
				if (is_xreg(rs)) {
					OP(movss),
					myprint("-%d(",imm),G(rs),myprint(")"),
					FC(rt),NL;
				} else {
					OP(movl),G(rs),SC(%edx),NL;
					OP(movss),ADR(%edx,imm), FC(rt),NL;
				}
			} else {
				OP(movl),G(rs),SC(%edx),NL;
				OP(movl),ADR(%edx,imm), SC(%eax),NL;
				OP(movl),S(%eax),FC(rt),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fsti") == 0){
		if(sscanf(asm_line, asm_fmt_ifgi, tmp, &rt, &rs, &imm) == 4) {
			if (is_xmm(rt)) {
				if (is_xreg(rs)) {
					OP(movss),F(rt),
					myprint(", -%d(",imm),G(rs),myprint(")\n");
				} else {
					OP(movl),G(rs),SC(%eax),NL;
					OP(movss),F(rt),ADRC(%eax,imm),NL;
				}
			} else {
				OP(movl),F(rt),SC(%edx),NL;
				OP(movl),G(rs),SC(%eax),NL;
				OP(movl),S(%edx),ADRC(%eax,imm),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fjeq") == 0){
		if(sscanf(asm_line, asm_fmt_iffl, tmp, &rs, &rt, lname) == 4) {
			if (is_xmm(rs) && is_xmm(rt)) {
				OP(comiss),F(rt),FC(rs),NL;
				OP(je),L(lname),NL;
			} else if (is_xmm(rs)) {
				OP(movss), F(rt),SC(%xmm15),NL;
				OP(comiss),S(%xmm15),FC(rs),NL;
				OP(je),L(lname),NL;
			} else if (is_xmm(rt)) {
				OP(movss), F(rs),SC(%xmm15),NL;
				OP(comiss),F(rt),SC(%xmm15),NL;
				OP(je),L(lname),NL;
			} else {
				OP(flds),F(rt),NL;
				OP(flds),F(rs),NL;
				OP(fcompp),NL;
				OP(fnstsw),S(%ax),NL;
				OP(sahf),NL;
				OP(je),L(lname),NL;
			}
		    return 0;
		}
	}
	if(strcmp(term0, "fjlt") == 0){
		if(sscanf(asm_line, asm_fmt_iffl, tmp, &rs, &rt, lname) == 4) {

			if (is_xmm(rs) && is_xmm(rt)) {
				OP(comiss),F(rt),FC(rs),NL;
				OP(jb),L(lname),NL;
			} else if (is_xmm(rs)) {
				OP(movss), F(rt),SC(%xmm15),NL;
				OP(comiss),S(%xmm15),FC(rs),NL;
				OP(jb),L(lname),NL;
			} else if (is_xmm(rt)) {
				OP(movss), F(rs),SC(%xmm15),NL;
				OP(comiss),F(rt),SC(%xmm15),NL;
				OP(jb),L(lname),NL;
			} else {
				OP(flds),F(rt),NL;
				OP(flds),F(rs),NL;
				OP(fcompp),NL;
				OP(fnstsw),S(%ax),NL;
				OP(sahf),NL;
				OP(jb),L(lname),NL;
			}

		    return 0;
		}
	}
	if(strcmp(term0, "nop") == 0){
		OP(nop),NL;
		return 0;
	}
	if(strcmp(term0, "halt") == 0){
		if (count_flag) {
			OP(call),S(NewLine),NL;
			OP(movq),S((CNT)),SC(%rax),NL;
			OP(call),S(PrintHex16),NL;
			OP(call),S(NewLine),NL;
		}
		OP(call),S(Exit),NL;
		return 0;
	}
	if(strcmp(term0, "setL") == 0){
		if(sscanf(asm_line, asm_fmt_igl, tmp, &rd, lname) == 3) {
			OP(movl),myprint("$%s",lname),GC(rd),NL;
		    return 0;
		}
	}
	return -1;
}

void print_gr(int register_index, int comma_flag, int quad_flag) {
	char *datalen1 = (quad_flag) ? (char *)"" : (char *)"d";
	char *datalen2 = (quad_flag) ? (char *)"r" : (char *)"e";
	if (comma_flag) {
		myprint(", ");
		if (is_const(register_index)) {
			myprint("%%eax");
			return ;
		}
	}
	switch (register_index) {
		case 0 : myprint("$0"); break;
		case 28: myprint("$1"); break;
		case 29: myprint("$-1"); break;
		case 26:
				myprint("%%r8%s", datalen1); break;
		case 1:
				myprint("%%r9%s", datalen1); break;
		case 31:
				myprint("%%r10%s", datalen1); break;
		case 3:
				myprint("%%r11%s", datalen1); break;
		case 2:
				myprint("%%r12%s", datalen1); break;
		case 4:
				myprint("%%r13%s", datalen1); break;
		case 5:
				myprint("%%r14%s", datalen1); break;
		case 9:
				myprint("%%r15%s", datalen1); break;
		case 6:
				myprint("%%%sbx", datalen2); break;
		case 10:
				myprint("%%%scx", datalen2); break;
		case 11:
				myprint("%%%ssi", datalen2); break;
		case  7:
				myprint("%%%sdi", datalen2); break;
		case 8:
				myprint("%%%sbp", datalen2); break;
		default :
				myprint("(GR%d)", register_index); break;
	}
}

void print_fr(int register_index, int comma_flag) {
	if (comma_flag) {
		myprint(", ");
	}
	switch (register_index) {
		case 0 : 
			myprint("%%xmm0"); break;
		case 1 :
			myprint("%%xmm1"); break;
		case 2 :
			myprint("%%xmm2"); break;
		case 12 :
			myprint("%%xmm3"); break;
		case 8 :
			myprint("%%xmm4"); break;
		case 18 :
			myprint("%%xmm5"); break;
		case 3 :
			myprint("%%xmm6"); break;
		case 22 :
			myprint("%%xmm7"); break;
		case 31 :
			myprint("%%xmm8"); break;
		case 4 :
			myprint("%%xmm9"); break;
		case 9 :
			myprint("%%xmm10"); break;
		case 5 :
			myprint("%%xmm11"); break;
		case 16 :
			myprint("%%xmm12"); break;
		case 7 :
			myprint("%%xmm13"); break;
		case 6 :
			myprint("%%xmm14"); break;
		default :
			myprint("(FR%d)", register_index); break;
	}
}

int is_const(int i) {
	return (i == 0 || i == 28 || i == 29);
}

#define XREG_NUM 13
int is_xreg(int i) {
	int j;
	const int a[XREG_NUM] = {3,1,26,7,31,6,9,4,5,10, 2,11,8};
	if (is_const(i)) {
		return 1;
	}
	for (j = 0; j < XREG_NUM; j++) {
		if (i == a[j]) {
			return 1;
		}
	}
	return 0;
}

#define XMM_NUM 15
int is_xmm(int i) {
	int j;
	const int a[XMM_NUM] = {0,1,3,6,2,7,5,4,8, 12,18,22,31,9,16};
	for (j = 0; j < XMM_NUM; j++) {
		if (i == a[j]) {
			return 1;
		}
	}
	return 0;
}

#define set_label(fmt, ...) \
	sprintf(label[label_idx], fmt, ##__VA_ARGS__)
static char *gr_label(int register_index, int dst_flag, int quad_flag) {
	static char label[3][16];
	static int label_idx = 0;
	char *datalen1 = (quad_flag) ? (char *)"" : (char *)"d";
	char *datalen2 = (quad_flag) ? (char *)"r" : (char *)"e";
	label_idx = (label_idx+1)%3;
	if (dst_flag && is_const(register_index)) {
		set_label("%%eax");

	} else {
		switch (register_index) {
			case 0 : set_label("$0"); break;
			case 28: set_label("$1"); break;
			case 29: set_label("$-1"); break;
			case 26:
					set_label("%%r8%s", datalen1); break;
			case 1:
					set_label("%%r9%s", datalen1); break;
			case 31:
					set_label("%%r10%s", datalen1); break;
			case 3:
					set_label("%%r11%s", datalen1); break;
			case 2:
					set_label("%%r12%s", datalen1); break;
			case 4:
					set_label("%%r13%s", datalen1); break;
			case 5:
					set_label("%%r14%s", datalen1); break;
			case 9:
					set_label("%%r15%s", datalen1); break;
			case 6:
					set_label("%%%sbx", datalen2); break;
			case 10:
					set_label("%%%scx", datalen2); break;
			case 11:
					set_label("%%%ssi", datalen2); break;
			case  7:
					set_label("%%%sdi", datalen2); break;
			case 8:
					set_label("%%%sbp", datalen2); break;
			default :
					set_label("(GR%d)", register_index); break;
		}
	}
	return label[label_idx];
}

static char *fr_label(int register_index) {
	static char label[3][16];
	static int label_idx = 0;
	label_idx = (label_idx+1)%3;
	switch (register_index) {
		case 0 : set_label("%%xmm0"); break;
		case 1 : set_label("%%xmm1"); break;
		case 2 : set_label("%%xmm2"); break;
		case 12 : set_label("%%xmm3"); break;
		case 8 : set_label("%%xmm4"); break;
		case 18 : set_label("%%xmm5"); break;
		case 3 : set_label("%%xmm6"); break;
		case 22 : set_label("%%xmm7"); break;
		case 31 : set_label("%%xmm8"); break;
		case 4 : set_label("%%xmm9"); break;
		case 9 : set_label("%%xmm10"); break;
		case 5 : set_label("%%xmm11"); break;
		case 16 : set_label("%%xmm12"); break;
		case 7 : set_label("%%xmm13"); break;
		case 6 : set_label("%%xmm14"); break;
		default : set_label("(FR%d)", register_index); break;
	}
	return label[label_idx];
}


#undef set_label


