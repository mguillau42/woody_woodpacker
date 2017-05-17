#include "woody.h"

Elf64_Shdr			*get_section_byindex_64(Elf64_Ehdr *hdr, Elf64_Half index)
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
		if (entry >= shdr->sh_addr && entry <= (shdr->sh_addr + shdr->sh_size))
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
