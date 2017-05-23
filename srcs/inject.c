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

void	*inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point, void *key)
{
	int				fd;
	char			*shellcode;
	struct stat		buf;

	// retrieve shellcode
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
	close(fd);

	// begin injection
	Elf64_Ehdr		*hdr = (void *)shellcode;
	Elf64_Shdr		*symtab = get_section_bytype_64((void *)hdr, SHT_SYMTAB);
	Elf64_Shdr		*shdr;
	Elf64_Shdr		*sh_strtab = NULL;

	shdr = (void *)hdr + hdr->e_shoff;
	for (int i = 0; i < hdr->e_shnum; ++i)
	{
		if (i != hdr->e_shstrndx && shdr->sh_type == SHT_STRTAB)
		{
			sh_strtab = shdr;
			break;
		}
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}

	char *strtab = (void *)hdr + sh_strtab->sh_offset;
	if (!symtab)
	{
		printf("[!] symtab not found\n");
		return (NULL);
	}

	Elf64_Shdr		*text_shdr = get_section_bytype_64(hdr, SHT_PROGBITS);
	void			*text = (void *)hdr + text_shdr->sh_offset;

	for (Elf64_Sym *sym = (void *)shellcode + symtab->sh_offset; (void *)sym < ((void *)shellcode + symtab->sh_offset + symtab->sh_size); sym = (void *)sym + sizeof(Elf64_Sym))
	{
		if (ft_strequ(strtab + sym->st_name, "key"))
		{
			ft_memcpy((void *)text + sym->st_value, key, 16);
		}
		else if (ft_strequ(strtab + sym->st_name, "to_decrypt"))
		{
			int	offset_inst = sym->st_value;
			Elf64_Addr	jump_inst = new_entry_point + offset_inst;
			int	jump_offset = entry_shdr->sh_addr - jump_inst;
			jump_offset = jump_offset < 0 ? -jump_offset : jump_offset;
			ft_memcpy((void *)text + sym->st_value, &jump_offset, 4);
		}
		else if (ft_strequ(strtab + sym->st_name, "to_jump"))
		{
			int	offset_inst = sym->st_value;
			Elf64_Addr	jump_inst = new_entry_point + offset_inst;
			int	jump_offset = original_hdr->e_entry - jump_inst;
			jump_offset = jump_offset < 0 ? -jump_offset : jump_offset;
			ft_memcpy((void *)text + sym->st_value, &jump_offset, 4);
		}
		else if (ft_strequ(strtab + sym->st_name, "len"))
		{
			ft_memcpy((void *)text + sym->st_value, &entry_shdr->sh_size, 8);
		}
	}

	// write section's content
	ft_memcpy(injected_section, text, text_shdr->sh_size);

	// free memory
	if (munmap((void *)shellcode, buf.st_size))
	{
		perror("[!]");
		return (NULL);
	}

	return (injected_section);
}
