#include "woody.h"

/*
 * inject code to the new section content
 * param injected_section pointer to the injected section's content
 * param original_hdr the beginning of the original file
 * param new_entry_point the vaddr corresponding to the start of the function
 */
/* void					inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point)*/
/* {*/
/*     void				*ptr;*/
/*     void				*ref;*/
/*     int					jump_offset;*/

/*     ptr = injected_section;*/
/*     ref = ptr; //store the start of the section for later*/

/*     char shellcode[] =	"\x9c\x50\x57\x56\x54\x52\x51"*/
/*         "\xbf\x01\x00\x00\x00"*/
/*         "\x48\x8d\x35\x3d\x00\x00\x00"*/
/*         "\xba\x0f\x00\x00\x00" // (0x5)*/
/*         "\x48\x89\xf8" // (3)*/
/*         "\x0f\x05" // syscall (2)*/
/*         "\x48\x8d\x05"; // append here mov value equal to entry point - rip (7)*/

/*     char shellcode_next[] ="\x48\x8b\x0d\x3d\x00\x00\x00" // mov    0x46(%rip),%rcx (7)*/
/*         "\x48\x8b\x15\x3e\x00\x00\x00" // mov    0x47(%rip),%rdx (7)*/
/*         "\x48\x01\xc1" // add    %rax,%rcx (3)*/
/*         "\x30\x10" // xor    %dl,(%rax) (2)*/
/*         "\x48\xff\xc0" // inc rax (3)*/
/*         "\x48\x39\xc8" // cmp rcx, rax (3)*/
/*         "\x75\xf6" // jmp -x0a (2)*/
/*         "\x59\x5a\x5c\x5e\x5f\x58\x9d\xb8\x00\x00\x00\x00" // popa && popf (0x0c)*/
/*         "\xe9"; // jmpq instruction, insert jump value after this (5)*/

/*     // shell code containing woody string*/
/*     char woody_string[] = "\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e"*/
/*         "\x2e\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00"; // 0x18*/

/*     // copy shellcode until mov instruction*/
/*     ft_memcpy(ptr, shellcode, sizeof(shellcode) - 1);*/
/*     ptr += sizeof(shellcode) - 1;*/
/*     Elf64_Addr	mov_addr = new_entry_point + (ptr - ref) + 4; // jmp's relative adress seems to take it's value in account so we add + 4*/
/*     int mov_offset = entry_shdr->sh_addr - mov_addr; // and voila ! substract it to the old_entrypoint*/
/*     int *int_buffer = (int *)ptr;*/
/*     int_buffer[0] = mov_offset; // write it after the jump instruction. done !*/
/*     ptr += sizeof(int);*/

/*     // copy shellcode until jump instruction*/
/*     ft_memcpy(ptr, shellcode_next, sizeof(shellcode_next) - 1);*/
/*     ptr += sizeof(shellcode_next) - 1;*/

/*     int			offset_inst = ptr - ref; // get the offset of the jmp instruction relatively to the start of the section's content*/
/*     Elf64_Addr	jump_inst = new_entry_point + offset_inst + 4; // jmp's relative adress seems to take it's value in account so we add + 4*/
/*     jump_offset = original_hdr->e_entry - jump_inst; // and voila ! substract it to the old_entrypoint*/
/*     int_buffer = (int *)ptr;*/
/*     int_buffer[0] = jump_offset; // write it after the jump instruction. done !*/
/*     ptr += sizeof(int);*/

/*     // append the rest of the shellcode*/
/*     ft_memcpy(ptr, woody_string, sizeof(woody_string) - 1);*/
/*     ptr += sizeof(woody_string) - 1;*/

/*     // add addr / size / key*/
/*     Elf64_Addr *buf = (Elf64_Addr *)ptr;*/
/*     buf[0] = entry_shdr->sh_size;*/
/*     buf[1] = 42;*/

/* }*/


/* void	inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point)*/
/* {*/
/*     (void)include*/
/* }*/
