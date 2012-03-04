TB = testbench
FPU = fadd fmul fsqrt finv lzc
FPU_SIM = $(FPU) finv_table fsqrt_table
OBJS = top.o clk_gen.o core_c.o decode.o reg_dc.o \
	std_logic_1164_additional.o \
	exec.o reg_wb.o testbench.o mem_acc.o u232c.o \
	io_dev.o prom.o fetch.o clk_delay.o sram.o\
	$(FPU_SIM:%=%.o)
ISE_VHDL = $(FPU) std_logic_1164_additional 
VSIM_VHDL = $(FPU_SIM) std_logic_1164_additional 

ISE_SPACE = ../ise_space
VSIM_SPACE = ../vsim_space
TMPL_DIR = ./tmpl


TIME = 10us
.SUFFIXES: .vhd .vcd .tmp
.PHONY: default wave vcd reload

default: ocore

CACHE = ../cache
CSERVER = $(CACHE)/cserver 
PICT_NAME = contest
SLD = $(CACHE)/sld/$(PICT_NAME).sld
PPM = $(CACHE)/core_output/$(PICT_NAME).ppm
PPM_ANS = $(CACHE)/ppm_p6_ans/$(PICT_NAME).ppm
rt :
	$(CSERVER) $(SLD) $(PPM) -b
cmp :
	cmp $(PPM) $(PPM_ANS) -l
diff :
	diff $(PPM) $(PPM_ANS) -l

ocore :
	make -C tmpl ocore

ise :
	make -C tmpl ise
	cp $(ISE_VHDL:%=%.vhd) top.ucf $(ISE_SPACE)

vsim :
	make -C tmpl vsim
	cp $(VSIM_VHDL:%=%.vhd) $(VSIM_SPACE)

wave:
	gtkwave -o $(TB).vcd
vcd : $(TB).vcd

TARGET = $(TB)
IEEE = 'synopsys'
OPTION = -fexplicit
$(TB).vcd: $(TB)
	ghdl -r $(TB) --vcd=$@ --stop-time=$(TIME)
$(TB): $(OBJS)
	ghdl -e --ieee=$(IEEE) $(OPTION) $(TB)
.vhd.o:
	ghdl -a --ieee=$(IEEE) $(OPTION) $<

asm:
	make -sC $(TMPL_DIR) asm
sim:
	make -sC $(TMPL_DIR) sim

clean:
	make -sC $(TMPL_DIR) clean
	rm -f *.s *.o *.vcd *.cf $(TB) *.fst test
