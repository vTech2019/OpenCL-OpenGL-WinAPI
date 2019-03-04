

.data
.code
_cpu_sse2_Gauss_function PROC
	mov ax, word ptr [rsp+40] 
	movq xmm0, rax  ;part_block_x
	mov ax, word ptr [rsp+44] 
	movq xmm1,  rax  ;part_block_y
	movss xmm2,  dword ptr [rsp+38h]  ;pow_sigma
	mov r10d, dword ptr [rsp+40h] ;pitch_image
	mov r11d, dword ptr [rsp+48h] ;aligned_offset
	xor rdi, rdi
	xor rsi, rsi
	mov edi, dword ptr [rsp+50h];blocx_x
	mov esi, dword ptr [rsp+58h]; block_y
	movq xmm5, rdi
	pshufd xmm5, xmm5, 0
	pshuflw xmm5, xmm5, 0
	pshufhw xmm5, xmm5, 0
	
	; rcx - image
	; rdx - result
	; r8 - width
	; r9 - height
	mov qword ptr [rsp-8], rbp
	mov qword ptr [rsp-16], rbx
	mov qword ptr [rsp-24], rdi
	mov qword ptr [rsp-32], rsi
	mov qword ptr [rsp-40], r12
	mov qword ptr [rsp-48], r13
	mov qword ptr [rsp-56], r14
	mov qword ptr [rsp-64], r15
	sub rsp, 64
	xor r12, r12
	xor r14, r14
	xor r15, r15
	pxor xmm3, xmm3

	
	height_jmp:
	xor r13d, r13d
	xor r15d, r15d
	cmp r14d, esi
	jne not_found_max
	xor r14d, r14d
	not_found_max:
	movq xmm3, r14
	pshufd xmm3, xmm3, 0
	pshuflw xmm3, xmm3, 0
	pshufhw xmm3, xmm3, 0
	width_jmp:

	xor rax, rax
	xor rbp, rbp
	add_data_1:
	cmp r15d, edi
	jne not_found_max_x_1
	xor r15, r15
	not_found_max_x_1:
	cmp rax, 3
	je exit_add_data_1
	add rbp, r15
	shl rbp, 16
	inc r15
	inc rax
	jmp add_data_1
	exit_add_data_1:
	
	add rbp, r15
	movq xmm4, rbp
	
	xor rbp, rbp
	add_data_2:
	cmp r15d, edi
	jne not_found_max_x_2
	xor r15, r15
	not_found_max_x_2:
	cmp rax, 7
	je exit_add_data_2
	add rbp, r15
	shl rbp, 16
	inc r15
	inc rax
	jmp add_data_2
	exit_add_data_2:

	add rbp, r15
	pslldq  xmm4, 8
	
	movq xmm5, rbp
	paddw xmm4, xmm5
	shufpd xmm4, xmm4, 0001b
	pshuflw xmm4, xmm4, 00011011b
	pshufhw xmm4, xmm4, 00011011b

	paddw xmm3, xmm1
	paddw xmm4, xmm0
	pmulhw xmm3, xmm3
	pmulhw xmm4, xmm4




	add r13d, 8
	cmp r13d, r8d
	jnae width_jmp
	inc r12d
	inc r14d
	cmp r12d, r9d
	jnae height_jmp


	mov rbp, qword ptr [rsp+56]
	mov rbx, qword ptr [rsp+48]
	mov rdi, qword ptr [rsp+40]
	mov rsi, qword ptr [rsp+32]
	mov r12, qword ptr [rsp+24]
	mov r13, qword ptr [rsp+16]
	mov r14, qword ptr [rsp+8]
	mov r15, qword ptr [rsp]
	add rsp, 64
	ret
_cpu_sse2_Gauss_function ENDP

end