/* *************************************************************************** */
/*                                                                             */
/*               ╦ ╦╔═╗╔═╗╔╦╗╦ ╦  ╦ ╦╔═╗╔═╗╔╦╗╔═╗╔═╗╔═╗╦╔═╔═╗╦═╗               */
/*               ║║║║ ║║ ║ ║║╚╦╝  ║║║║ ║║ ║ ║║╠═╝╠═╣║  ╠╩╗║╣ ╠╦╝               */
/*               ╚╩╝╚═╝╚═╝═╩╝ ╩   ╚╩╝╚═╝╚═╝═╩╝╩  ╩ ╩╚═╝╩ ╩╚═╝╩╚═               */
/*                                                                             */
/*                                                                             */
/*                          mguillau42 and FlorianVenturini                    */
/*             <mguillau@student.42.fr> | <fventuri@student.42.fr>             */
/*                                                                             */
/* *************************************************************************** */

#include <woody.h>

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
	len = last->p_offset + last->p_filesz;	// copy until end of DATA segment
	ft_memcpy(packed, original, len);
	packed += len;
	original += len;

	// get bss section len
	size_t len_bss = last->p_memsz - last->p_filesz;	// copy until end of DATA segment
	packed += len_bss;
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

static int			encrypt_and_inject(void *packed, void *injected_section, void *original, Elf64_Addr new_ep, void *shellcode, void *key_param)
{
	void			*key;
	Elf64_Shdr		*entry_shdr = get_section_entry_64(packed, ((Elf64_Ehdr *)packed)->e_entry);

	if (!key_param)
	{
		key = ft_memalloc(16);
		syscall(SYS_getrandom, key, 16, 0);
	}
	else
		key = key_param;
	printf("[+] key: ");
	for (int i = 0; i < 16; ++i)
		printf("%hhX", ((unsigned char *)key)[i]);
	printf("\n");
	encrypt(packed + entry_shdr->sh_offset, entry_shdr->sh_size, key);
	if (!inject_code(injected_section, entry_shdr, original, new_ep, key, shellcode))
	{
		free(packed);
		free(key);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void			*get_shellcode(size_t *code_size)
{
	int				fd;
	void			*shellcode;
	struct stat		buf;

	if ((fd = open("./inject_me", O_RDONLY)) < 0)
	{
		printf("[!] An error occured while opening the necessary file inject_me\n");
		perror("[!]");
		return (NULL);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("[!]");
		close(fd);
		return (NULL);
	}
	if ((shellcode = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
	{
		perror("[!]");
		close(fd);
		return (NULL);
	}
	if (close(fd) < 0)
	{
		perror("[!]");
		return (NULL);
	}
	(*code_size) = buf.st_size;
	return (shellcode);
}

int				handle_elf64(void *original, size_t original_size, void *key_param)
{
	int			fd;
	void		*packed; // packed file
	void		*injected_section; // pointer to the new section
	void		*shellcode;
	size_t		packed_size; // size of the packed file
	size_t		code_size; // size of the shellcode
	Elf64_Shdr	*shdr;
	Elf64_Phdr	*last = get_last_segment_64(original);
	size_t		len_bss = (last->p_memsz - last->p_filesz);
	Elf64_Addr	new_entry_point;

	if (!(shellcode = get_shellcode(&code_size)))
		return (EXIT_FAILURE);
	packed_size = original_size + code_size + sizeof(Elf64_Shdr) + len_bss;
	if (!(packed = (void *)ft_memalloc(packed_size)))
	{
		printf("[!] Out of memory\n");
		return (EXIT_FAILURE);
	}
	if (!(injected_section = prepare_injection(original, packed, original_size, code_size)))
		return (EXIT_FAILURE);
	// get the shdr table if there is one
	if (!(shdr = get_shdr_table_64(original, original_size)))
		printf("[!] No section header table found\n");

	new_entry_point = update_packed(injected_section, packed, code_size, shdr);
	update_segments(packed, code_size + len_bss);
	if (encrypt_and_inject(packed, injected_section, original, new_entry_point, shellcode, key_param))
		return (EXIT_FAILURE);
	// change entry point
	((Elf64_Ehdr *)packed)->e_entry = new_entry_point;

	if ((fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH )) < 0)
	{
		perror("[!]");
		return (EXIT_FAILURE);
	}
	write(fd, packed, packed_size);
	free(packed);
	if (close(fd) < 0)
	{
		perror("[!]");
		return (EXIT_FAILURE);
	}
	if (munmap(shellcode, code_size))
	{
		perror("[!]");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
