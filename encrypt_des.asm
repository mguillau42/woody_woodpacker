section .text
	global permute
	global get_m
	global set_m
	global rol_subkey
	global subkey_cat
	global get_block
	global replace

;uint64_t	get_m(uint8_t *msg)
get_m:
	push rbp
	mov rbp, rsp
	xor rax, rax				; ret = 0
	xor rcx, rcx				; i = 0

get_m_loop:
	shl rax, 8				; ret <<= 8
	add al, [rdi + rcx]			; ret += msg[i]
	inc rcx						; i++
	cmp rcx, 8
	jne get_m_loop				; for i < 8

get_m_end:
	leave
	ret


;void		set_m(uint8_t *msg, uint64_t m)
set_m:
	push rbp
	mov rbp, rsp
	xor rcx, rcx				; i = 0

set_m_loop:
	mov rbx, 56					; 56
	mov rax, 8					; 8
	push rcx
	mul rcx						; i * 8
	sub rbx, rax				; 56 - (8 * i)
	pop rcx
	mov r9, rsi					; tmp store second param
	push rcx
	mov rcx, rbx				; store rbx
	shr r9, cl					; m >> (56 - (8 * i))
	pop rcx
	mov [rdi + rcx], r9			; msg[i] = m >> (56 - (8 * i));
	inc rcx						; i++
	cmp rcx, 8
	jne set_m_loop				; for i < 8

set_m_end:
	leave
	ret

; uint64_t		permute(const uint64_t nbr, const uint32_t *table, size_t t_len, size_t n_len);
permute:
	push rbp ; Beginning of every function
	mov rbp, rsp ; Beginning of every function
	xor rax, rax ; Set rax to 0
	xor r9, r9 ; Set loop counter to 0

permute_loop:
	shl rax, 1 ; left shift rax by 1
	push rdi ; save rdi (nbr) for further uses
	push rcx
	mov r8, rcx
	mov cl, byte [rsi + r9] ; set r8 to table[i]
	add cx, 63 ; add 63 to r8
	sub rcx, r8 ; sub n_len to r8
	shl rdi, cl ; left shift rdi by r8 (overflow is wanted)
	shr rdi, 63 ; right shift rdi by 63 (overflow is also wanted). We extracted the table[i]-th bit with that
	add rax, rdi ; append the bit to rax
	pop rcx
	pop rdi ; Get back default rdi
	inc r9 ; increment loop counter
	cmp r9, rdx ; check if we reached the end of the loop
	jl permute_loop ; do this while r9 < rdx (we know for sure that rdx > 0)

permute_end:
	leave ; End of every function
	ret ; End of every function

; uint64_t	subkeycat(uint64_t c, uint64_t d)
subkey_cat:
	push rbp
	mov rbp, rsp
	push rbx				; woot
	mov rax, rdi			; ret = c
	shl rax, 28				; ret <<= 28
	mov rbx, rsi
	add rax, qword rbx
	pop rbx					; woot
	leave
	ret

; uint32_t	rol_subkey(uint64_t nbr, uint32_t state)
rol_subkey:
	push rbp
	mov rbp, rsp
	push rbx				; if i don't do this, rsi is fucked up next call to subkey_cat
	mov rbx, sub_rot		; load sub_rot
	mov rcx, [rbx + rsi]	; sub_rot[state]
	mov rbx, rdi
	rol rbx, cl				; flo why isn't this enough ? Because we manipulate 56bits values, not 64 :/
	mov eax, ebx
	shr rdi, 27				; i don't understand why this is necessary but ok
	add eax, edi
	and eax, 0xfffffff
	pop rbx
	leave
	ret

;uint8_t		get_block(uint64_t blocks, uint32_t i)
get_block:
	push rbp
	mov rbp, rsp
	xor rax, rax
	mov rax, rsi
	mov rcx, 6
	mul rcx					; i * 6
	add rax, 16				; 16 + (i * 6)
	mov rcx, rax
	shl rdi, cl			; blocks << (16 + (i * 6))
	shr rdi, 58
	mov rax, rdi
	leave
	ret

