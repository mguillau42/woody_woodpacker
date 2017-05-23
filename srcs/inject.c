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

static void			*get_strtab(Elf64_Ehdr *hdr)
{
	Elf64_Shdr		*sec_strtab = NULL;
	Elf64_Shdr		*shdr = (void *)hdr + hdr->e_shoff;

	for (int i = 0; i < hdr->e_shnum; ++i)
	{
		if (i != hdr->e_shstrndx && shdr->sh_type == SHT_STRTAB)
		{
			sec_strtab = shdr;
			break;
		}
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
	if (!sec_strtab)
		return (NULL);
	return ((void *)hdr + sec_strtab->sh_offset);
}

/**
 * Iterates over the shellcode's symbols and fill our custom symbols with
 * the necessary values.
 * - key : the key used for decryption
 * - to_decrypt: relative offset from the to_decrypt symbol, to the section containing the old entrypoint
 * - to_jump: relative offset from the to_jump symbol, to the original entry point
 * - len : length of the section to decrypt
 * Then writes the shellcode to the injected section's content.
 */
void	*inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point, void *key, void *shellcode)
{
	Elf64_Shdr		*symtab;
	Elf64_Shdr		*text_shdr = get_section_bytype_64(shellcode, SHT_PROGBITS);
	Elf64_Sym		*sym;
	void			*text = shellcode + text_shdr->sh_offset;
	char			*strtab = NULL;
	int				inst_value = 0;
	int				inst_offset = 0;
	int				to_write = 0;

	if (!(symtab = get_section_bytype_64(shellcode, SHT_SYMTAB)))
	{
		printf("[!] symtab not found in shellcode\n");
		return (NULL);
	}
	if (!(strtab = get_strtab(shellcode)))
	{
		printf("[!] strtab not found in shellcode\n");
	}
	sym = shellcode + symtab->sh_offset;
	while ((void *)sym < (shellcode + symtab->sh_offset + symtab->sh_size))
	{
		if (ft_strequ(strtab + sym->st_name, "key"))
			ft_memcpy((void *)text + sym->st_value, key, 16);
		else if (ft_strequ(strtab + sym->st_name, "to_decrypt"))
		{
			inst_value = sym->st_value;
			inst_offset = new_entry_point + inst_value;
			to_write = entry_shdr->sh_addr - inst_offset;
			to_write = to_write < 0 ? -to_write : to_write;
			ft_memcpy((void *)text + sym->st_value, &to_write, 4);
		}
		else if (ft_strequ(strtab + sym->st_name, "to_jump"))
		{
			inst_value = sym->st_value;
			inst_offset = new_entry_point + inst_value;
			to_write = original_hdr->e_entry - inst_offset;
			to_write = to_write < 0 ? -to_write : to_write;
			ft_memcpy((void *)text + sym->st_value, &to_write, 4);
		}
		else if (ft_strequ(strtab + sym->st_name, "len"))
			ft_memcpy((void *)text + sym->st_value, &entry_shdr->sh_size, 8);
		sym = (void *)sym + sizeof(Elf64_Sym);
	}
	ft_memcpy(injected_section, text, text_shdr->sh_size);
	return (injected_section);
}
