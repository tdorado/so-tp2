GLOBAL _ioRead
GLOBAL _ioWrite

section .text

; _ioRead -- Reads in port
; IN:	RDI = port
; OUT:	RAX = result from port read
_ioRead:
	push rbp
	mov rbp, rsp

	xor rax, rax

	push rdx

	mov rdx, rdi
	
	in al, dx

	pop rdx

	pop rbp
	ret

; _ioWrite -- Writes in port
; IN:	RDI = port
;		RSI = data for port
; OUT:	void
_ioWrite:
	push rbp
	mov rbp, rsp

	push rdx
	push rax

	mov rdx, rdi
	mov rax, rsi

	out dx, al

	pop rax
	pop rdx

	pop rbp
	ret