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

Elf64_Shdr				*add_shdr(void *packed, size_t section_size, size_t packed_size)
{
	Elf64_Ehdr			*hdr;
	Elf64_Shdr			*shdr;
	Elf64_Shdr			*new_shdr;

	hdr = packed;
	hdr->e_shoff += section_size;	// update new offset to section header table
	hdr->e_shnum++;					// increase number of sections
	hdr->e_shstrndx++;				// increase string table index since we add a new section

	// get the bss section header of the packed binary
	shdr = get_section_bytype_64(hdr, SHT_NOBITS);
	Elf64_Shdr *bss = shdr;

	// fill new section
	shdr = (void *)shdr + sizeof(Elf64_Shdr);
	shdr->sh_type = SHT_PROGBITS;
	shdr->sh_flags = 6;
	shdr->sh_addr = bss->sh_addr + bss->sh_size;
	shdr->sh_offset = bss->sh_offset + bss->sh_size;
	shdr->sh_size = section_size;
	shdr->sh_addralign = 16;
	new_shdr = shdr;

	/* update other sections offsets*/
	shdr = (void *)shdr + sizeof(Elf64_Shdr);
	while ((void *)shdr < ((void *)hdr + packed_size))
	{
		shdr->sh_offset += section_size;
		shdr = (void *)shdr +  sizeof(Elf64_Shdr);
	}
	return (new_shdr);
}

void					edit_phdr(void *packed, size_t section_size)
{
	Elf64_Phdr			*phdr;
	Elf64_Ehdr			*hdr;
	Elf64_Shdr			*bss;

	hdr = packed;
	phdr = (void *)hdr + hdr->e_phoff;
	bss = get_section_bytype_64(hdr, SHT_NOBITS); // retrieve bss shdr to check which segment contains bss
	// iterate through each program headers
	for (int i = 0; i < hdr->e_phnum; i++)
	{
		if (phdr->p_type == PT_LOAD) // if the current phdr is a loadable segment
		{
			if (phdr->p_vaddr <= bss->sh_addr && bss->sh_addr <= (phdr->p_vaddr + phdr->p_memsz)) // if the current segment contains the bss section
			{
				phdr->p_memsz += section_size; // increase it's virtual size by section_size
				phdr->p_filesz += section_size;
			}
			phdr->p_flags = PF_X | PF_W | PF_R; // 7 or all permissions
		}
		phdr = (void *)phdr + sizeof(Elf64_Phdr);
	}
}

void					inject_code(void *ptr, Elf64_Ehdr *hdr, Elf64_Shdr *shdr, Elf64_Shdr *entry_shdr)
{
	void				*ref;
	int					jump_offset; // offset to jump to old entrypoint

	ref = ptr; //store the start of the section for later

	// code displaying woody
	// generated using pi.py script ty flo <3
	char shellcode[] =	"\x9c\x50\x57\x56\x54\x52\x51"
		"\xbf\x01\x00\x00\x00"
		"\x48\x8d\x35\x3e\x00\x00\x00"
		"\xba\x0f\x00\x00\x00"
		"\x48\x89\xf8"
		"\x0f\x05" // syscall (2)
		"\x48\x8d\x05"; // append here mov value equal to entry point - rip

	char shellcode_next[] ="\x48\x8b\x0d\x3e\x00\x00\x00" // mov    0x46(%rip),%rcx
		"\x48\x8b\x15\x3f\x00\x00\x00" // mov    0x47(%rip),%rdx
		"\x48\x01\xc1" // add    %rax,%rcx
		"\x30\x10" // xor    %dl,(%rax)
		"\x48\xff\xc0" // inc rax
		"\x48\x39\xc8" // cmp rcx, rax
		"\x75\xf6" // jmp -x0a
		"\x59\x5a\x5c\x5e\x5f\x58\x9d\xb8\x00\x00\x00\x00\x5d" // popa && popf
		"\xe9"; // jmpq instruction, insert jump value after this

	// shell code containing woody string
	char woody_string[] = "\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e"
		"\x2e\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00"; // 0x18

	// copy shellcode until mov
	ft_memcpy(ptr, shellcode, sizeof(shellcode) - 1);
	ptr += sizeof(shellcode) - 1;
	Elf64_Addr	mov_addr = shdr->sh_addr + (ptr - ref) + 4; // jmp's relative adress seems to take it's value in account so we add + 4
	int mov_offset = hdr->e_entry - mov_addr; // and voila ! substract it to the old_entrypoint
	int *int_buffer = (int *)ptr;
	int_buffer[0] = mov_offset; // write it after the jump instruction. done !
	ptr += sizeof(int);

	// copy shellcode until jump instruction
	ft_memcpy(ptr, shellcode_next, sizeof(shellcode_next) - 1);
	ptr += sizeof(shellcode_next) - 1;

	// now the tricky part, we need to jump to the old entrypoint
	// jump instruction are relative to the current instruction
	// so first we compute the adress of the current instruction
	int			offset_inst = ptr - ref; // get the offset of the jmp instruction relatively to the start of the section's content
	// then add the offset to the virtual addr of the section
	Elf64_Addr	jump_inst = shdr->sh_addr + offset_inst + 4; // jmp's relative adress seems to take it's value in account so we add + 4
	jump_offset = hdr->e_entry - jump_inst; // and voila ! substract it to the old_entrypoint
	int_buffer = (int *)ptr;
	int_buffer[0] = jump_offset; // write it after the jump instruction. done !
	ptr += sizeof(int);

	// append the rest of the shellcode
	ft_memcpy(ptr, woody_string, sizeof(woody_string) - 1);
	ptr += sizeof(woody_string) - 1;

	// add addr / size / key
	Elf64_Addr *buf = (Elf64_Addr *)ptr;
	buf[0] = entry_shdr->sh_size;
	buf[1] = 42;
}

