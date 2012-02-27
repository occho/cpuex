
#ifndef _ASM_HEAD
#define _ASM_HEAD
#include "common.h"
#include <stdint.h>
#define	COL_MAX 512
#define LINE_MAX (32*1024)

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

#define warning(fmt, ...) \
	fprintf(stderr, fmt, ##__VA_ARGS__)
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
extern inline uint32_t shift_right_a(unsigned shiftwidth, uint32_t num);
void register_op_using_label(char*);

#endif
