# asm/Makefile
ASM = ./asmcho
TARGET = asmcho
ASMH = common.h oc_asm.h
HOME_DIR = ..
include ../Makefile.in
CC = gcc
LD = gcc

CFLAGS += -I ../include/
vpath %.h ../include/
ASM_TARGET = $(TEST_TARGET:%=$(COMPILER_DIR)/test/%.s)
#ASM_TARGET = $(TEST_TARGET:%=source/as/%.s)
#ASM_FLAGS += -h

UTIL = asm_util.o
asmcho: asmcho.o expand_mnemonic.o assemble.o $(UTIL) label_hash_map.o encode_op.o
asmcho.o: $(ASMH) expand_mnemonic.o assemble.o $(UTIL)
assemble.o: $(ASMH) $(UTIL) label_hash_map.o encode_op.o
expand_mnemonic.o: $(ASMH) $(UTIL)
encode_op.o: $(ASMH) $(UTIL)

asm:
	$(ASM) $(ASM_FLAGS) $(ASM_TARGET)
rt:
	$(ASM) $(MIN_RT:%=%.s)

