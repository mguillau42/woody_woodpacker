section .text
	global aes
	; global unaes

; ENCRYPT
aes:
	push rbp
	mov rbp, rsp
	push rdi				; saave pointer to data
	; STORE ROUND KEYS IN xmm0-10
	movdqu xmm11, [rdi]
	movdqu xmm0, xmm11					; first key
	aeskeygenassist xmm12, xmm11, 0x1
	call key_expansion_128
	movdqu xmm1, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x2
	call key_expansion_128
	movdqu xmm2, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x4
	call key_expansion_128
	movdqu xmm3, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x8
	call key_expansion_128
	movdqu xmm4, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x10
	call key_expansion_128
	movdqu xmm5, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x20
	call key_expansion_128
	movdqu xmm6, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x40
	call key_expansion_128
	movdqu xmm7, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x80
	call key_expansion_128
	movdqu xmm8, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x1b
	call key_expansion_128
	movdqu xmm9, xmm11					; add key
	aeskeygenassist xmm12, xmm11, 0x36
	call key_expansion_128
	movdqu xmm10, xmm11					; add key
	jmp aes_encrypt

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

aes_encrypt:
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
	mov [rax+8], r8			; writes encrypted bytes in memory
	mov [rax], r9		; ^
	leave
	ret

	; movdqa xmm1, [rdi]	; data stored in xmm1
	; movdqa xmm2, [rsi]	; key stored in xmm2
	; HOW TO DO THE FCKING FIRST STEP FFS WTF MAN.............
	; AddRoundKey(xmm1, xmm1[12])

	; aeskeygenassist xmm1, xmm1, 0
	; xor rcx, rcx
	; pextrb rcx, xmm2, 3
	; movq xmm3, rcx
	; pxor xmm2, xmm3
	
	; BELOW THIS, CODE SHOULD BE OK
;     mov rcx, 1

;     aesenc xmm1, xmm2
;     inc rcx
;     cmp rcx, 9
;     jl aes_loop

; aes_end:
;     aesenclast xmm1, xmm2
;     pextrq r8, xmm1, 0
;     pextrq r9, xmm1, 1
;     pop rax			; set transformed bytes
;     ; MAYBE INVERT BYTE SEQUENCE ? WE'LL SEE ONCE FIRST STEP WILL BE OK
;     mov [rax], r8
;     mov [rax + 8], r9
;     leave
;     ret

; DECRYPT
; unaes:
;     push rbp
;     mov rbp, rsp
;     push rdi		; save pointer to data
;     movdqa xmm1, [rdi]	; data
;     movdqa xmm3, [rsi]	; key
;     mov rcx, 1

; unaes_loop:
;     aesdec xmm1, xmm3
;     inc rcx
;     cmp rcx, 9
;     jl aes_loop

; unaes_end:
;     aesdeclast xmm1, xmm3
;     pextrq r8, xmm1, 0
;     pextrq r9, xmm1, 1
;     pop rax			; set transformed bytes
;     mov [rax], r8
;     mov [rax + 8], r9
;     leave
;     ret

