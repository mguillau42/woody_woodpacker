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

static void			*load_original_binary(char *path, struct stat *buf)
{
	int				fd;
	void			*original;

	if ((fd = open(path, O_RDONLY)) < 0)
	{
		perror("[!]");
		return (NULL);
	}
	if (fstat(fd, buf) < 0)
	{
		perror("[!]");
		return (NULL);
	}
	if ((original = mmap(0, buf->st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))
			== MAP_FAILED)
	{
		perror("[!]");
		return (NULL);
	}
	if (close(fd) < 0)
	{
		perror("[!]");
		return (NULL);
	}
	return (original);
}

int					main(int ac, char **av)
{
	struct stat		buf;
	void			*original;
	unsigned char	*p;

	if (ac != 2)
	{
		printf("usage: %s [elf64-binary]\n", av[0]);
		return (EXIT_FAILURE);
	}

	if (!(original = load_original_binary(av[1], &buf)))
		return (EXIT_FAILURE);

	// check ELF file format
	p = original;
	if (!ft_memcmp(ELFMAG, original, SELFMAG) && p[EI_CLASS] == ELFCLASS64)
	{
		if (handle_elf64(original, buf.st_size))
			return (EXIT_FAILURE);
	}
	else if (!ft_memcmp(ELFMAG, original, SELFMAG) && p[EI_CLASS] == ELFCLASS32)
	{
		printf("[!] Provided binary is an ELF32. Not implemented\n");
		return (EXIT_FAILURE);
	}
	else
	{
		printf("[!] Provided file is not an ELF binary\n");
		return (EXIT_FAILURE);
	}

	if (munmap(original, buf.st_size))
	{
		perror("[!]");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
