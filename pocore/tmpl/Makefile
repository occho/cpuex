RENDER = mako-render
OCORE_DIR = ..
ISE_SPACE = ../../ise_space
VSIM_SPACE = ../../vsim_space
ASM = ./asmcho
SIM = ./simcho

OCORE_MODS = reg_dc top u232c io_dev core_c clk_gen clk_delay \
			 prom decode exec mem_acc sram reg_wb testbench
			
ISE_MODS = ise_top u232c io_dev core_c clk_gen clk_delay \
			decode reg_dc exec mem_acc reg_wb

TARGET = $(OCORE_MODS:%=%.vhd) $(ISE_MODS:%=%.vhd)
COMPILER_DIR = ../../compiler
SIM_TARGET = $(COMPILER_DIR)/test/fib

.SUFFIXES: .s
.SUFFIXES: .vhd .tmp

.tmp.vhd:
	@echo '$< ==> $@'
	$(RENDER) $< > $@


.PHONY: all
all: $(TARGET)

ocore: .ocore.stamp
.ocore.stamp: $(OCORE_MODS:%=%.vhd) test
	cp $? $(OCORE_DIR)/
	touch .ocore.stamp

ise: .ise.stamp
.ise.stamp: $(ISE_MODS:%=%.vhd) test.coe
	cp $? $(ISE_SPACE)/
	touch .ise.stamp

vsim: $(OCORE_MODS:%=%.vhd)
	cp $? $(VSIM_SPACE)/

BASE = skeleton.tmp macro.tmp

CORE_C = clk_gen clk_delay prom decode reg_dc exec reg_wb mem_acc
core_c.vhd: core_c.tmp $(CORE_C:%=%.tmp) $(BASE)

clk_gen.vhd: clk_gen.tmp $(BASE)
clk_dly.vhd: clk_dly.tmp $(BASE)
decode.vhd: decode.tmp $(BASE)
reg_dc.vhd: reg_dc.tmp $(BASE)
exec.vhd: exec.tmp $(BASE)
mem_acc.vhd: mem_acc.tmp $(BASE)
reg_wb.vhd: reg_wb.tmp $(BASE)
prom.vhd: prom.tmp test $(BASE)

u232c.vhd: u232c.tmp 
io_dev.vhd: io_dev.tmp u232c.tmp contest_sldbin.txt contest_sld.txt $(BASE)

top.vhd: top.tmp skeleton_top.tmp $(BASE)
ise_top.vhd: ise_top.tmp skeleton_top.tmp $(BASE)
skeleton_top.tmp: core_c.tmp io_dev.tmp $(BASE)

sram.vhd: sram.tmp $(BASE)
testbench.vhd: testbench.tmp top.tmp sram.tmp $(BASE)

.PHONY: asm
asm : test.coe test

LINE_MIN = 2048
test.coe: $(SIM_TARGET).s
	$(ASM) $< -c -o test.coe
	mv $(SIM_TARGET) $(SIM_TARGET).coe
	$(ASM) $< -x 2048 -l

test: $(SIM_TARGET).s
	$(ASM) $< -x 2048 -o test -l -p

sim: $(SIM_TARGET)
	$(ASM) $<.s -l
	$(SIM) ika.out -l

.PHONY: clean
clean:
	$(RM) *.vhd $(SIM_TARGET) $(SIM_TARGET).coe *.lst asmlog *.out test
