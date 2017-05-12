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

Elf64_Shdr			*get_section_64(Elf64_Ehdr *hdr, Elf64_Half index)
{
	Elf64_Shdr		*shdr;

	shdr = (void *)hdr + hdr->e_shoff;
	for (int i = 0; i < hdr->e_shnum; i++)
	{
		if (i == index)
			return (shdr);
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
	return (NULL);
}

void				print_all(void *ptr)
{
	Elf64_Ehdr		*hdr;
	Elf64_Shdr		*shdr;
	Elf64_Shdr		*sh_strtable;
	char			*strtable;
	int				i;

	hdr = ptr;
	printf("Header:\n\te_ident: ");
	for (i = 0; i < EI_NIDENT; ++i) {
		printf("%02X ", hdr->e_ident[i]);
	}
	printf("\n\te_type: %hu (%hX)", hdr->e_type, hdr->e_type);
	printf("\n\te_machine: %hu (%hX)", hdr->e_machine, hdr->e_machine);
	printf("\n\te_version: %u (%X)", hdr->e_version, hdr->e_version);
	printf("\n\te_entry: %lu (%lX)", hdr->e_entry, hdr->e_entry);
	printf("\n\te_phoff: %lu (%lX)", hdr->e_phoff, hdr->e_phoff);
	printf("\n\te_shoff: %lu (%lX)", hdr->e_shoff, hdr->e_shoff);
	printf("\n\te_flags: %u (%X)", hdr->e_flags, hdr->e_flags);
	printf("\n\te_ehsize: %hu (%hX)", hdr->e_ehsize, hdr->e_ehsize);
	printf("\n\te_phentsize: %hu (%hX)", hdr->e_phentsize, hdr->e_phentsize);
	printf("\n\te_phnum: %hu (%hX)", hdr->e_phnum, hdr->e_phnum);
	printf("\n\te_shentsize: %hu (%hX)", hdr->e_shentsize, hdr->e_shentsize);
	printf("\n\te_shnum: %hu (%hX)", hdr->e_shnum, hdr->e_shnum);
	printf("\n\te_shstrndx: %hu (%hX)\n", hdr->e_shstrndx, hdr->e_shstrndx);

	shdr = (void *)hdr + hdr->e_shoff;
	// get shstrtable section
	sh_strtable = get_section_64(hdr, hdr->e_shstrndx);
	strtable = (void *)hdr + sh_strtable->sh_offset;
	printf("Sections:\n");
	for (int i = 0; i < hdr->e_shnum; i++)
	{
		printf("\n\n\t%s:\n", strtable + shdr->sh_name);

		printf("sh_name: %u\n", shdr->sh_name);
		printf("sh_type: %u\n", shdr->sh_type);
		printf("sh_flags: %lu\n", shdr->sh_flags);
		printf("sh_addr: %#lx\n", shdr->sh_addr);
		printf("sh_offset: %lu\n", shdr->sh_offset);
		printf("sh_size: %lu\n", shdr->sh_size);
		printf("sh_link: %u\n", shdr->sh_link);
		printf("sh_info: %u\n", shdr->sh_info);
		printf("sh_addralign: %lu\n", shdr->sh_addralign);
		printf("sh_entsize: %lu\n", shdr->sh_entsize);

		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
}

Elf64_Shdr				*get_section_bytype_64(Elf64_Ehdr *hdr, Elf64_Word type)
{
	Elf64_Shdr			*shdr;

	shdr = (void *)hdr + hdr->e_shoff;
	for (int i = 0; i < hdr->e_shnum; i++)
	{
		if (shdr->sh_type == type)
			return (shdr);
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
	return (shdr);
}

        /* .text:*/
		/* sh_name: 157*/
		/* sh_type: 1*/
		/* sh_flags: 6*/
		/* sh_addr: 0x580*/
		/* sh_offset: 1408*/
		/* sh_size: 450*/
		/* sh_link: 0*/
		/* sh_info: 0*/
		/* sh_addralign: 16*/
		/* sh_entsize: 0*/

void					add_shdr(void *packed, size_t section_size, size_t packed_size)
{
	Elf64_Ehdr			*hdr;
	Elf64_Shdr			*shdr;

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
	shdr->sh_addralign = 1;

	// update other sections offsets
	shdr = (void *)shdr + sizeof(Elf64_Shdr);
	while ((void *)shdr < ((void *)hdr + packed_size))
	{
		shdr->sh_offset += section_size;
		shdr = (void *)shdr +  sizeof(Elf64_Shdr);
	}
}

void					edit_phdr(void *packed, size_t section_size)
{
	//
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
	//

	// add section header and update offsets
	add_shdr(packed, section_size, packed_size);
	edit_phdr(packed, section_size);

	// change header entry point

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

	print_all(m);
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
