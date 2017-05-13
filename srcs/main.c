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

void					pack(void *m, struct stat *buf)
{
	Elf64_Ehdr			*hdr;
	Elf64_Shdr			*shdr;
	void				*packed;
	size_t				section_size;
	size_t				packed_size;

	section_size = 4096;
	packed_size = buf->st_size + section_size + sizeof(Elf64_Shdr);
	if (!(packed = (void *)malloc(packed_size)))
		return ;
	ft_bzero(packed, packed_size);

	// find bss section
	hdr = m;
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

	//
	// inject code
	ft_memcpy(new_sect, "AAAA", 4);
	//

	// add section header and update offsets
	Elf64_Shdr *new_shdr = add_shdr(packed, section_size, packed_size);
	edit_phdr(packed, section_size);

	// change header entry point
	hdr = packed;
	// force executable format
	/* hdr->e_type = 2;*/
	/* (void)new_shdr;*/
	hdr->e_entry = new_shdr->sh_addr;

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
