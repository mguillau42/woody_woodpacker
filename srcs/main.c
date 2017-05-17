/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fventuri <fventuri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 11:45:13 by fventuri          #+#    #+#             */
/*   Updated: 2017/05/11 15:39:15 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

/* void					inject_code(void *ptr, Elf64_Ehdr *hdr, Elf64_Shdr *shdr, Elf64_Shdr *entry_shdr)*/
/* {*/
/*     void				*ref;*/
/*     int					jump_offset; // offset to jump to old entrypoint*/

/*     ref = ptr; //store the start of the section for later*/

/*     // code displaying woody*/
/*     // generated using pi.py script ty flo <3*/
/*     char shellcode[] =	"\x9c\x50\x57\x56\x54\x52\x51"*/
/*         "\xbf\x01\x00\x00\x00"*/
/*         "\x48\x8d\x35\x3e\x00\x00\x00"*/
/*         "\xba\x0f\x00\x00\x00"*/
/*         "\x48\x89\xf8"*/
/*         "\x0f\x05" // syscall (2)*/
/*         "\x48\x8d\x05"; // append here mov value equal to entry point - rip*/

/*     char shellcode_next[] = "\x48\x8b\x0d\x3e\x00\x00\x00" // mov    0x3e(%rip),%rcx*/
/*         "\x48\x8b\x15\x3f\x00\x00\x00" // mov    0x3f(%rip),%rdx*/
/*         "\x48\x01\xc1" // add    %rax,%rcx*/
/*         "\x30\x10" // xor    %dl,(%rax)*/
/*         "\x48\xff\xc0" // inc rax*/
/*         "\x48\x39\xc8" // cmp rcx, rax*/
/*         "\x75\xf6" // jmp -x0a*/
/*         "\x59\x5a\x5c\x5e\x5f\x58\x9d\xb8\x00\x00\x00\x00\x5d" // popa && popf*/
/*         "\xe9"; // jmpq instruction, insert jump value after this*/

/*     // shell code containing woody string*/
/*     char woody_string[] = "\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e"*/
/*         "\x2e\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00"; // 0x18*/

/*     // copy shellcode until mov*/
/*     ft_memcpy(ptr, shellcode, sizeof(shellcode) - 1);*/
/*     ptr += sizeof(shellcode) - 1;*/

/*     Elf64_Addr	mov_addr = shdr->sh_addr + (ptr - ref) + 4;*/
/*     int mov_offset = hdr->e_entry - mov_addr;*/
/*     int *int_buffer = (int *)ptr;*/
/*     int_buffer[0] = mov_offset;*/
/*     ptr += sizeof(int);*/

/*     // copy shellcode until jump instruction*/
/*     ft_memcpy(ptr, shellcode_next, sizeof(shellcode_next) - 1);*/
/*     ptr += sizeof(shellcode_next) - 1;*/

/*     // now the tricky part, we need to jump to the old entrypoint*/
/*     // jump instruction are relative to the current instruction*/
/*     // so first we compute the adress of the current instruction*/
/*     int			offset_inst = ptr - ref; // get the offset of the jmp instruction relatively to the start of the section's content*/
/*     // then add the offset to the virtual addr of the section*/
/*     Elf64_Addr	jump_inst = shdr->sh_addr + offset_inst + 4; // jmp's relative adress seems to take it's value in account so we add + 4*/
/*     jump_offset = hdr->e_entry - jump_inst; // and voila ! substract it to the old_entrypoint*/
/*     int_buffer = (int *)ptr;*/
/*     int_buffer[0] = jump_offset; // write it after the jump instruction. done !*/
/*     ptr += sizeof(int);*/

/*     // append the rest of the shellcode*/
/*     ft_memcpy(ptr, woody_string, sizeof(woody_string) - 1);*/
/*     ptr += sizeof(woody_string) - 1;*/

/*     // add addr / size / key*/
/*     Elf64_Addr *buf = (Elf64_Addr *)ptr;*/
/*     buf[0] = entry_shdr->sh_size;*/
/*     buf[1] = 0;*/
/* }*/

int					main(int ac, char **av)
{
	int				fd;
	void			*original;
	unsigned char	*p;
	struct stat		buf;

	if (ac != 2)
	{
		printf("usage: %s [elf64-binary]\n", av[0]);
		return (1);
	}

	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		perror("[!]");
		return (1);
	}

	if (fstat(fd, &buf) < 0)
	{
		perror("[!]");
		return (1);
	}

	if ((original = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
	{
		perror("[!]");
		return (1);
	}

	// check ELF
	p = original;
	if (!ft_memcmp(ELFMAG, original, SELFMAG) && p[EI_CLASS] == ELFCLASS64)
	{
		printf("[+] Infecting ELF64 %s\n", av[1]);
		handle_elf64(original, buf.st_size);
	}
	else if (!ft_memcmp(ELFMAG, original, SELFMAG) && p[EI_CLASS] == ELFCLASS32)
	{
		printf("[!] Provided binary is an ELF32. To implement ?\n");
		return (1);
	}
	else
	{
		printf("[!] Provided file is not an ELF binary\n");
		return (1);
	}

	// free memory
	if (munmap(original, buf.st_size))
	{
		perror("[!]");
		return (1);
	}
	close(fd);

	return (0);
}
