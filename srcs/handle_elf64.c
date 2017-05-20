#include "woody.h"

/**
 * Insert code_size null bytes after the end of the last segment, in which we
 * will inject the new section's content
 */
static void			*reserve_space(void *original, void *packed, Elf64_Phdr *last, size_t original_size, size_t code_size)
{
	// copy original until end of last segment into packed
	Elf64_Ehdr		*hdr;
	size_t			len;

	hdr = original;
	printf("[+] Section header table offset: %lu (%#lx)\n", hdr->e_shoff, hdr->e_shoff);

	len = last->p_offset + last->p_filesz;	// copy until end of DATA segment
	ft_memcpy(packed, original, len);
	packed += len;
	original += len;

	// get bss section len
	size_t len_bss = last->p_memsz - last->p_filesz;	// copy until end of DATA segment
	packed += len_bss;
	printf("[+] Inserting %lu free bytes after bss\n", len_bss);
	printf("[+] Inserting %lu free bytes at file offset %lu (%#zx)\n", code_size, len, len);
	// append rest at + code size to reserve space for our new section
	ft_memcpy(packed + code_size , original, original_size - len);
	return (packed);
}

static void *			prepare_injection(Elf64_Ehdr *original, Elf64_Ehdr *packed, size_t original_size, size_t code_size)
{
	Elf64_Phdr		*last;
	void			*injected_section;

	// find last segment
	if (!(last = get_last_segment_64(original)))
	{
		printf("[!] No segment found !\n");
		return (NULL);
	}
	printf("[+] Last segment start: %1$lu (%1$#lx) end_file: %2$lu (%2$#lx), end_mem: %3$lu (%3$#lx)\n", last->p_offset, last->p_offset + last->p_filesz, last->p_offset + last->p_memsz);
	injected_section = reserve_space(original, packed, last, original_size, code_size);
	return (injected_section);
}

/**
 * if there is a shdr table, add a new entry corresponding to our newly created
 * section. In the meantime, sections offsets have to be updated if they are
 * located after our injection offset
 */
static Elf64_Addr		update_packed(void *injected_section, Elf64_Ehdr *packed, size_t code_size, Elf64_Shdr *shdr)
{
	Elf64_Phdr			*last;

	last = get_last_segment_64(packed);
	// fix section header table offset
	packed->e_shoff += code_size + (last->p_memsz - last->p_filesz);
	// if there is no section header table skip this function
	if (shdr == NULL)
		return (last->p_vaddr + last->p_memsz);

	// get the actual shdr table after inection
	printf("[+] Moving e_shoff from %1$lu (%1$#lx) to %2$lu (%2$#lx)\n", packed->e_shoff - code_size, packed->e_shoff);
	shdr = (void *)packed + packed->e_shoff;

	// fix sections offsets after injection
	for (int i = 0; i < packed->e_shnum; i++)
	{
		if (shdr->sh_offset > (last->p_offset + last->p_filesz)) // REMOVE P_OFFSET ?
			shdr->sh_offset += code_size + (last->p_memsz - last->p_filesz) ;
		shdr = (void *)shdr +  sizeof(Elf64_Shdr);
	}

	// fill new section
	shdr->sh_type = SHT_PROGBITS;
	shdr->sh_flags = 6;
	shdr->sh_offset = ((void *)injected_section - (void *)packed);
	shdr->sh_addr = (last->p_vaddr - last->p_offset) + shdr->sh_offset;
	shdr->sh_size = code_size;
	shdr->sh_addralign = 16;

	// finally increase section number
	packed->e_shnum++;
	return (shdr->sh_addr);
}

/**
 * Change segments permissions and increase allocated memory by code_size
 */
static void				update_segments(Elf64_Ehdr *hdr, size_t code_size)
{
	Elf64_Phdr			*phdr;
	Elf64_Phdr			*last;

	phdr = (void *)hdr + hdr->e_phoff;
	last = get_last_segment_64(hdr);
	// iterate through each program headers
	for (int i = 0; i < hdr->e_phnum; i++)
	{
		if (phdr->p_type == PT_LOAD)
		{
			if (phdr == last)
			{
				phdr->p_memsz += code_size;
				phdr->p_filesz = phdr->p_memsz;
			}
			phdr->p_flags = PF_X | PF_W | PF_R;
		}
		phdr = (void *)phdr + sizeof(Elf64_Phdr);
	}
}

static void					encrypt(Elf64_Ehdr *hdr, Elf64_Shdr *entry_shdr)
{
	unsigned char		*entry_content;

	entry_content = (void *)hdr + entry_shdr->sh_offset;
	for (unsigned long i = 0; i < entry_shdr->sh_size; i++)
	{
		entry_content[i] = entry_content[i] ^ 42;
	}
}

/*
 * inject code to the new section content
 * param injected_section pointer to the injected section's content
 * param original_hdr the beginning of the original file
 * param new_entry_point the vaddr corresponding to the start of the function
 */
/* static void					inject_code(void *injected_section, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point)*/
/* {*/
/*     void				*ptr;*/
/*     void				*ref;*/
/*     int					jump_offset;*/

/*     ptr = injected_section;*/
/*     ref = ptr; //store the start of the section for later*/

/*     char shellcode[] =	"\xe9";*/

/*     // copy shellcode until jump instruction*/
/*     ft_memcpy(ptr, shellcode, sizeof(shellcode) - 1);*/
/*     ptr += sizeof(shellcode) - 1;*/

