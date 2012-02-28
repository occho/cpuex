#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include "oc_sim.h"

static int log_flag;
static char *log_file = (char*) "ika.log";
static char *sfile;

static void configure(int argc, char **argv);
static void register_segv_handler(void);
static void prom_set(int argc, char **argv);
static void print_conf(void);
int simulate(void);
static void print_elapsed_time(void);
static void print_result(void);
static void print_usage(char*);
FILE *err_fp;

static struct timeval tv1,tv2;
int main(int argc, char **argv, char **envp) {
	configure(argc, argv);
	print_conf();
	register_segv_handler();
	prom_set(argc, argv);
	gettimeofday(&tv1, NULL);
	simulate();
	gettimeofday(&tv2, NULL);
	print_result();
	
	return 0;
}


#define print_option(fmt, ...) \
	warning("\t"fmt"\n", ##__VA_ARGS__)
static void print_usage(char*name) {
	warning("\n");
	warning("USAGE\t: %s $file [$option]\n", name);
	warning("OPTIONS\t:\n");
	print_option("-l [$log_file]\t: place log output in file $log_file");
	warning("\n");
}
#undef print_option

static void configure(int argc, char **argv) {
	int opt;
	err_fp = stderr;
	sfile = argv[1];

	while ((opt = getopt(argc, argv, "l")) != -1) {
		switch (opt) {
			case 'l' :
				log_flag = 1;
				if ((argc != optind) && (argv[optind][0] != '-')) {
					log_file = argv[optind];
					optind++;
				}
				break;

			default :
				print_usage(argv[0]);
				exit(1);
				break;
		}
	}
}

static void prom_set(int argc, char **argv) {
	int fd, ret;
	if (argc < 2) {
		print_usage(argv[0]);
		exit(1);
	}
	fd = open(sfile, O_RDONLY);
	if (fd<0) {
		perror("open @ prom_set");
		exit(1);
	}
	ret = read(fd, prom, ROM_NUM*4);
	if (ret<0) {
		perror("read");
		exit(1);
	}
	close(fd);
}

void segv_handler(int);
static void register_segv_handler(void) {
	struct sigaction sa;
	sa.sa_handler = segv_handler;
	if (sigaction(SIGSEGV, &sa, NULL) != 0) {
		perror("sigaction");
		exit(1);
	}

}

void segv_handler(int n) {
	uint32_t ir = prom[pc/4 - 1];
	warning("せぐふぉー@%lu.[%x] ir:%08X ", cnt, pc, ir);
	print_ir(ir);
	warning("\n");
	exit(1);
}

#define print_val(fmt, ...) \
	warning("* "fmt"\n", ##__VA_ARGS__)
static void print_conf(void) {
	warning("\n");
	warning("######################## SIMCHO CONFIGURATION ########################\n\n");
	print_val("source\t: %s", sfile);
	if (log_flag > 0) {
		print_val("log file\t: %s", log_file);
	} else {
		print_val("log file\t: no output");
	}
	warning_nl();
	warning("========================== RUNNING PROGRAM ===========================\n");
}
static void print_result(void) {
	warning("[sim's newline]\n");
	warning("========================== SIMULATOR RESULT ==========================\n");
	warning_nl();
	print_elapsed_time();
	print_val("cnt\t: %lu", cnt);
	warning_nl();
	warning("######################################################################\n");
	warning_nl();

}

static void print_elapsed_time(void) {
	int elapsed_sec, elapsed_usec;
	elapsed_sec = tv2.tv_sec - tv1.tv_sec;
	elapsed_usec = tv2.tv_usec - tv1.tv_usec;
	if (elapsed_sec == 0) {
		print_val("elapsed_time\t: %u [us]", elapsed_usec);
	} else {
		print_val("elapsed_time\t: %lf [s]", (double)elapsed_sec + elapsed_usec*1e-6);
	}
}

#undef print_val
