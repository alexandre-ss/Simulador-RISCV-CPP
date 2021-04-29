# testa acesso a memoria

.data
w0:	.word 0x01F203F4
w1:	.word 0


.text
	li a7, 1
	la s0, w0
	lb a0, 0(s0)
	ecall			# imprime -12
	la s1, w1
	sb a0, 0(s1)
	lw a0, 0(s1)
	ecall			# imprime 244
	lbu a0, 0(s0)
	ecall			# imprime 244
	lb a0, 1(s0)
	ecall			# imprime 3
	lw a0, 0(s0)
	sw a0, 0(s1)
	lb a0, 3(s1)
	ecall			# imprime 1
    li a7,10
    ecall
	
	
