# teste das instrucoes logico-aritmeticas

.text
	li t0, 10
	li t1, -5
	li a7, 1
	add a0, t0, t1
	ecall			# imprime 5
	add a0, zero, t1
	ecall			# imprime -5
	add t1, t0, t1
	and a0, t0, t1
	ecall			# imprime 0
	or  a0, t0, t1
	ecall			# imprime 15
	andi a0, t0, 3
	ecall			# imprime 2
	ori a0, t0, 1
	ecall			# imprime 11
	sub a0, t0, t1
	ecall			# imprime 5
	sub a0, t1, t0
	ecall			# imprime -5
	slt a0, t1, t0 
	ecall			# imprime 1
	slt a0, t0, t1
	ecall			# imprime 0
	li  t2, 5
	li  t3, -5
	sltu a0, t3, t2 
	ecall			# imprime 0
	sltu a0, t2, t3
	ecall			# imprime 1
	xor a0, t0, t1
	ecall			# imprime 15
	slli a0, t0, 2
	ecall			# imprime 40
	li t2, -96000
	srai a0, t2, 8
	ecall			# imprime -375
	srli a0, t2, 24
	ecall			# imprime 255
	lui a0, 0x1
	ecall			# imprime 4096
	li a7, 10
	ecall
	