;uint8_t		replace(const uint8_t block, const uint32_t table[4][16])
replace:
	push rbp
	mov rbp, rsp
	xor rcx, rcx			; i = 0
	xor rdx, rdx			; j = 0
	; compute i
	mov r8, rdi				; block
	shr r8d, 5				; block >> 5
	shl r8d, 1				; (block >> 5) << 1
	mov r9, rdi				; block
	and r9d, 1				; block & 1
	add r8, r9				; addition
	mov rcx, r8				; i computed
	; compute j
	mov rdx, rdi			; j = block
	shl dl, 3				; block << 3
	shr dl, 4				; j >>= 4
	; compute index
	shl rcx, 4
	add rdx, rcx			; += i
	mov al, byte [rsi + rdx]
	leave
	ret

ft:
	push rbp
	mov rbp, rsp
	; CODE HERE
	leave
	ret

generate_keys:
	push rbp
	mov rbp, rsp
	; CODE HERE
	leave
	ret

encrypt: ; rdi = *msg / rsi = key / rdx = len
	push rbp
	mov rbp, rsp
	; CODE HERE
	leave
	ret

pc1:		db 57, 49, 41, 33, 25, 17,  9
			db  1, 58, 50, 42, 34, 26, 18
			db 10,  2, 59, 51, 43, 35, 27
			db 19, 11,  3, 60, 52, 44, 36
			db 63, 55, 47, 39, 31, 23, 15
			db  7, 62, 54, 46, 38, 30, 22
			db 14,  6, 61, 53, 45, 37, 29
			db 21, 13,  5, 28, 20, 12,  4

pc2:		db 14, 17, 11, 24,  1,  5
			db  3, 28, 15,  6, 21, 10
			db 23, 19, 12,  4, 26,  8
			db 16,  7, 27, 20, 13,  2
			db 41, 52, 31, 37, 47, 55
			db 30, 40, 51, 45, 33, 48
			db 44, 49, 39, 56, 34, 53
			db 46, 42, 50, 36, 29, 32

sub_rot:	db 1, 1, 2, 2, 2, 2, 2, 2
			db 1, 2, 2, 2, 2, 2, 2, 1

ip:			db 58, 50, 42, 34, 26, 18, 10, 2
			db 60, 52, 44, 36, 28, 20, 12, 4
			db 62, 54, 46, 38, 30, 22, 14, 6
			db 64, 56, 48, 40, 32, 24, 16, 8
			db 57, 49, 41, 33, 25, 17,  9, 1
			db 59, 51, 43, 35, 27, 19, 11, 3
			db 61, 53, 45, 37, 29, 21, 13, 5
			db 63, 55, 47, 39, 31, 23, 15, 7

fp:			db 40, 8, 48, 16, 56, 24, 64, 32
			db 39, 7, 47, 15, 55, 23, 63, 31
			db 38, 6, 46, 14, 54, 22, 62, 30
			db 37, 5, 45, 13, 53, 21, 61, 29
			db 36, 4, 44, 12, 52, 20, 60, 28
			db 35, 3, 43, 11, 51, 19, 59, 27
			db 34, 2, 42, 10, 50, 18, 58, 26
			db 33, 1, 41,  9, 49, 17, 57, 25

expand:		db 32,  1,  2,  3,  4,  5,  4,  5
			db  6,  7,  8,  9,  8,  9, 10, 11
			db 12, 13, 12, 13, 14, 15, 16, 17
			db 16, 17, 18, 19, 20, 21, 20, 21
			db 22, 23, 24, 25, 24, 25, 26, 27
			db 28, 29, 28, 29, 30, 31, 32,  1

p_box:		db 16,  7, 20, 21, 29, 12, 28, 17
			db  1, 15, 23, 26,  5, 18, 31, 10
			db  2,  8, 24, 14, 32, 27,  3,  9
			db 19, 13, 30,  6, 22, 11,  4, 25

s_box:		db 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7
			db  0, 16,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8
			db  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0
			db 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
			db 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10
			db  3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5
			db  0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15
			db 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
			db 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8
			db 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1
			db 13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7
			db  1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
			db  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15
			db 13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9
			db 10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4
			db  3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
			db  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9
			db 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6
			db  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14
			db 11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
			db 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11
			db 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8
			db  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6
			db  4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
			db  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1
			db 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6
			db  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2
			db  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
			db 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7
			db  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2
			db  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8
			db  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
