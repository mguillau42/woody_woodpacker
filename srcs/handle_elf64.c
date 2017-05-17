#include "woody.h"

static void			*reserve_space(void *original, void *infected, Elf64_Phdr *last, size_t original_size, size_t code_size)
{
	// copy original until end of last segment into infected
	Elf64_Ehdr		*hdr;
	size_t			len;

	hdr = original;
	if (hdr->e_shoff && hdr->e_shoff >= last->p_offset && hdr->e_shoff <= (last->p_offset + last->p_memsz))
		len = last->p_offset + last->p_filesz;
	else
		len = last->p_offset + last->p_memsz;
	ft_memcpy(infected, original, len);
	infected += len;
	original += len;
	// append rest at + code size to reserve space for our new section
	ft_memcpy(infected + code_size, original, original_size - len);
	return (infected);
}

static void *			prepare_injection(Elf64_Ehdr *original, Elf64_Ehdr *infected, size_t original_size, size_t code_size)
{
	Elf64_Phdr		*last;
	void			*injected_section;

	// find last segment
	if (!(last = get_last_segment_64(original)))
	{
		printf("[!] No segment found !\n");
		return (NULL);
	}
	injected_section = reserve_space(original, infected, last, original_size, code_size);
	return (injected_section);
}

static void				update_infected(void *injected_section, Elf64_Ehdr *infected, size_t code_size)
{
	Elf64_Shdr			*shdr;
	Elf64_Phdr			*last;

	// fix section header table offset
	infected->e_shoff += code_size;

	last = get_last_segment_64(infected);
	// fix sections offsets after injection
	shdr = (void *)infected + infected->e_shoff;
	for (int i = 0; i < infected->e_shnum; i++)
	{
		if (shdr->sh_offset > (last->p_offset + last->p_filesz))
			shdr->sh_offset += code_size;
		shdr = (void *)shdr +  sizeof(Elf64_Shdr);
	}

	// fill new section
	shdr->sh_type = SHT_PROGBITS;
	shdr->sh_flags = 6;
	shdr->sh_addr = last->p_align + ((void *)injected_section - (void *)infected);
	shdr->sh_offset = ((void *)injected_section - (void *)infected);
	shdr->sh_size = code_size;
	shdr->sh_addralign = 16;


	infected->e_shnum++;


}

void			handle_elf64(void *original, size_t original_size)
{
	void		*infected; // infected file
	void		*injected_section; // pointer to the new section
	size_t		infected_size; // size of the infected file
	size_t		code_size; // size of the section to inject

	// allocate memory for the infected file
	code_size = 4096; // to replace with actual code size !
	infected_size = original_size + code_size + sizeof(Elf64_Shdr);
	if (!(infected = (void *)malloc(infected_size)))
	{
		printf("[!] Out of memory\n");
		return ;
	}
	ft_bzero(infected, infected_size);

	injected_section = prepare_injection(original, infected, original_size, code_size);
	if (injected_section == NULL)
		return ;
	update_infected(injected_section, infected, code_size);

	// inject & encrypt
	/* inject_section(*/

	// output infected
	printf("[+] Writing infected binary\n");
	int fd;

	if ((fd = open("woody", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH )) < 0)
	{
		perror("[!]");
		return ;
	}
	write(fd, infected, infected_size);
	close(fd);
	free(infected);
}
