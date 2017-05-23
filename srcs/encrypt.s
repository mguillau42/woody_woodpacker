; ----------------------------------------------------------------------------- ;
;                                                                               ;
;               ╦ ╦╔═╗╔═╗╔╦╗╦ ╦  ╦ ╦╔═╗╔═╗╔╦╗╔═╗╔═╗╔═╗╦╔═╔═╗╦═╗                 ;
;               ║║║║ ║║ ║ ║║╚╦╝  ║║║║ ║║ ║ ║║╠═╝╠═╣║  ╠╩╗║╣ ╠╦╝                 ;
;               ╚╩╝╚═╝╚═╝═╩╝ ╩   ╚╩╝╚═╝╚═╝═╩╝╩  ╩ ╩╚═╝╩ ╩╚═╝╩╚═                 ;
;                                                                               ;
;                                                                               ;
;                          mguillau42 and FlorianVenturini                      ;
;             <mguillau@student.42.fr> | <fventuri@student.42.fr>               ;
;                                                                               ;
; ----------------------------------------------------------------------------- ;

section .text
	global encrypt

; void	*encrypt(void *data, size_t len, void *key);
encrypt:
	push rbp
	mov rbp, rsp
	push rdi					; Save pointer to data
	push rsi					; Save len
	push rdx					; Save pointer to the default key
	; Key expansion, xmm0-10 will store key schedule
	movdqu xmm11, [rdx]			; Store default key in xmm11
	call key_expansion
	; Set loop counter
	mov rcx, rsi
	shr rcx, 4					; len /= 16 --> number of loops to do

encrypt_loop:
	; Loop condition --> rcx > 0
	cmp rcx, 0
	je encrypt_xor
	; ENCRYPTION SEQUENCE
	movdqu xmm15, [rdi]
	pxor xmm15, xmm0			; Whitening step (Round 0)
	aesenc xmm15, xmm1			; Round 1
	aesenc xmm15, xmm2			; Round 2
	aesenc xmm15, xmm3			; Round 3
	aesenc xmm15, xmm4			; Round 4
	aesenc xmm15, xmm5			; Round 5
	aesenc xmm15, xmm6			; Round 6
	aesenc xmm15, xmm7			; Round 7
	aesenc xmm15, xmm8			; Round 8
	aesenc xmm15, xmm9			; Round 9
	aesenclast xmm15, xmm10		; Round 10
	; Write encrypted data in memory
	pextrq [rdi], xmm15, 0		; extracts bits 0-63 and writes them in memory
	pextrq [rdi + 8], xmm15, 1	; extracts bits 64-127 and writes them in memory
	add rdi, 16					; We encrypt data 16 bytes by 16 bytes
	; Iteration
	dec rcx
	jmp encrypt_loop

; In the case where the data to encrypt is not a multiple of 16 bytes, we XOR the last bytes with the default key
encrypt_xor:
	pop rdx
	mov rdx, [rdx]				; use initial key as XOR key for the last bytes (64 first bits to be exact)
	pop rcx						; get back initial len
	and rcx, 0xf				; extracts first 4 bits from len == result of len % 16

encrypt_xor_loop:
	; Loop condition
	cmp rcx, 0
	je encrypt_end
	; Computation
	xor byte [rdi], dl			; we XOR the byte in memory with the lower byte of the key
	; Iteration
	inc rdi
	dec rcx
	ror rdx, 8					; We rotate the key on right so the XOR key isn't the same for each byte (except if the key is in the form AAAAAAAA (where A is a byte))
	jmp encrypt_xor_loop

encrypt_end:
	pop rax						; get back ptr to data
	leave
	ret

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
