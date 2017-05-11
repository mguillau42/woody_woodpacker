/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fventuri <fventuri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 11:45:13 by fventuri          #+#    #+#             */
/*   Updated: 2017/05/11 11:56:19 by fventuri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

void	print_all(void *ptr)
{
	Elf64_Ehdr	*hdr;

}

int		main(int ac, char **av)
{
	int			fd;
	struct stat	s;
	void		*ptr;

	if (ac < 2)
		return (1);
	if ((fd = open(av[1], O_RDONLY)) < 0)
		return (1);
	if (fstat(fd, &s) < 0)
		return (1);
	if ((ptr = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (1);

	print_all(ptr);

	if (munmap(ptr, s.st_size) < 0)
		return (1);
	if (close(fd) < 0)
		return (1);
	return (0);
}
