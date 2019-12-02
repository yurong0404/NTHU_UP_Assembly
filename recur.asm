	mov rdi, 15
	call r
	jmp exit
r:
	mov rbx, rdi
	cmp rbx, 0
	je RETZERO
	cmp rbx, 1
	je RETONE
OTHERWISE:
	mov rdi, rbx
	sub rdi, 1
	push rbx
	call r
	pop rbx
	shl rax, 1
	mov r8, rax

	mov rdi, rbx
	sub rdi, 2
	push r8
	call r
	pop r8
	add r8, rax
	shl rax, 1
	add r8, rax
	mov rax, r8
	ret
RETZERO:
	mov rax, 0
	ret

RETONE:
	mov rax, 1
	ret
exit:
done:
