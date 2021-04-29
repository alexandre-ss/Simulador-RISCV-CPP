/*
 *  riscv.cpp
 *
 * Instructions: (v1.0)
 *   add,	addi,   and,  andi,   auipc,
 *   beq,   bne,    jal,  jalr,   lb,
 *   lbu,   lw,     lui,  nop,    or,
 *   ori,   sb,     slli, slt,    sltu,
 *   srai,  srli,   sub,  sw,     xor,
 *   ecall
 */

//
// Initial values for registers
//

void init() {
    pc = 0; ri = 0;
    stop_prg = false;
    sp = 0x3ffc;
    gp = 0x1800;
    build_dic();
}

// Imprime conteudo do banco de registradores
//
void dump_breg() {
	for (int i=0; i<32; i++) {
		printf("BREG[%s] = \t%8d \t\t\t%8x\n", reg_str[i].c_str(), breg[i], breg[i]);
	}
}

// Carrega um arquivo binario para a memoria
//
int load_mem(const char *fn, int start) {
	FILE *fptr;
	int *m_ptr = mem + (start>>2);  
	int size = 0;
	
	fptr = fopen(fn, "rb");
	if (!fptr) {
		printf("Arquivo nao encontrado!\n");
		return -1;
	}
	else {
		while (!feof(fptr)) {
			fread(m_ptr, 4, 1, fptr);
			m_ptr++;
			size++;
		}
		fclose(fptr);
	}
	return size;
}

// Determina o formato da intrucao
//
FORMATS get_i_format(uint32_t opcode, uint32_t func3, uint32_t func7) {
    switch(opcode) {
        case 0x33 :  return RType;
        case 0x03: case 0x13: case 0x67: case 0x73:
            return IType;
        case 0x23 :
            return SType;
        case 0x63 :
            return SBType;
        case 0x37: case 0x17:
            return UType;
        case 0x6F: 
            return UJType;
        case 0x00:
            if (func3 == 0 && func7 == 0)
                return NOPType;
            else
                return NullFormat;
        default:
            cout << "Undefined Format";
            return NullFormat;
            break;
    }
}

// Determina a instrucao a ser executada

INSTRUCTIONS get_instr_code(uint32_t opcode, uint32_t func3, uint32_t func7) {
    switch (opcode) {
        case LUI:   return I_lui;
        case AUIPC: return I_auipc;
        case BType:
            switch (funct3) {
                case BEQ3:   return I_beq;
                case BNE3:   return I_bne;
                case BGE3:   return I_bge;
                case BGEU3:  return I_bgeu;
                case BLT3:   return I_blt;
                case BLTU3:  return I_bltu;
            }
            break;
        case ILType:
            switch (funct3) {
                case LB3:   return I_lb;
                case LW3:   return I_lw;
                case LBU3:  return I_lbu;
                default:    break;
            }
            break;
        case JAL:       return I_jal;
        case JALR:      return I_jalr;
        case StoreType:
            switch (funct3) {
                case SB3:   return I_sb;
                case SW3:   return I_sw;
                default:    break;
            }
            break;
        case ILAType:
            switch (funct3) {
                case ADDI3:     return I_addi;
                case ORI3:      return I_ori;
                case ANDI3:     return I_andi;
                case SLLI3:     return I_slli;
                case SRI3:   
                    switch(funct7){
                        case SRAI7:   return I_srai;
                        case SRLI7:   return I_srli;
                        default:    break;
                    }
                break;
                default:    break;
            }
            break;
        case RegType:
            switch (funct3) {
                case ADDSUB3:
                    if (funct7 == SUB7)     return I_sub;
                    else                    return I_add;
                case SLL3:                  return I_sll;
                case SLT3:                  return I_slt;
                case SLTU3:                 return I_sltu;
                case XOR3:                  return I_xor;
                case OR3:                   return I_or;
                case AND3:                  return I_and;
                case SR3:
                    if (funct7 == SRA7)     return I_sra;
                    else                    return I_srl;
                default:
                    break;
            }
            break;
        case ECALL:     return I_ecall;
        default:
            printf("Instrucao Invalida (PC = %08x RI = %08x)\n", pc, ri);
            break;
    }
    return I_nop;
}


// Instrucao decodificada
//
INSTRUCTIONS instruction;



/************************************ FETCH **********************************/
void fetch () {
    ri = lw(pc, 0);
    pc = pc + 4;
}

/*********************************** DECODE **********************************/

