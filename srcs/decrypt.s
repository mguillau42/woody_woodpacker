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

decrypt:
	; Save all flags + registers 
	pushf
	push rdi
	push rsi
	push rax
	push rbx
	push rcx
	push rdx
	; Print "....WOODY....."
	mov rax, 1						; syscall write
	mov rdi, rax					; stdout
	lea rsi, [rel str_woody]		; ptr to the string to write
	mov rdx, 16						; length of the string
	syscall
	; Key expansion --> xmm0-10 will store key schedule
	movdqu xmm11, [rel key]			; Store default key in xmm11
	call key_expansion
	; Set loop counter
	mov rcx, [rel len]				; Get size of section to decrypt
	push rcx						; Save len in stack
	shr rcx, 4						; len /= 16 --> number of loops to do
	lea rdi, [rel to_decrypt]		
	sub rdi, [rdi]

decrypt_loop:
	; Loop condition --> rcx > 0
	cmp rcx, 0
	je decrypt_xor
	; DECRYPTION SEQUENCE
	movdqu xmm15, [rdi]
	pxor xmm15, xmm10			; Whitening step (Round 0)
	aesdec xmm15, xmm9			; Round 1
	aesdec xmm15, xmm8			; Round 2
	aesdec xmm15, xmm7			; Round 3
	aesdec xmm15, xmm6			; Round 4
	aesdec xmm15, xmm5			; Round 5
	aesdec xmm15, xmm4			; Round 6
	aesdec xmm15, xmm3			; Round 7
	aesdec xmm15, xmm2			; Round 8
	aesdec xmm15, xmm1			; Round 9
	aesdeclast xmm15, xmm0		; Round 10
	; Write decrypted data in memory
	pextrq [rdi], xmm15, 0		; extracts bits 0-63 and writes them in memory
	pextrq [rdi + 8], xmm15, 1	; extracts bits 64-127 and writes them in memory
	; Iteration
	add rdi, 16
	dec rcx
	jmp decrypt_loop

; In the case where the data to decrypt is not a multiple of 16 bytes, we XOR the last bytes with the default key
decrypt_xor:
	mov rdx, [rel key]
	pop rcx						; get back initial len
	and rcx, 0xf				; extracts first 4 bits from len (which is the result of len % 16)

decrypt_xor_loop:
	; Loop condition
	cmp rcx, 0
	je decrypt_end
	; Computation
	xor byte [rdi], dl			; we XOR the byte in memory with the lower byte of the key
	; Iteration
	inc rdi
	dec rcx
	ror rdx, 8					; We rotate the key on right so the XOR key isn't the same for each byte (except if the key is in the form AAAAAAAA (where A is a byte))
	jmp decrypt_xor_loop

decrypt_end:
	; Get back all previously saved registers / flags
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rsi
	pop rdi
	popf
	; Set address to jump now that we decrypted
	lea r8, [rel to_jump]
	sub r8, [r8]
	jmp r8

; Fills registers xmm0-10 with the round keys
key_expansion:
	movdqu xmm0, xmm11
	aeskeygenassist xmm12, xmm0, 0x1
	call key_expansion_128
	movdqu xmm1, xmm11
	aeskeygenassist xmm12, xmm1, 0x2
	aesimc xmm1, xmm1
	call key_expansion_128
	movdqu xmm2, xmm11
	aeskeygenassist xmm12, xmm2, 0x4
	aesimc xmm2, xmm2
	call key_expansion_128
	movdqu xmm3, xmm11
	aeskeygenassist xmm12, xmm3, 0x8
	aesimc xmm3, xmm3
	call key_expansion_128
	movdqu xmm4, xmm11
	aeskeygenassist xmm12, xmm4, 0x10
	aesimc xmm4, xmm4
	call key_expansion_128
	movdqu xmm5, xmm11
	aeskeygenassist xmm12, xmm5, 0x20
	aesimc xmm5, xmm5
	call key_expansion_128
	movdqu xmm6, xmm11
	aeskeygenassist xmm12, xmm6, 0x40
	aesimc xmm6, xmm6
	call key_expansion_128
	movdqu xmm7, xmm11
	aeskeygenassist xmm12, xmm7, 0x80
	aesimc xmm7, xmm7
	call key_expansion_128
	movdqu xmm8, xmm11
	aeskeygenassist xmm12, xmm8, 0x1b
	aesimc xmm8, xmm8
	call key_expansion_128
	movdqu xmm9, xmm11
	aeskeygenassist xmm12, xmm9, 0x36
	aesimc xmm9, xmm9
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

; RODATA
str_woody:	db "....WOODY.....", 10, 0							; Woody string (16 bytes long)
key:		db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	; 128-bit key
to_decrypt:	db 0, 0, 0, 0, 0, 0, 0, 0							; start of the encrypted section
len:		db 0, 0, 0, 0, 0, 0, 0, 0							; len of the section to decrypt (in bytes)
to_jump:	db 0, 0, 0, 0, 0, 0, 0, 0							; original entry point


