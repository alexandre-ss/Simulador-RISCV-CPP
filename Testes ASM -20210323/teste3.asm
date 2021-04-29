# Teste para as instruções condicionais e saltos

.text 
	li t0, 2
	li a7, 1
	li a0, 1
	li s0, 10
	jal tbeq
	li a7, 10
	ecall
tbeq:
	beq t0, zero, tbne
	ecall
	addi a0, a0, 1
	li t0, 0
	j tbeq
tbne:
	bne t0, zero, tbge
	ecall 
	addi a0, a0, 1
	li t0, -1
	j tbne
tbge:
	bge t0, zero, tbgeu
	ecall
	addi a0, a0, 1
	li t0, 1
	li s0, -1
	j tbge
tbgeu:
	bgeu t0, s0, tblt
	ecall
	addi a0, a0, 1
	li t0, -1
	li s0, 0
	j tbgeu
tblt:
	blt zero, s0, tbltu
	ecall
	addi a0, a0, 1
	li s0, 1
	li t0, 0
	j tblt
tbltu:
	bltu s0, t0, fim
	ecall
	addi a0, a0, 1
	li t0, -1
	j tbltu
fim:
	nop
	nop
	ret