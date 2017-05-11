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

Elf64_Shdr	*get_section_64(Elf64_Ehdr *hdr, Elf64_Half index)
{
	Elf64_Shdr *shdr;

	shdr = (void *)hdr + hdr->e_shoff;
	for (int i = 0; i < hdr->e_shnum; i++)
	{
		if (i == index)
			return (shdr);
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
	return (NULL);
}

void	print_all(void *ptr)
{
	Elf64_Ehdr	*hdr;
	Elf64_Shdr	*shdr;
	Elf64_Shdr	*sh_strtable;
	char		*strtable;
	int			i;

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
	sh_strtable = get_section_64(hdr, hdr->e_shstrndx);
	strtable = (void *)hdr + sh_strtable->sh_offset;
	printf("Sections:\n");
	for (int i = 0; i < hdr->e_shnum; i++)
	{
		printf("\t%s\n", strtable + shdr->sh_name);
		/* if (shdr->sh_type == SHT_NOBITS)*/
			/* printf("bss found\n");*/
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
}

//int					pack(void *m, struct stat *buf)
//{
//	void			*packed;
//
//	if (!(packed = (void *)malloc(buf->st_size + 4096)))
//		return ;
//	// find bss section
//	
//	// output to file "woody"
//}

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
	//pack(m);

	// free memory
	if (munmap(m, buf.st_size))
	{
		perror("[!]");
		return (1);
	}
	close(fd);

	return (0);
}
