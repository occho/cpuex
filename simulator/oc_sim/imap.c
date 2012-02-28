#include "oc_sim.h"
#define BLANK "    "
const char *InstMap[INST_NUM] = {
BLANK,BLANK,"jmp",BLANK,BLANK,BLANK,"sin", "mvlo",
"addi",BLANK,"jeq",BLANK,BLANK,BLANK,"cos", "mvhi",
"subi",BLANK,"jne","ld",BLANK,BLANK,"atan",BLANK,
"muli",BLANK,"jlt","st",BLANK,BLANK,"int_of_float",BLANK,
"divi",BLANK,"jle","ldi",BLANK,BLANK,"float_of_int",BLANK,
"slli",BLANK,"srli""sti",BLANK,BLANK,BLANK,BLANK,
"call","fldi", "fjeq","ldlr",BLANK,"io",BLANK,BLANK,
"return","fsti", "fjlt","stlr","special","link","fpi","others",
};
const char *InstTyMap[INST_NUM] = {
BLANK,BLANK,"fl",  BLANK,BLANK,BLANK,"else", "fgi",
"fggi",BLANK,"fggl",BLANK,BLANK,BLANK,"else", "fgi",
"fggi",BLANK,"fggl","fggg",BLANK,BLANK,"else",BLANK,
"fggi",BLANK,"fggl","fggg",BLANK,BLANK,"else",BLANK,
"fggi",BLANK,"fggl","fggi",BLANK,BLANK,"else",BLANK,
"fggi",BLANK,"fggi","fggi",BLANK,BLANK,BLANK,BLANK,
"fl",  "ffgi","fffl","fgi", BLANK,"io",BLANK,BLANK,
"f",   "ffgi","fffl","fgi", "special","fi","fpi","others",
};
const char *SFunctMap[INST_NUM] = {
"sll",BLANK,"srl", BLANK, BLANK,BLANK,BLANK, BLANK
"b",BLANK,BLANK,BLANK, BLANK,BLANK,BLANK, BLANK
"btmplr",BLANK,BLANK,BLANK, BLANK,BLANK,BLANK,BLANK,
"mul",BLANK,BLANK,"nor",  BLANK,BLANK,BLANK,BLANK,
"add",BLANK,"sub", BLANK,   "and", "or",  BLANK,BLANK,
BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,
"callR", "fld",BLANK,"movlr",BLANK,BLANK,BLANK,BLANK,
BLANK,  "fst",BLANK,BLANK, BLANK,BLANK,BLANK,"halt",
};
const char *SFunctTyMap[INST_NUM] = {
"fggg",BLANK,"fggg",BLANK,BLANK,BLANK,BLANK,BLANK
"fg",  BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK
"f",   BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,
"fggg",BLANK,"fggg","fggg",BLANK,BLANK,BLANK,BLANK,
"fggg",BLANK,"fggg",BLANK,"fggg","fggg",BLANK,BLANK,
BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,
"fg","ffgg",BLANK,"f",BLANK,BLANK,BLANK,BLANK,
BLANK,"ffgg",BLANK,BLANK,BLANK,BLANK,BLANK,"f",
};
const char *FFunctMap[INST_NUM] = {
"fadd","fsub","fmul","fdiv","fsqrt","fabs","fmov","fneg",
};
const char *FFunctTyMap[INST_NUM] = {
"ffff","ffff","ffff","ffff","fff","fff","fff","fff",
};
const char *IOFunctMap[INST_NUM] = {
"input","output",
};
const char *IOFunctTyMap[INST_NUM] = {
"fg","fg",
};