void					encrypt(Elf64_Ehdr *hdr, Elf64_Shdr *entry_shdr)
{
	unsigned char		*entry_content;

	entry_content = (void *)hdr + entry_shdr->sh_offset;
	for (unsigned long i = 0; i < entry_shdr->sh_size; i++)
	{
		entry_content[i] = entry_content[i] ^ 42;
	}
}

void					pack(void *m, struct stat *buf)
{
	Elf64_Ehdr			*hdr;
	Elf64_Shdr			*shdr;
	void				*packed;
	Elf64_Addr			old_entry;
	size_t				section_size;
	size_t				packed_size;

	section_size = 4096;
	packed_size = buf->st_size + section_size + sizeof(Elf64_Shdr);
	if (!(packed = (void *)malloc(packed_size)))
		return ;
	ft_bzero(packed, packed_size);

	// find bss section
	hdr = m;
	old_entry = hdr->e_entry;
	if (!(shdr = get_section_bytype_64(hdr, SHT_NOBITS)))
	{
		printf("[!] bss not found\n");
		return ;
	}

	// add space in order to inject sections
	// copy until end of bss section
	size_t	len_copy = shdr->sh_offset + shdr->sh_size;
	ft_memcpy(packed, m, len_copy);

	// add space for new section  by appending at +section_size
	void	*new_sect = packed + len_copy;
	// do not copy every section, stop until bss and add space for a new section
	size_t	len_remain = ((void *)hdr + buf->st_size) - ((void *)shdr + sizeof(Elf64_Shdr));
	ft_memcpy(new_sect + section_size, m + len_copy, buf->st_size - len_copy - len_remain);

	// then copy remaining sections
	size_t	new_sect_offset = buf->st_size - len_copy - len_remain;
	void	*new_sect_header = new_sect + section_size + new_sect_offset;
	ft_memcpy(new_sect_header + sizeof(Elf64_Shdr), m + len_copy + new_sect_offset, len_remain);

	// add section header and update offsets
	Elf64_Shdr *new_shdr = add_shdr(packed, section_size, packed_size);
	edit_phdr(packed, section_size);

	// change header entry point
	hdr = packed;
	hdr->e_entry = new_shdr->sh_addr;
	/* hdr->e_type = 1;*/

	// encrypt
	Elf64_Shdr		*entry_shdr = NULL;
	if (!(entry_shdr = get_section_entry_64(hdr, old_entry)))
	{
		printf("[!] entry section not found\n");
		return ;
	}
	encrypt(hdr, entry_shdr);

	// code injection time
	inject_code(new_sect, m, new_shdr, entry_shdr);



	printf("[+] generating packed file\n");
	int fd;

	if ((fd = open("woody", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH )) < 0)
	{
		perror("[!]");
		return ;
	}
	write(fd, packed, packed_size);
	close(fd);
	free(packed);
}

int					main(int ac, char **av)
{
	int				fd;
	void			*m;
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

	if ((m = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
	{
		perror("[!]");
		return (1);
	}

	// check ELF64
	p = m;
	if (ft_memcmp(ELFMAG, m, SELFMAG) && p[EI_CLASS] != ELFCLASS64)
	{
		printf("[!] Provided binary file isn't an ELF64\n");
		return (1);
	}

	print_phdr(m);
	print_shdr(m);
	// Begin code injection
	pack(m, &buf);

	// free memory
	if (munmap(m, buf.st_size))
	{
		perror("[!]");
		return (1);
	}
	close(fd);

	return (0);
}
