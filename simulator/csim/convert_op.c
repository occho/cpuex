#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "oc_geso.h"

static char *gr_label(int,int,int);
static char *fr_label(int);
#define grl(idx) gr_label(idx, 0, 0)
#define grlq(idx) gr_label(idx, 0, 1)
#define grldst(idx) gr_label(idx, 1, 0)
#define frl(idx) fr_label(idx)



#define tmp_print(inst, fmt, ...) \
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
#define FTMP "%%xmm15"
#define FTMPc "%%xmm15"COMMA
#define TMP "%%eax"
#define TMPc "%%eax"COMMA
#define TMP1 "%%edx"
#define TMP1c "%%edx"COMMA
#define LABEL "%s"
#define LABELc "%s"COMMA
#define ADDR(x) "-%d("x")"
#define ADDRc(x) "-%d("x")"COMMA

#define REG "reg[%d]"
#define EQ " = "
#define VBAR "|"
#define AMP "&"
#define PLUS "+"
#define MINUS "-"
#define AST "*"
#define LP "("
#define RP ")"
#define U_IMM "%uU"

#define print_line(fmt, ...) \
	myprint("\t"fmt";\n", ##__VA_ARGS__)
#define print_goto(x) \
	print_line("goto %s", x)
#define print_if(fmt, ...) \
	myprint("\tif "LP fmt RP" {\n\t", ##__VA_ARGS__)

#define COND_JMP(x) \
	print_if(REG #x REG, rs, rt);
#define COND_END \
	myprint("\t}\n")
#define BIN_OPR(x) \
	print_line(REG EQ REG #x REG, rd, rs, rt);
#define BIN_OPI(x) \
	print_line(REG EQ REG #x U_IMM, rd, rs, imm);


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
			print_line(REG EQ REG, rd, rs);
		    return 0;
		}
	}
	if (inst_is("mvhi")) {
		if (myscan(igi, &rs, &imm) == 2) {
			print_line(REG EQ U_IMM VBAR LP REG AMP U_IMM RP,
						rs, (uint32_t)imm<<16, rs, 0xffff);
		    return 0;
		}
	}
	if (inst_is("mvlo")) {
		if (myscan(igi, &rs, &imm) == 2) {
			print_line(REG EQ LP REG AMP U_IMM RP VBAR LP U_IMM AMP U_IMM RP,
						rs, rs, 0xffff<<16, imm, 0xffff);
		    return 0;
		}
	}
	if(inst_is("add")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(+);
		    return 0;
		}
	}
	if(inst_is("sub")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(-);
		    return 0;
		}
	}
	if(inst_is("mul")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(*);
		    return 0;
		}
	}
	if(inst_is("and")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(&);
		    return 0;
		}
	}
	if(inst_is("or")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(|);
		    return 0;
		}
	}
	if(inst_is("sll")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(<<);
		    return 0;
		}
	}
	if(inst_is("sll")) {
		if (myscan(iggg, &rd, &rs, &rt) == 3) {
			BIN_OPR(>>);
		    return 0;
		}
	}
	if(inst_is("addi")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
			BIN_OPI(+);
		    return 0;
		}
	}
	if(inst_is("subi")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
			BIN_OPI(-);
		    return 0;
		}
	}
	if(inst_is("muli")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
			BIN_OPI(*);
		    return 0;
		}
	}
	if(inst_is("slli")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
			BIN_OPI(<<);
		    return 0;
		}
	}
	if(inst_is("srli")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {
			BIN_OPI(>>);
		    return 0;
		}
	}
	if (inst_is("input")) {
		if (myscan(ig, &rd) == 1) {
			print_line(REG EQ "getchar()" AMP U_IMM, 
						rd, 0xff);
		    return 0;
		}
	}
	if (inst_is("output")) {
		if (myscan(ig, &rs) == 1) {
			print_line("putchar" LP REG AMP U_IMM RP, rs, 0xff);
			return 0;
		}
	}
	if (inst_is("b")) {
	/*
		if (myscan(ig, &rs) == 1) {
			if (is_xreg(rs)) {
				tmp_print(jmp *, GR, grlq(rs));
			} else {
				tmp_print(movl, GRc LABEL, grl(rs), "%edx");
				tmp_print(jmp *, LABEL, "%rdx");
			}
		    return 0;
		}
	*/
	}
	if (inst_is("jmp")) {
		if (myscan(il, lname) == 1) {
			print_goto(lname);
			return 0;
		}
	}
	if (inst_is("jeq")) {
		if (myscan(iggl, &rs, &rt, lname) == 3) {
			COND_JMP(==);
			print_goto(lname);
			COND_END;
		    return 0;
		}
	}
	if (inst_is("jne")) {
		if (myscan(iggl, &rs, &rt, lname) == 3) {
			COND_JMP(!=);
			print_goto(lname);
			COND_END;
		    return 0;
		}
	}
	if (inst_is("jlt")) {
		if (myscan(iggl, &rs, &rt, lname) == 3) {
			COND_JMP(<);
			print_goto(lname);
			COND_END;
		    return 0;
		}
	}
	if (inst_is("jle")) {
		if (myscan(iggl, &rs, &rt, lname) == 3) {
			COND_JMP(<=);
			print_goto(lname);
			COND_END;
		    return 0;
		}
	}
	if (inst_is("call")) {
		if (myscan(il, lname) == 1) {
			print_line(LABEL LP RP, lname);
			return 0;
		}
	}

	if (inst_is("callR")) {
		if (myscan(ig, &rs) == 1) {
			if (is_xreg(rs)) {
				tmp_print(call *, GR, grlq(rs));
			} else {
				tmp_print(movl, GRc LABEL, grl(rs), "%edx");
				tmp_print(call *, LABEL, "%rdx");
			}
		    return 0;
		}
	}
	if (inst_is("return")) {
		print_line("return");
		return 0;
	}

	if (inst_is("ldi")) {
		if (myscan(iggi, &rt, &rs, &imm) == 3) {

			if (is_xreg(rs) && is_xreg(rt)) {
				tmp_print(movl, ADDRc(GR) GR, imm, grl(rs), grldst(rt));
			} else if (is_xreg(rt)) {
				tmp_print(movl, GRc TMP, grl(rs));
				tmp_print(movl, ADDRc(TMP) GR, imm, grldst(rt));
			} else {
				tmp_print(movl, GRc TMP, grl(rs));
				tmp_print(movl, ADDRc(TMP) TMP1, imm);
				tmp_print(movl, TMP1c GR, grldst(rt));
			}

		    return 0;
		}
	}

	if (inst_is("ld")) {
		if (myscan(iggg, &rd, &rs, &rt)==3) {
			tmp_print(movl, GRc TMP1, grl(rs));
			tmp_print(movl, GRc TMP, grl(rt));
			tmp_print(movl, "0("TMPc TMP1c"1), " TMP);
			tmp_print(movl, TMPc GR, grldst(rd));

		    return 0;
		}
	}
	if (inst_is("st")) {
		if (myscan(iggg, &rd, &rs, &rt)==3) {
			tmp_print(push, LABEL, "%rbx");
			tmp_print(movl, GRc TMP1, grl(rs));
			tmp_print(movl, GRc TMP, grl(rt));
			tmp_print(movl, GRc LABEL, grl(rd), "%ebx");
			tmp_print(movl, LABELc "0("TMPc TMP1c"1)", "%ebx");
			tmp_print(pop, LABEL, "%rbx");
		    return 0;
		}
	}

	if (inst_is("sti")) {
		if (myscan(iggi, &rt, &rs, &imm)==3) {

			if (is_xreg(rs) && is_xreg(rt)) {
				tmp_print(movl, GRc ADDR(GR), grl(rt), imm, grl(rs));
			} else if (is_xreg(rt)) {
				tmp_print(movl, GRc TMP, grl(rs));
				tmp_print(movl, GRc ADDR(TMP), grl(rt), imm);
			} else {
				tmp_print(movl, GRc TMP, grl(rs));
				tmp_print(movl, GRc TMP1, grl(rt));
				tmp_print(movl, TMP1c ADDR(TMP), imm);
			}
		    return 0;
		}
	}
	if(inst_is("fadd")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {

	 		if ((rd == rs) && (is_xmm(rs))) {
				tmp_print(addss, FRc FR, frl(rt), frl(rs));
			} else if ((rd == rt) && (is_xmm(rd))) {
				tmp_print(addss, FRc FR, frl(rs), frl(rd));
			} else {
				tmp_print(movss, FRc FTMP, frl(rs));
				tmp_print(addss, FRc FTMP, frl(rt));
				tmp_print(movss, FTMPc FR, frl(rd));
			}

		    return 0;
		}
	}
	if(inst_is("fsub")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {
	 		if ((rd == rs) && (is_xmm(rs))) {
				tmp_print(subss, FRc FR, frl(rt), frl(rs));
			} else {
				tmp_print(movss, FRc FTMP, frl(rs));
				tmp_print(subss, FRc FTMP, frl(rt));
				tmp_print(movss, FTMPc FR, frl(rd));
			}
		    return 0;
		}
	}
	if(inst_is("fmul")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {

	 		if ((rd == rs) && (is_xmm(rs))) {
				tmp_print(mulss, FRc FR, frl(rt), frl(rs));
			} else if ((rd == rt) && (is_xmm(rd))) {
				tmp_print(mulss, FRc FR, frl(rs), frl(rd));
			} else {
				tmp_print(movss, FRc FTMP, frl(rs));
				tmp_print(mulss, FRc FTMP, frl(rt));
				tmp_print(movss, FTMPc FR, frl(rd));
			}

		    return 0;
		}
	}

	if(inst_is("fdiv")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {
			if (mathlib_flag) {
				if (is_xmm(rt)) {
					tmp_print(movd, FRc TMP, frl(rt));
				} else {
					tmp_print(movl, FRc TMP, frl(rt));
				}
				tmp_print(call, LABEL, "FInv");
				tmp_print(movd, TMPc FTMP);
				tmp_print(mulss, FRc FTMP, frl(rs));
				tmp_print(movss, FTMPc FR, frl(rd));

			} else {
				if ((rd == rs) && (is_xmm(rs))) {
					tmp_print(divss, FRc FR, frl(rt), frl(rs));
				} else {
					tmp_print(movss, FRc FTMP, frl(rs));
					tmp_print(divss, FRc FTMP, frl(rt));
					tmp_print(movss, FTMPc FR, frl(rd));

				}
			}


		    return 0;
		}
	}
	if(inst_is("fsqrt")) {
		if (myscan(iff, &rd, &rs) == 2) {
			if (mathlib_flag) {
				if (is_xmm(rs)) {
					tmp_print(movd, FRc TMP, frl(rs));
				} else {
					tmp_print(movl, FRc TMP, frl(rs));
				}
				tmp_print(call, LABEL, "FSqrt");
				if (is_xmm(rd)) {
					tmp_print(movd, TMPc FR, frl(rd));
				} else {
					tmp_print(movl, TMPc FR, frl(rd));
				}

			} else {
				if (is_xmm(rd)) {
					tmp_print(sqrtss, FRc FR, frl(rs), frl(rd));
				} else {
					tmp_print(movss, FRc FTMP, frl(rs));
					tmp_print(sqrtss, FTMPc FTMP);
					tmp_print(movss, FTMPc FRc, frl(rd));
				}
			}
		    return 0;
		}
	}
	if(inst_is("fabs")) {
		if (myscan(iff, &rd, &rs) == 2) {
			tmp_print(movss, FRc TMP, frl(rs));
			tmp_print(andl, IMMc TMP, 0x7fffffff);
			tmp_print(movss, TMPc FRc, frl(rd));
		    return 0;
		}
	}
	if(inst_is("fmov")) {
		if (myscan(iff, &rd, &rs) == 2) {
			if (is_xmm(rd)) {
				tmp_print(movss, FRc FR, frl(rs), frl(rd));
			} else {
				if (is_xmm(rs)) {
					tmp_print(movd, FRc TMP, frl(rs));
				} else {
					tmp_print(movl, FRc TMP, frl(rs));
				}
				tmp_print(movl, TMPc FR, frl(rd));
			}
		    return 0;
		}
	}
	if(inst_is("fneg")) {
		if (myscan(iff, &rd, &rs) == 2) {
			if (is_xmm(rs)) {
				tmp_print(movd, FRc TMP, frl(rs));
			} else {
				tmp_print(movl, FRc TMP, frl(rs));
			}
			tmp_print(xorl, IMMc TMP, 0x80000000);
			if (is_xmm(rd)) {
				tmp_print(movd, TMPc FR, frl(rd));
			} else {
				tmp_print(movl, TMPc FR, frl(rd));
			}
		    return 0;
		}
	}

	if(inst_is("fld")) {
		if (myscan(ifgg, &rd, &rs, &rt) == 3) {
			tmp_print(movl, GRc TMP1, grl(rs));
			tmp_print(movl, GRc TMP, grl(rt));
			tmp_print(movl, "0("TMPc TMP1c"1), "TMP);
			if (is_xmm(rd)) {
				tmp_print(movd, TMPc FR, frl(rd));
			} else {
				tmp_print(movl, TMPc FR, frl(rd));
			}
		    return 0;
		}
	}
	if(inst_is("fst")) {
		if (myscan(ifgg, &rd, &rs, &rt) == 3) {
			tmp_print(push, LABEL, "%rbx");
			tmp_print(movl, GRc TMP1, grl(rt));
			tmp_print(movl, GRc TMP, grl(rs));
			if (is_xmm(rd)) {
				tmp_print(movd, FRc LABEL, frl(rd), "%ebx");
			} else {
				tmp_print(movl, FRc LABEL, frl(rd), "%ebx");
			}
			tmp_print(movl, LABELc "0("TMPc TMP1c "1)", "%ebx");
			tmp_print(pop, LABEL, "%rbx");
		    return 0;
		}
	}
	if (inst_is("fldi")) {
		if (myscan(ifgi, &rt, &rs, &imm) == 3) {
			if (is_xmm(rt)) {
				if (is_xreg(rs)) {
					tmp_print(movss, ADDRc(GR) FR, imm, grl(rs), frl(rt));
				} else {
					tmp_print(movl, GRc TMP, grl(rs));
					tmp_print(movss, ADDRc(TMP) FR, imm, frl(rt));
				}
			} else {
				tmp_print(movl, GRc TMP1, grl(rs));
				tmp_print(movl, ADDRc(TMP1) TMP, imm);
				tmp_print(movl, TMPc FR, frl(rt));
			}
		    return 0;
		}
	}
	if (inst_is("fsti")) {
		if (myscan(ifgi, &rt, &rs, &imm) == 3) {
			if (is_xmm(rt)) {
				if (is_xreg(rs)) {
					tmp_print(movss, FRc ADDR(GR), frl(rt), imm, grl(rs));
				} else {
					tmp_print(movl, GRc TMP, grl(rs));
					tmp_print(movss, FRc ADDR(TMP), frl(rt), imm);
				}
			} else {
				tmp_print(movl, FRc TMP1, frl(rt));
				tmp_print(movl, GRc TMP, grl(rs));
				tmp_print(movl, TMP1c ADDR(TMP), imm);
			}
		    return 0;
		}
	}
	if (inst_is("fjeq")) {
		if (myscan(iffl, &rs, &rt, lname) == 3) {
			if (is_xmm(rs) && is_xmm(rt)) {
				tmp_print(comiss, FRc FR, frl(rt), frl(rs));
				tmp_print(je, LABEL, lname);
			} else if (is_xmm(rs)) {
				tmp_print(movss, FRc FTMP, frl(rt));
				tmp_print(comiss, FTMPc FR, frl(rs));
				tmp_print(je, LABEL, lname);
			} else if (is_xmm(rt)) {
				tmp_print(movss, FRc FTMP, frl(rs));
				tmp_print(comiss, FTMPc FR, frl(rt));
				tmp_print(je, LABEL, lname);
			} else {
				tmp_print(flds, FR, frl(rt));
				tmp_print(flds, FR, frl(rs));
				tmp_print(fcompp, "");
				tmp_print(fnstsw, LABEL, "%ax");
				tmp_print(sahf, "");
				tmp_print(je, LABEL, lname);
			}
		    return 0;
		}
	}

	if (inst_is("fjlt")) {
		if (myscan(iffl, &rs, &rt, lname) == 3) {
			if (is_xmm(rs) && is_xmm(rt)) {
				tmp_print(comiss, FRc FR, frl(rt), frl(rs));
				tmp_print(jb, LABEL, lname);
			} else if (is_xmm(rs)) {
				tmp_print(movss, FRc FTMP, frl(rt));
				tmp_print(comiss, FTMPc FR, frl(rs));
				tmp_print(jb, LABEL, lname);
			} else if (is_xmm(rt)) {
				tmp_print(movss, FRc FTMP, frl(rs));
				tmp_print(comiss, FRc FTMP, frl(rt));
				tmp_print(jb, LABEL, lname);
			} else {
				tmp_print(flds, FR, frl(rt));
				tmp_print(flds, FR, frl(rs));
				tmp_print(fcompp, "");
				tmp_print(fnstsw, LABEL, "%ax");
				tmp_print(sahf, "");
				tmp_print(jb, LABEL, lname);
			}

		    return 0;
		}
	}
	if(inst_is("nop")){
		tmp_print(nop, "");
		return 0;
	}
	if(inst_is("halt")){
		print_line("return 0");
		return 0;
	}
	if (inst_is("not")) {
		if (myscan(igg, &rd, &rs) == 2) {

			if (rd == rs) {
				tmp_print(notl, GR, grl(rd));
	 		} else if (is_xreg(rs) || is_xreg(rd)) {
				tmp_print(movl, GRc GR, grl(rs), grldst(rd));
				tmp_print(notl, GR, grl(rd));
			} else {
				tmp_print(movl, GRc TMP, grl(rs));
				tmp_print(notl, TMP);
				tmp_print(movl, TMPc GR, grldst(rd));
			}

		    return 0;
		}
	}

	if(inst_is("setL")){
		if (myscan(igl, &rd, lname) == 2) {
			tmp_print(movl, "$%s, "GR, lname, grl(rd));
		    return 0;
		}
	}
	return -1;
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

void print_inc_line(void) {
	tmp_print(incq, LABEL, "(CNT)");
}

#undef set_label


