#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "oc_asm.h"
enum out_fmt_t { OFMT_BIN, OFMT_STR_BIN, OFMT_STR_HEX, OFMT_COE, 
				 OFMT_EX_MNE};
static enum out_fmt_t output_type = OFMT_BIN;

static char *dfile = (char*) "ika.out";
static char *lst_file = (char*) "ika.lst";
static char *sfile;
static FILE *sfp, *dfp, *lst_fp;
static int sfd;
static int lst_flag, dst_flag, mne_flag, be_quiet;
static int output_line_min;
static uint32_t binary_data[LINE_MAX];
static char ex_mne_buf[LINE_MAX*COL_MAX];
static char asm_buf[LINE_MAX*COL_MAX];

static void print_usage(char*name);
static void configure(int argc, char** argv);
static void print_conf(void);
static void conf_sfile(int argc, char** argv);
static void conf_out_fmt(int argc, char** argv); 
static void open_files(void);
static void close_files(void);
int assemble(char *buf, uint32_t* binary_data);
int expand_mnemonic(char *asm_buf, char *ex_mne_buf);

int main(int argc, char **argv) {
	int i,size;
	configure(argc, argv);
	open_files();
	print_conf();
	size = read(sfd, asm_buf, LINE_MAX*COL_MAX);
	if (size<0) { exit(1); }
	//warning("size: %d\n", size);
	size = expand_mnemonic(asm_buf, ex_mne_buf);
	if (size<0) { exit(1); }
	//warning("size: %d\n", size);
		//fprintf(dfp,"%s", ex_mne_buf);
	if ((size = assemble(ex_mne_buf, binary_data)) < 0) {
		exit(1);
	}
	for (i=0;i<size;i++) {
		fprintf(dfp,"%08x\n", binary_data[i]);
	}
	//fprintf(STDERR, "%08x\n", ADD_F);


	exit(0);
}
static void configure(int argc, char** argv) {
	conf_sfile(argc, argv);
	conf_out_fmt(argc, argv);
}

static void conf_sfile(int argc, char** argv) {
	if (argc < 2) {
		print_usage(argv[0]);
		exit(1);
	}
	sfile = argv[1];

	if (sfile==NULL) {
		warning("Not Found: source file\n");
		exit(1);
	}

}
static void close_files(void) {
	fclose(sfp);
	close(sfd); 
	if (dst_flag == 0) {
		fclose(dfp);
	}
	if (lst_flag > 0) {
		fclose(lst_fp);
	}
}
static void open_files(void) {

	sfp = fopen(sfile, "r");
	if (sfp == NULL) {
		warning("sfile @ open_files: %s\n", sfile);
		perror("fopen");
		exit(1);
	};
	sfd = open(sfile, O_RDONLY);
	if (sfd < 0) {
		warning("sfile @ open_files: %s\n", sfile);
		perror("open");
		exit(1);
	}

	if (dst_flag == 1) {
		dfp = stdout;
	} else if (dst_flag == 2) {
		dfp = stderr;
	} else {
		dfp = fopen(dfile, "wr");
		if (dfp == NULL) {
			warning("dfile @ open_files: %s\n", dfile);
			perror("fopen");
			exit(1);
		};
	}


	if (lst_flag > 0) {
		lst_fp = fopen(lst_file, "w");
		if (lst_fp == NULL) {
			warning("lst_file @ open_files: %s\n", lst_file);
			perror("fopen");
			exit(1);
		};
	}
	atexit(close_files);
}

#define print_option(fmt, ...) \
	warning("\t"fmt"\n", ##__VA_ARGS__)
static void print_usage(char*name) {
	warning("\n");
	warning("USAGE\t: %s $file [$options]\n", name);
	warning("OPTIONS\t:\n");
	print_option("-o $dst_file\t: place output in file $dst_file");
	print_option("-l [$lst_file]\t: place list output in file $lst_file");
	print_option("-b $line_num\t: output binary string");
	print_option("-h $line_num\t: output hexadecimal string");
	print_option("-c $line_num\t: output coe format string");
	print_option("-m\t: output code after expanding mnemonic");
	print_option("-q\t: don't print configure information");
	warning("\n");
}
#undef print_option

static void conf_out_fmt(int argc, char** argv) {
	int opt;
	while ((opt = getopt(argc, argv, "lmqo:b:h:c:")) != -1) {
		switch (opt) {
			case 'o' :
				if (atoi(optarg) == 1) {
					dst_flag = 1;
				} else if (atoi(optarg) == 2) {
					dst_flag = 2;
				} else {
					dst_flag = 0;
					dfile = optarg;
				}
				break;
			case 'q' :
				be_quiet = 1;
				break;
			case 'l' :
				lst_flag = 1;
				if ((argc != optind) && (argv[optind][0] != '-')) {
					lst_file = argv[optind];
					optind++;
				}
				break;
			case 'm' :
				output_type = OFMT_EX_MNE;
				mne_flag = 1;
				break;
			case 'b' :
				output_type = OFMT_STR_BIN;
				output_line_min = atoi(optarg);
				break;
			case 'h' :
				output_type = OFMT_STR_HEX;
				output_line_min = atoi(optarg);
				break;
			case 'c' :
				output_type = OFMT_COE;
				output_line_min = atoi(optarg);
				break;
			case ':' :
			case '?' :
			default :
				print_usage(argv[0]);
				exit(1);
				break;
		}
	}
}

#define print_val(fmt, ...) \
	warning("* "fmt"\n", ##__VA_ARGS__)
static void print_conf(void) {
	if (be_quiet == 1) {
		return;
	}
	warning("\n");
	warning("######################## ASMCHO CONFIGURATION ########################\n\n");
	print_val("source\t: %s", sfile);
	switch (dst_flag) {
		case 1:
			print_val("destination\t: stdout");
			break;
		case 2:
			print_val("destination\t: stderr");
			break;
		default:
			print_val("destination\t: %s", dfile);
			break;
	}
	if (lst_flag > 0) {
		print_val("list file\t: %s", lst_file);
	} else {
		print_val("list file\t: no output");
	}
	switch (output_type) {
		case OFMT_BIN :
			print_val("output_type\t: binary format (executable by simulator)");
			break;
		case OFMT_STR_BIN :
			print_val("output_type\t: binary string format");
			break;
		case OFMT_STR_HEX :
			print_val("output_type\t: hexadecimal string format");
			break;
		case OFMT_COE :
			print_val("output_type\t: coe format");
			break;
		case OFMT_EX_MNE :
			print_val("output_type\t: assembly after expand mnemonic");
			break;
		default :
			print_val("conf_out_fmt: unexpected output_type");
			exit(1);
			break;
	}
	print_val("output_line_min\t: %d", output_line_min);
	warning("\n");
}
#undef print_val
