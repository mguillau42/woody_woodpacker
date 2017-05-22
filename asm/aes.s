section .text
	global aes
	global unaes

; Fills registers xmm0-10 with the round keys
key_expansion:
	movdqu xmm0, xmm11					; first key
	aeskeygenassist xmm12, xmm0, 0x1
	call key_expansion_128
	movdqu xmm1, xmm11					; add key
	aeskeygenassist xmm12, xmm1, 0x2
	call key_expansion_128
	movdqu xmm2, xmm11					; add key
	aeskeygenassist xmm12, xmm2, 0x4
	call key_expansion_128
	movdqu xmm3, xmm11					; add key
	aeskeygenassist xmm12, xmm3, 0x8
	call key_expansion_128
	movdqu xmm4, xmm11					; add key
	aeskeygenassist xmm12, xmm4, 0x10
	call key_expansion_128
	movdqu xmm5, xmm11					; add key
	aeskeygenassist xmm12, xmm5, 0x20
	call key_expansion_128
	movdqu xmm6, xmm11					; add key
	aeskeygenassist xmm12, xmm6, 0x40
	call key_expansion_128
	movdqu xmm7, xmm11					; add key
	aeskeygenassist xmm12, xmm7, 0x80
	call key_expansion_128
	movdqu xmm8, xmm11					; add key
	aeskeygenassist xmm12, xmm8, 0x1b
	call key_expansion_128
	movdqu xmm9, xmm11					; add key
	aeskeygenassist xmm12, xmm9, 0x36
	call key_expansion_128
	movdqu xmm10, xmm11					; add key
	ret

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

; ENCRYPT
aes:
	push rbp
	mov rbp, rsp
	push rdi				; save pointer to data
	movdqu xmm11, [rsi]
	movdqu xmm15, [rdi]
	call key_expansion
	; ENCRYPTION SEQUENCE
	pxor xmm15, xmm0 ; Whitening step (Round 0)
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

	; CHANGE DATA IN MEMORY AND RETURNS
	pextrq r8, xmm15, 0		; extracts bits 0-63 and stores them in r8
	pextrq r9, xmm15, 1		; extracts bits 64-127 and stores them in r9
	pop rax					; get back ptr to data
	mov [rax], r8			; writes encrypted bytes in memory
	mov [rax + 8], r9		; ^
	leave
	ret

; DECRYPT
unaes:
	push rbp
	mov rbp, rsp
	push rdi				; saave pointer to data
	; STORE ROUND KEYS IN xmm0-10
	movdqu xmm11, [rsi]
	movdqu xmm15, [rdi]
	call key_expansion
	; DECRYPTION SEQUENCE
	pxor xmm15, xmm10 ; Whitening step (Round 0)
	aesimc xmm9, xmm9
	aesdec xmm15, xmm9		; Round 1
	aesimc xmm8, xmm8
	aesdec xmm15, xmm8		; Round 2
	aesimc xmm7, xmm7
	aesdec xmm15, xmm7		; Round 3
	aesimc xmm6, xmm6
	aesdec xmm15, xmm6		; Round 4
	aesimc xmm5, xmm5
	aesdec xmm15, xmm5		; Round 5
	aesimc xmm4, xmm4
	aesdec xmm15, xmm4		; Round 6
	aesimc xmm3, xmm3
	aesdec xmm15, xmm3		; Round 7
	aesimc xmm2, xmm2
	aesdec xmm15, xmm2		; Round 8
	aesimc xmm1, xmm1
	aesdec xmm15, xmm1		; Round 9
	aesdeclast xmm15, xmm0	; Round 10

	; CHANGE DATA IN MEMORY AND RETURNS
	pextrq r8, xmm15, 0		; extracts bits 0-63 and stores them in r8
	pextrq r9, xmm15, 1		; extracts bits 64-127 and stores them in r9
	pop rax					; get back ptr to data
	mov [rax], r8			; writes encrypted bytes in memory
	mov [rax + 8], r9		; ^
	leave
	ret

