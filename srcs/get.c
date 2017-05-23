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
	return (NULL);
}

Elf64_Shdr				*get_section_entry_64(Elf64_Ehdr *hdr, Elf64_Addr entry)
{
	Elf64_Shdr			*shdr;

	shdr = (void *)hdr + hdr->e_shoff;
	for (int i = 0; i < hdr->e_shnum; i++)
	{
		if (entry >= shdr->sh_addr && entry < (shdr->sh_addr + shdr->sh_size))
			return (shdr);
		shdr = (void *)shdr + sizeof(Elf64_Shdr);
	}
	return (NULL);
}

Elf64_Phdr				*get_last_segment_64(Elf64_Ehdr *hdr)
{
	Elf64_Phdr		*last;
	Elf64_Phdr		*phdr;

	last = NULL;
	if (!hdr->e_phoff)
		return (NULL);
	phdr = (void *)hdr + hdr->e_phoff;
	for (int i = 0; i < hdr->e_phnum; i++)
	{
		if (phdr->p_type == PT_LOAD)
		{
			if (last == NULL)
				last = phdr;
			else if (last->p_offset <= phdr->p_offset)
				last = phdr;
		}
		phdr = (void *)phdr + sizeof(Elf64_Phdr);
	}
	return (last);
}

Elf64_Shdr				*get_shdr_table_64(Elf64_Ehdr *hdr, size_t original_size)
{
	void				*tmp;

	tmp = (void *)hdr + hdr->e_shoff;
	if (!hdr->e_shoff)
		return (NULL);
	if (tmp < (void *)hdr || (tmp + (hdr->e_shentsize * hdr->e_shnum)) > (void *)hdr + original_size) // check oob
		return (NULL);
	return ((void *)hdr + hdr->e_shoff);
}
