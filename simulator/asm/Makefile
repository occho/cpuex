# asm/Makefile
ASM = ./oc_asmcho
TARGET = oc_asmcho
ASMH = common.h asm.h
HOME_DIR = ..
include ../Makefile.in

CFLAGS += -I ../include/
CXXFLAGS += -I ../include/
vpath %.h ../include/
ASM_TARGET = $(TEST_TARGET:%=$(COMPILER_DIR)/test/%.s)
#ASM_TARGET = $(TEST_TARGET:%=source/as/%.s)
#ASM_FLAGS += -h

asmcho: asmcho.o assemble.o encode_op.o mnemonic.o
asmcho.o: $(ASMH) assemble.o
assemble.o: $(ASMH) encode_op.o mnemonic.o
encode_op.o: $(ASMH)
mnemonic.o: $(ASMH)

oc_asmcho: oc_asmcho.o oc_assemble.o encode_op.o oc_mnemonic.o
oc_asmcho.o : $(ASMH) oc_assemble.o
oc_assemble.o: $(ASMH) encode_op.o oc_mnemonic.o
oc_mnemonic.o: $(ASMH)

asm:
	$(ASM) $(ASM_FLAGS) $(ASM_TARGET)
rt:
	$(ASM) $(MIN_RT:%=%.s)

