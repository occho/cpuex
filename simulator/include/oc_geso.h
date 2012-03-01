
#ifndef __GESO_HEAD__
#define __GESO_HEAD__
#include "asm_fmt.h"

#define NL myprint("\n")
#define OP(name) myprint("\t%s\t\t", #name)
#define S(name) myprint("%s", #name)
#define SC(name) myprint(", %s", #name)
#define L(name) myprint("%s", name)
#define LC(name) myprint(", %s", name)
#define G(i) print_gr(i, 0, 0)
#define GC(i) print_gr(i, 1, 0)
#define GQ(i) print_gr(i, 0, 1)
#define F(i) print_fr(i,0)
#define FC(i) print_fr(i,1)
#define IM(i) myprint("$%d", i)
#define IMDW(i) myprint("$%d", i)
#define ADR(a,b) myprint("-%d(%s)", b, #a)
#define ADRC(a,b) myprint(", -%d(%s)",b, #a)
#define WORD myprint("word ")
#define DWORD myprint("dword ")

extern int is_const(int);
extern int is_xreg(int);
extern int is_xmm(int);

extern int count_flag;
extern int mathlib_flag;
extern FILE *dfp;
#define warning(fmt, ...) \
	fprintf(stderr, fmt, ##__VA_ARGS__)
#define myprint(fmt, ...) \
	fprintf(dfp, fmt, ##__VA_ARGS__)
#define myputs(str) \
	fputs(str"\n", dfp)
#endif