void decode () {
    int32_t tmp;
    opcode	= ri & 0x7F;            // codigo da instrucao
    rs2		= (ri >> 20) & 0x1F;    // segundo operando
    rs1		= (ri >> 15) & 0x1F;    // primeiro operando
    rd		= (ri >> 7)  & 0x1F;    // registrador destino
    shamt	= (ri >> 20) & 0x1F;    // deslocamento
    funct3	= (ri >> 12) & 0x7;     // auxiliar codigo instrucao
    funct7  = (ri >> 25);           // auxiliar codigo de instrucao
    imm12_i = ((int32_t)ri) >> 20;  // imediato 12 bits
    tmp     = get_field(ri, 7, 0x1f);
    imm12_s = set_field(imm12_i, 0, 0x1f, tmp);
    imm13   = imm12_s;
    imm13   = set_bit(imm13, 11, imm12_s&1);
    imm13   = imm13 & ~1;
    imm20_u = ri & (~0xFFF);
    // mais aborrecido: imediato 21 bits
    imm21   = (int32_t)ri >> 11;			// estende sinal
    tmp     = get_field(ri, 12, 0xFF);		// le campo 19:12
    imm21   = set_field(imm21, 12, 0xFF, tmp);	// escreve campo em imm21
    tmp     = get_bit(ri, 20);				// le o bit 11 em ri(20)
    imm21   = set_bit(imm21, 11, tmp);			// posiciona bit 11
    tmp     = get_field(ri, 21, 0x3FF);
    imm21   = set_field(imm21, 1, 0x3FF, tmp);
    imm21   = imm21 & ~1;					// zera bit 0

    instruction = get_instr_code(opcode, funct3, funct7);
    imm32_t = *imediatos[get_i_format(opcode, funct3, funct7)];
}
int i_word = 0;
char *letra;

void execute () {
    switch(instruction){
        case I_add: 
            breg[rd] = breg[rs1] + breg[rs2];
            break;
        case I_addi: 
            breg[rd] = breg[rs1] + imm32_t;
            break;
        case I_and: 
            breg[rd] = breg[rs1]&breg[rs2];
            break;
        case I_andi: 
            breg[rd] = breg[rs1]&imm32_t;
            break;
        case I_auipc: 
            breg[rd] = pc - 4 + imm32_t;
            break;
        case I_beq: 
            if(breg[rs1] == breg[rs2]){
                pc = pc - 4 + imm32_t;
            }
            break;
        case I_bge: 
            if(breg[rs1] >= breg[rs2]){
                pc = pc - 4 + imm32_t;
            }
            break;
        case I_bgeu: 
            if((unsigned)breg[rs1] >= (unsigned)breg[rs2]){
                pc = pc - 4 + imm32_t;
            }
            break;
        case I_blt: 
            if(breg[rs1] < breg[rs2]){
                pc = pc - 4 + imm32_t;
            }
            break;
        case I_bltu: 
            if((unsigned)breg[rs1] < (unsigned)breg[rs2]){
                pc = pc - 4 + imm32_t;
            }
            break;
        case I_bne:   
            if(breg[rs1] != breg[rs2]){
                pc = pc - 4 + imm32_t;
            }
            break;
        case I_jal: 
            breg[rd] = pc;
            pc = pc - 4 + imm32_t;
            breg[ZERO] = 0;
            break;
        case I_jalr:
            breg[rd] = pc; 
            pc = imm32_t + breg[rs1];
            breg[ZERO] = 0;
            break;
        case I_nop:
            break;
        case I_sltu:
            if((unsigned)breg[rs1] < (unsigned)breg[rs2]){
                breg[rd] = 1;
            }
            else{
                breg[rd] = 0;
            }
            break;
        case I_or: 
            breg[rd] = breg[rs1] | breg[rs2];
            break;
        case I_ori:
            breg[rd] = breg[rs1] | imm32_t;
            break;
        case I_slt: 
            if(breg[rs1] < breg[rs2]){
                breg[rd] = 1;
            }
            else{
                breg[rd] = 0;
            }
            break;
        case I_lb: 
            breg[rd] = lb(breg[rs1], imm32_t);
            break;
        case I_lbu: 
            breg[rd] = lbu(breg[rs1], imm32_t);
            break;
        case I_lw:
            breg[rd] = lw(breg[rs1], imm32_t);
            break;
        case I_lui: 
            breg[rd] = imm32_t;
            break;
        case I_sb: 
            sb(breg[rs1], imm32_t, breg[rs2]);
            break;
        case I_sw:
            sw(breg[rs1], imm32_t, breg[rs2]);
            break;
        case I_slli:
            breg[rd] = breg[rs1] << imm32_t;
            break;            
        case I_srai:
            breg[rd] = (signed)breg[rs1] >> imm32_t;
            break;
        case I_srli:
            breg[rd] = (unsigned)breg[rs1] >> imm32_t;
            break;
        case I_sub: 
            breg[rd] = breg[rs1] - breg[rs2];
            break;
        case I_xor: 
            breg[rd] = breg[rs1] ^ breg[rs2];
            break;
        case I_ecall:
            switch(breg[A7]){
                case 1:
                    printf("%d\n", breg[A0]);
                    break;
                case 4: 
                    letra = (char*)mem + breg[A0];
					i_word = 0;  
                    while(*(letra + i_word) != '\0'){
                        cout << *(letra+i_word);
                        i_word++;
                    }
                    break;
                case 10:
                    stop_prg = 1;
                    break;
                default:
                break;
            }
            break;
        default:
            cout << "command not found";    
            break;
    }
}

void step() {
	fetch();
	decode();
    execute();
}

void run() {
	init();
	while ((pc < DATA_SEGMENT_START) && !stop_prg)
        step();
}
