#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "oc_asm.h"

static inline int set_term0(char*,char*);
static inline int is_directive(char*,char*);
static inline int is_label(char*,char*);
static inline int is_comment(char*,char*);
static inline void output_data(uint32_t data);
static inline void exec_directive(char*, char*);
static inline void register_label(char*, char*);
static inline void encode_and_output(char*, char*);

static int input_line_cnt;
static int output_cnt;
static uint32_t *output_alias;
static int err_cnt;

#define print_asm_line() \
	warning(">>> %s", asm_line)
#define _myerr(fmt, ...) \
	warning("Error %d Line %d : "fmt"\n", ++err_cnt, input_line_cnt, ##__VA_ARGS__)
#define myerr(fmt, ...) \
	do { \
		_myerr(fmt, ##__VA_ARGS__); \
		print_asm_line(); \
	} while(0) 

int assemble(char *asm_buf, uint32_t *out_buf) {
	char asm_line[LINE_MAX];
	char term0[LINE_MAX];
	output_alias = out_buf;

	while (mygets(asm_line, asm_buf, COL_MAX) != NULL) {
		if (set_term0(asm_line, term0) == 1) {
			if (is_comment(asm_line, term0)) {
				// blank(comment)
			} else if (is_directive(asm_line, term0)) {
				exec_directive(asm_line, term0);
			} else if (is_label(asm_line, term0)) {
				register_label(asm_line, term0);
			} else { 
				encode_and_output(asm_line, term0);
			}
		} else {
			// blank(empty line)
		}
		input_line_cnt++;
	}

	return output_cnt;
}
static inline void output_data(uint32_t data) {
	output_alias[output_cnt++] = data;
}
static inline void encode_and_output(char*asm_line, char*term0) {
	uint32_t ir = encode_op(asm_line, term0);
	if (ir == 0xffffffff) {
		myerr("Unknown instruction @ encode_op");
	}
	output_data(ir);
}
static inline int _directive_is(char *line, const char* str) {
	return strstr(line, str) != NULL;
}
#define directive_is(str) _directive_is(asm_line, "."str)
static inline void exec_directive(char *asm_line, char *term0) {
	int heap_size;
	uint32_t data;
	int reg_num;
	char line[COL_MAX], lname[COL_MAX];

	if (directive_is("init")) { // init 
		if ((output_cnt == 0) &&
			(sscanf(asm_line, " .init_heap_size %d", &heap_size) == 1)) {
			output_data((uint32_t)heap_size/8);
		} else {
			myerr("init directive");
		}
	} else if (directive_is("long")) { // data(.long)
	 	if (sscanf(asm_line, " .long 0x%x", &data) == 1) {
			output_data(data);
		} else {
			myerr("long directive");
		}
	} else if (directive_is("setL")) { // setL
		if(sscanf(asm_line, " .setL %%g%d, %s", &reg_num, lname) == 2) {
			//strcpy(label_name[label_cnt],lname);
			sprintf(line, asm_fmt_iggi, "addi", reg_num, 0, 0);
			encode_and_output(line, "addi");
		} else {
			myerr("setL directive");
		}

	}
}
static inline void register_label(char *asm_line, char *term0) {
	char *label_name;
	label_t label;
	if ((label_name = strtok(term0, ":")) != NULL) {
		label.len = strlen(label_name);
		strcpy(label.name, label_name);
		label.line = output_cnt;
		hash_insert(label);
	} else {
		myerr("register label");
	}
}

static inline int set_term0(char *line, char *term0) {
	return sscanf(line, "%s", term0);
}

static inline int is_directive(char*line, char *term0) {
	return term0[0] == '.';
}
static inline int is_label(char*line, char *term0) {
	return strchr(line,':') != NULL;
}
static inline int is_comment(char*line, char *term0) {
	return term0[0] == '!';
}

