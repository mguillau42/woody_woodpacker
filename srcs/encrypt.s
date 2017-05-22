section .text
	global encrypt

; Fills registers xmm0-10 with the round keys
key_expansion:
	movdqu xmm0, xmm11
	aeskeygenassist xmm12, xmm0, 0x1
	call key_expansion_128
	movdqu xmm1, xmm11
	aeskeygenassist xmm12, xmm1, 0x2
	call key_expansion_128
	movdqu xmm2, xmm11
	aeskeygenassist xmm12, xmm2, 0x4
	call key_expansion_128
	movdqu xmm3, xmm11
	aeskeygenassist xmm12, xmm3, 0x8
	call key_expansion_128
	movdqu xmm4, xmm11
	aeskeygenassist xmm12, xmm4, 0x10
	call key_expansion_128
	movdqu xmm5, xmm11
	aeskeygenassist xmm12, xmm5, 0x20
	call key_expansion_128
	movdqu xmm6, xmm11
	aeskeygenassist xmm12, xmm6, 0x40
	call key_expansion_128
	movdqu xmm7, xmm11
	aeskeygenassist xmm12, xmm7, 0x80
	call key_expansion_128
	movdqu xmm8, xmm11
	aeskeygenassist xmm12, xmm8, 0x1b
	call key_expansion_128
	movdqu xmm9, xmm11
	aeskeygenassist xmm12, xmm9, 0x36
	call key_expansion_128
	movdqu xmm10, xmm11
	ret

; sub-function for the key expansion
key_expansion_128:
	pshufd xmm12, xmm12, 0xff
	vpslldq xmm13, xmm11, 0x4
	pxor xmm11, xmm13
	vpslldq xmm13, xmm11, 0x4
	pxor xmm11, xmm13
	vpslldq xmm13, xmm11, 0x4
	pxor xmm11, xmm13
	pxor xmm11, xmm12
	ret

; void	*encrypt(void *data, size_t len, void *key);
encrypt:
	push rbp
	mov rbp, rsp
	push rdi				; Save pointer to data
	push rsi				; Save len
	; Key expansion, xmm0-10 will store key schedule
	movdqu xmm11, [rdx]
	call key_expansion
	; Set loop counter
	mov rcx, rsi
	shr rcx, 4				; len /= 16 --> number of loops to do

encrypt_loop:
	; Loop condition --> rcx > 0
	cmp rcx, 0
	je encrypt_xor
	; ENCRYPTION SEQUENCE
	movdqu xmm15, [rdi]
	pxor xmm15, xmm0		; Whitening step (Round 0)
	aesenc xmm15, xmm1		; Round 1
	aesenc xmm15, xmm2		; Round 2
	aesenc xmm15, xmm3		; Round 3
	aesenc xmm15, xmm4		; Round 4
	aesenc xmm15, xmm5		; Round 5
	aesenc xmm15, xmm6		; Round 6
	aesenc xmm15, xmm7		; Round 7
	aesenc xmm15, xmm8		; Round 8
	aesenc xmm15, xmm9		; Round 9
	aesenclast xmm15, xmm10	; Round 10
	; Write encrypted data in memory
	pextrq r8, xmm15, 0		; extracts bits 0-63 and stores them in r8
	pextrq r9, xmm15, 1		; extracts bits 64-127 and stores them in r9
	mov [rdi], r8			; writes encrypted bytes in memory
	mov [rdi + 8], r9		; ^
	add rdi, 16
	dec rcx
	jmp encrypt_loop

encrypt_xor:
	pop rcx					; get back initial len
	and rcx, 0xf			; extracts first 4 bits from len == result of len % 16
	pextrq rdx, xmm8, 0		; set the xor key


encrypt_xor_loop:
	cmp rcx, 0
	je encrypt_end
	xor byte [rdi], dl
	inc rdi
	dec rcx
	ror rdx, 8

encrypt_end:
	pop rax					; get back ptr to data
	leave
	ret
