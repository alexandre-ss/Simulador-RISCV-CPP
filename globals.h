//
//  globals.h
//  RV32Ic++
//
//
//

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define get_bit(word, index) (word >> index)&1
#define set_bit(word, index, val) ((word & ~(1 << index)) | ((val&1) << index))
#define get_field(word, index, mask) (word >> index)&mask
#define set_field(word, index, mask, value) (word & ~(mask << index)) | (value << index)

// OPCODE: campo principal de identificacao da instrucao
enum OPCODES {
    LUI = 0x37,		AUIPC = 0x17,		// atribui 20 msbits
    ILType = 0x03,						// Load type
    BType = 0x63,						// branch condicional
    JAL = 0x6F,		JALR = 0x67,		// jumps
    StoreType = 0x23,					// store
    ILAType = 0x13,						// logico-aritmeticas com imediato
    RegType = 0x33,
    ECALL = 0x73
};

// FUNCT3: campo de 3 bits auxiliar na identificacao da instrucao
enum FUNCT3 {
    BEQ3=0,		BNE3=01,	BLT3=04,	BGE3=05,	BLTU3=0x06, BGEU3=07,
    LB3=0,		LH3=01,		LW3=02,		LBU3=04,	LHU3=05,
    SB3=0,		SH3=01,		SW3=02,
    ADDSUB3=0,	SLL3=01,	SLT3=02,	SLTU3=03,
    XOR3=04,	SR3=05,		OR3=06,		AND3=07,
    ADDI3=0,	ORI3=06,	SLTI3=02,	XORI3=04,	ANDI3=07,
    SLTIU3=03,	SLLI3=01,	SRI3=05
};

// FUNCT7: campo auxiliar na identificacao da instrucao
enum FUNCT7 {
    ADD7=0,	SUB7=0x20,	SRA7=0x20,	SRL7=0, SRLI7=0x00,	SRAI7=0x20
};

enum FORMATS { RType, IType, SType, SBType, UType, UJType, NullFormat, NOPType };

enum INSTRUCTIONS {
    I_add,	I_addi, I_and,  I_andi, I_auipc,
    I_beq,	I_bge,	I_bgeu, I_blt,  I_bltu,
    I_bne,  I_jal,	I_jalr, I_lb,	I_lbu,
    I_lw,   I_lh,   I_lhu,  I_lui,  I_sb,
    I_sh,   I_sw,   I_sll,  I_slt,  I_slli,
    I_srl,  I_sra,  I_sub,  I_slti, I_sltiu,
    I_xor,	I_or,	I_srli, I_srai,  I_sltu,
    I_ori, I_ecall, I_xori, I_nop
};

//
// identificacao dos registradores do banco do RV32I
//
enum REGISTERS {
    ZERO=0, RA=1,	SP=2,	GP=3,
    TP=4,	T0=5,	T1=6,	T2=7,
    S0=8,	S1=9,	A0=10,	A1=11,
    A2=12,	A3=13,	A4=14,	A5=15,
    A6=16,	A7=17,  S2=18,	S3=19,
    S4=20,	S5=21, 	S6=22,	S7=23,
    S8=24,	S9=25,  S10=26,	S11=27,
    T3=28,	T4=29,	T5=30,	T6=31
};

//
//	Memory
//
enum { MEM_SIZE = 4096 };
enum { DATA_SEGMENT_START = 0x2000 };


//
// Identificacao de tipo de dado
//
enum DATA_TYPE {
     BYTE=4, INTEGER=1
};

//
// Word size: 2ˆ(size) em bytes
//
enum WORD_SIZE_E {
    BYTE_SIZE = 0, HALF_SIZE = 1, WORD_SIZE = 2, DOUBLE_SIZE = 3
};

const WORD_SIZE_E WSIZE = WORD_SIZE;

int32_t breg[32];                   // banco de registradores

uint32_t pc,						// contador de programa
         ri,						// registrador de intrucao
         sp,						// stack pointe4r
         gp;						// global pointer

int32_t		imm32_t,                // imediato da instrucao
            zero = 0,
            imm12_i,				// constante 12 bits
            imm12_s,				// constante 12 bits
            imm13,					// constante 13 bits
            imm20_u,				// constante 20 bis mais significativos
            imm21;					// constante 21 bits

uint32_t	opcode,					// codigo da operacao
            rs1,					// indice registrador rs
            rs2,					// indice registrador rt
            rd,						// indice registrador rd
            shamt,					// deslocamento
            funct3,					// campos auxiliares
            funct7;					// constante instrucao tipo J

int32_t *imediatos[] = {&zero, &imm12_i, &imm12_s, &imm13, &imm20_u, &imm21, &zero, &zero};

string instr_str[39];

void build_dic() {
    instr_str[I_add]	= "ADD";	instr_str[I_addi] = "ADDi";	instr_str[I_and] = "AND";
    instr_str[I_andi]	= "ANDi";	instr_str[I_auipc] = "AUIPC";
    instr_str[I_beq]	= "BEQ";	instr_str[I_bge] = "BGE";
    instr_str[I_bgeu]	= "BGEU";	instr_str[I_blt] = "BLT";	instr_str[I_bltu] = "BLTU";
    instr_str[I_bne]	= "BNE";
    instr_str[I_jal]	= "JAL";	instr_str[I_jalr] = "JALR";
    instr_str[I_lb]		= "LB";		instr_str[I_lbu] = "LBU";	instr_str[I_lh] = "LH";
    instr_str[I_lhu]	= "LHU";	instr_str[I_lui] = "LUI";	instr_str[I_lw] = "LW";
    instr_str[I_or]		= "OR";		instr_str[I_ori] = "ORi";
    instr_str[I_sb]		= "SB";		instr_str[I_sh] = "SH";		instr_str[I_sll] = "SLL";
    instr_str[I_slt]	= "SLT";	instr_str[I_slli] = "SLLi";
    instr_str[I_slti]	= "SLTi";	instr_str[I_sltiu] = "SLTIU";
    instr_str[I_sltu]	= "SLTU";	instr_str[I_srl] = "SRL";
    instr_str[I_sra]	= "SRA";	instr_str[I_srli] = "SRLi";
    instr_str[I_srai]	= "SRAi";	instr_str[I_sub] = "SUB";
    instr_str[I_sw]		= "SW";		instr_str[I_xor] = "XOR";	instr_str[I_xori] = "XORi";
    instr_str[I_ecall]  = "ECALL";  instr_str[I_nop]  = "NOP";
}

string iformat_str[8] = { "RType", "IType", "SType", "SBType", "UType", "UJType", "NullFormat", "NOPType" };

//
// identificacao dos registradores do banco do RV32I
//
string reg_str[] = {
        "ZERO", "RA",	"SP",	"GP",
        "TP",	"T0",	"T1",	"T2",
        "S0",	"S1",	"A0",	"A1",
        "A2",	"A3",	"A4",	"A5",
        "A6",	"A7",  "S2",	"S3",
        "S4",	"S5", 	"S6",	"S7",
        "S8",	"S9",  "S10",	"S11",
        "T3",	"T4",	"T5",	"T6"
};

bool stop_prg;

#endif
