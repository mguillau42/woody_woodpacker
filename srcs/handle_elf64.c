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
	if (hdr->e_shoff && hdr->e_shoff >= last->p_offset && hdr->e_shoff < (last->p_offset + last->p_memsz))	// handles stripped binary whose shstroff starts inside the data segment
	{
		printf("[!] Section header table located in last segment\n");
	len = last->p_offset + last->p_filesz;	// copy until end of DATA segment
		len = hdr->e_shoff;
	}
	else
		len = hdr->e_shoff;
	ft_memcpy(packed, original, len);
	packed += len;
	original += len;
	printf("[+] Inserting %lu free bytes at file offset %lu (%#zx)\n", code_size, len, len);
	// append rest at + code size to reserve space for our new section
	ft_memcpy(packed + code_size, original, original_size - len);
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

	packed->e_shoff += code_size;
	last = get_last_segment_64(packed);
	// if there is no section header table skip this function
	if (shdr == NULL)
		return (last->p_vaddr + last->p_memsz);

	// get the actual shdr table after inection
	printf("[+] Moving e_shoff from %1$lu (%1$#lx) to %2$lu (%2$#lx)\n", packed->e_shoff - code_size, packed->e_shoff);
	shdr = (void *)packed + packed->e_shoff;

	// fix section header table offset
	// fix sections offsets after injection
	for (int i = 0; i < packed->e_shnum; i++)
	{
		if ((long)shdr->sh_offset > ((void *)injected_section - (void *)packed))
			shdr->sh_offset += code_size;
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
				phdr->p_filesz += code_size;
			}
			phdr->p_flags = PF_X | PF_W | PF_R;
		}
		phdr = (void *)phdr + sizeof(Elf64_Phdr);
	}
}

/*
 * inject code to the new section content
 * param injected_section pointer to the injected section's content
 * param original_hdr the beginning of the original file
 * param new_entry_point the vaddr corresponding to the start of the function
 */
static void					inject_code(void *injected_section, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point)
{
	void				*ptr;
	void				*ref;
	int					jump_offset;

	ptr = injected_section;
	ref = ptr; //store the start of the section for later

	char shellcode[] =	"\xe9";

	// copy shellcode until jump instruction
	ft_memcpy(ptr, shellcode, sizeof(shellcode) - 1);
	ptr += sizeof(shellcode) - 1;

	int			offset_inst = ptr - ref; // get the offset of the jmp instruction relatively to the start of the section's content
	Elf64_Addr	jump_inst = new_entry_point + offset_inst + 4; // jmp's relative adress seems to take it's value in account so we add + 4
	jump_offset = original_hdr->e_entry - jump_inst; // and voila ! substract it to the old_entrypoint
	int *int_buffer = (int *)ptr;
	int_buffer[0] = jump_offset; // write it after the jump instruction. done !
	ptr += sizeof(int);
}

void			handle_elf64(void *original, size_t original_size)
{
	void		*packed; // packed file
	void		*injected_section; // pointer to the new section
	size_t		packed_size; // size of the packed file
	size_t		code_size; // size of the section to inject

	code_size = 4096; // to replace with actual code size !
	packed_size = original_size + code_size + sizeof(Elf64_Shdr);
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
	update_segments(packed, code_size);

	// inject & encrypt
	inject_code(injected_section, original, new_entry_point);

	// change entry point
	Elf64_Ehdr	*packed_hdr = packed;
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
