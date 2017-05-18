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

	// check ELF file format
	p = original;
	if (!ft_memcmp(ELFMAG, original, SELFMAG) && p[EI_CLASS] == ELFCLASS64)
	{
		printf("[+] Packing ELF64 %s\n", av[1]);
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