/*     int			offset_inst = ptr - ref; // get the offset of the jmp instruction relatively to the start of the section's content*/
/*     Elf64_Addr	jump_inst = new_entry_point + offset_inst + 4; // jmp's relative adress seems to take it's value in account so we add + 4*/
/*     jump_offset = original_hdr->e_entry - jump_inst; // and voila ! substract it to the old_entrypoint*/
/*     int *int_buffer = (int *)ptr;*/
/*     int_buffer[0] = jump_offset; // write it after the jump instruction. done !*/
/*     ptr += sizeof(int);*/
/* }*/

/*
 * inject code to the new section content
 * param injected_section pointer to the injected section's content
 * param original_hdr the beginning of the original file
 * param new_entry_point the vaddr corresponding to the start of the function
 */
static void					inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point)
{
	void				*ptr;
	void				*ref;
	int					jump_offset;

	ptr = injected_section;
	ref = ptr; //store the start of the section for later

	char shellcode[] =	"\x9c\x50\x57\x56\x54\x52\x51"
		"\xbf\x01\x00\x00\x00"
		"\x48\x8d\x35\x3d\x00\x00\x00"
		"\xba\x0f\x00\x00\x00" // (0x5)
		"\x48\x89\xf8" // (3)
		"\x0f\x05" // syscall (2)
		"\x48\x8d\x05"; // append here mov value equal to entry point - rip (7)

	char shellcode_next[] ="\x48\x8b\x0d\x3d\x00\x00\x00" // mov    0x46(%rip),%rcx (7)
		"\x48\x8b\x15\x3e\x00\x00\x00" // mov    0x47(%rip),%rdx (7)
		"\x48\x01\xc1" // add    %rax,%rcx (3)
		"\x30\x10" // xor    %dl,(%rax) (2)
		"\x48\xff\xc0" // inc rax (3)
		"\x48\x39\xc8" // cmp rcx, rax (3)
		"\x75\xf6" // jmp -x0a (2)
		"\x59\x5a\x5c\x5e\x5f\x58\x9d\xb8\x00\x00\x00\x00" // popa && popf (0x0c)
		"\xe9"; // jmpq instruction, insert jump value after this (5)

	// shell code containing woody string
	char woody_string[] = "\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e"
		"\x2e\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00"; // 0x18

	// copy shellcode until mov instruction
	ft_memcpy(ptr, shellcode, sizeof(shellcode) - 1);
	ptr += sizeof(shellcode) - 1;
	Elf64_Addr	mov_addr = new_entry_point + (ptr - ref) + 4; // jmp's relative adress seems to take it's value in account so we add + 4
	int mov_offset = entry_shdr->sh_addr - mov_addr; // and voila ! substract it to the old_entrypoint
	int *int_buffer = (int *)ptr;
	int_buffer[0] = mov_offset; // write it after the jump instruction. done !
	ptr += sizeof(int);

	// copy shellcode until jump instruction
	ft_memcpy(ptr, shellcode_next, sizeof(shellcode_next) - 1);
	ptr += sizeof(shellcode_next) - 1;

	int			offset_inst = ptr - ref; // get the offset of the jmp instruction relatively to the start of the section's content
	Elf64_Addr	jump_inst = new_entry_point + offset_inst + 4; // jmp's relative adress seems to take it's value in account so we add + 4
	jump_offset = original_hdr->e_entry - jump_inst; // and voila ! substract it to the old_entrypoint
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

void			handle_elf64(void *original, size_t original_size)
{
	void		*packed; // packed file
	void		*injected_section; // pointer to the new section
	size_t		packed_size; // size of the packed file
	size_t		code_size; // size of the section to inject

	code_size = 4096; // to replace with actual code size !
	Elf64_Phdr	*last = get_last_segment_64(original);
	size_t		len_bss = (last->p_memsz - last->p_filesz);
	
	packed_size = original_size + code_size + sizeof(Elf64_Shdr) + len_bss;
	if (!(packed = (void *)malloc(packed_size)))
	{
		printf("[!] Out of memory\n");
		return ;
	}
	ft_bzero(packed, packed_size);

	injected_section = prepare_injection(original, packed, original_size, code_size);
	if (injected_section == NULL)
		return ;

	// get the shdr table if there is one
	Elf64_Shdr	*shdr;
	if (!(shdr = get_shdr_table_64(original, original_size)))
		printf("[!] No section header table found\n");

	Elf64_Addr	new_entry_point = update_packed(injected_section, packed, code_size, shdr);
	printf("[+] New entry_point: %#lx\n", new_entry_point);
	update_segments(packed, code_size + len_bss);

	// inject & encrypt
	// basic encryption
	Elf64_Ehdr	*packed_hdr = packed;
	Elf64_Shdr *entry_shdr = get_section_entry_64(packed, packed_hdr->e_entry);
	printf("[+] Encrypting %1$lu (%1$#lx) - %2$lu (%2$#lx): size %3$lu (%3$#lx)\n", entry_shdr->sh_offset, entry_shdr->sh_offset + entry_shdr->sh_size, entry_shdr->sh_size);
	encrypt(packed, entry_shdr);
	inject_code(injected_section, entry_shdr, original, new_entry_point);

	// change entry point
	packed_hdr->e_entry = new_entry_point;

	// output packed file
	printf("[+] Packed file size: %lu\n", packed_size);
	printf("[+] Writing packed binary\n");
	int fd;

	if ((fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH )) < 0)
	{
		perror("[!]");
		return ;
	}
	write(fd, packed, packed_size);
	close(fd);
	free(packed);
}
