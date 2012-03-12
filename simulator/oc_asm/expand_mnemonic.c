#include <stdio.h>
#include <string.h>
#include "oc_asm.h"

static char asm_line[COL_MAX];
static char term0[COL_MAX];
static char tmpbuf[COL_MAX];

#define print_expanded(fmt, ...) \
	(dst_ptr += sprintf(dst_ptr, "\t" asm_fmt_##fmt "\n", ##__VA_ARGS__))
#define print_original(fmt, ...) \
	(dst_ptr += sprintf(dst_ptr, fmt, ##__VA_ARGS__))
#define pad_nop() \
	print_expanded(iggi, "slli", 0,0,0)
static inline int _inst_is(char *inst, const char *str) {
	return strcmp(inst, str) == 0;
}
#define inst_is(str) _inst_is(term0, str)
#undef myscan
#define myscan(fmt, ...) \
	(sscanf(asm_line, asm_fmt_##fmt, tmpbuf, ##__VA_ARGS__) - 1)

static void pocore_mnemonic(char *asm_line, char *term0);
static void ocore_mnemonic(char *asm_line, char *term0);
static char *dst_ptr;
static int rd,rt,rs;
int expand_mnemonic(char *ex_mne_buf, char *asm_buf) {
	dst_ptr = ex_mne_buf;

    while (mygets(asm_line, asm_buf, COL_MAX) != NULL) {

    	if (sscanf(asm_line, "%s", term0) == 1) {

			if (is_comment(asm_line, term0)) {
			} else if (is_directive(asm_line, term0)) {
				print_original("%s", asm_line);
			} else if (is_label(asm_line, term0)) {
				print_original("%s", asm_line);
			} else { 
				if (arch_is_pocore()) {
					pocore_mnemonic(asm_line, term0);
				} else {
					ocore_mnemonic(asm_line, term0);
				}
			}

		} else {
		}
    }
	return (int) (dst_ptr - ex_mne_buf);
}
static void pocore_mnemonic(char *asm_line, char *term0) {

	pad_nop();
	if (inst_is("nop")) {
		print_expanded(iggi, "slli", 0,0,0);
	} else if (inst_is("halt")) {
		print_original("__HALT_LABEL__:\n");
		print_expanded(il, "jmp", "__HALT_LABEL__");
		print_expanded(il, "jmp", "__HALT_LABEL__");
		print_expanded(il, "jmp", "__HALT_LABEL__");
	} else if (inst_is("mov")) {
		if (myscan(igg, &rd, &rs) == 2) {
			print_expanded(iggi, "addi", rd, rs, 0);
		}
	} else if (inst_is("not")) {
		if (myscan(igg, &rd, &rs) == 2) {
			print_expanded(iggg, "nor", rd, rs, rs);
		}
	} else if (inst_is("setL")) {
		if (myscan(igl, &rd, tmpbuf) == 2) {
			print_original(asm_fmt_igl, ".setL", rd, tmpbuf);
			print_original("\n");
		}
	} else if (inst_is("fadd")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {
			print_original("%s", asm_line);
			pad_nop();
		}
	} else if (inst_is("fsub")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {
			print_original("%s", asm_line);
			pad_nop();
		}
	} else if (inst_is("fmul")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {
			print_original("%s", asm_line);
			pad_nop();
		}
	} else if (inst_is("fdiv")) {
		if (myscan(ifff, &rd, &rs, &rt) == 3) {
			print_original("%s", asm_line);
			pad_nop();
		}
	} else if (inst_is("fsqrt")) {
		if (myscan(iff, &rd, &rs) == 2) {
			print_original("%s", asm_line);
			pad_nop();
		}
	} else {
		print_original("%s", asm_line);
	}
	pad_nop();

}

static void ocore_mnemonic(char *asm_line, char *term0) {
	if (inst_is("nop")) {
		print_expanded(iggi, "slli", 0,0,0);
	} else if (inst_is("mov")) {
		if (myscan(igg, &rd, &rs) == 2) {
			print_expanded(iggi, "addi", rd, rs, 0);
		}
	} else if (inst_is("not")) {
		if (myscan(igg, &rd, &rs) == 2) {
			print_expanded(iggg, "nor", rd, rs, rs);
		}
	} else if (inst_is("setL")) {
		if (myscan(igl, &rd, tmpbuf) == 2) {
			print_original(asm_fmt_igl, ".setL", rd, tmpbuf);
			print_original("\n");
		}
	} else {
		print_original("%s", asm_line);
	}

}
