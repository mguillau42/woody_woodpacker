#include "woody.h"

//#define START_ADRESS (unsigned int) 0x08048000
#define START_ADRESS (unsigned int) 0x0000000000400000
#define CODE_OFFSET (phdr->p_offset + phdr->p_memsz)
#define CODE_ADRESS (START_ADRESS + CODE_OFFSET)

void				inject(void *m, struct stat *buf)
{
	Elf64_Ehdr		*ehdr;
	Elf64_Phdr		*phdr;

	ehdr = (void*)m;
	phdr = (void*)f_mmaped + ehdr->e_phoff;
	printf("[+] Find free space\n");
	for(i = 0; i < ehdr->e_phnum - 1; i++)
	{
		if(phdr->p_type == PT_LOAD)
			break;
		phdr++;
	}
	if(next->p_type != PT_LOAD || phdr->p_type != PT_LOAD)
	{
		printf("[!] Didn't find two PT_LOAD\n");
		return ;
	}

	if(phdr->p_memsz != phdr->p_filesz || (CODE_OFFSET + 47) > (next->p_offset + phdr->p_offset))
	{
		printf("[!] Didn't find enough free space\n");
		return ;
	}
	printf("[+] Free space found : %d bytes.n", (next->p_offset) - (CODE_OFFSET));
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
	if (ft_memcmp(ELFMAG, m, SELFMAG) && p[EI_CLASS] != ELFCLASS64)
	{
		printf("[!] Provided binary file isn't an ELF64\n");
		return (1);
	}

	// Begin code injection
	inject(m);
	// output to file "woody"

	// free memory
	if (munmap(m, buf.st_size))
	{
		perror("[!]");
		return (1);
	}
	close(fd);

	return (0);
}
