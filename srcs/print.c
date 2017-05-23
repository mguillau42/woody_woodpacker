#include "woody.h"

void				print_all(void *ptr)
{
	print_hdr(ptr);
	print_phdr(ptr);
	print_shdr(ptr);
}

void				print_hdr(void *ptr)
{
	Elf64_Ehdr		*hdr;

	hdr = ptr;
	printf("Header:\n\te_ident: ");
	for (int i = 0; i < EI_NIDENT; ++i) {
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
}

void				print_shdr(void *ptr)
{
	Elf64_Ehdr		*hdr;
	Elf64_Shdr		*shdr;
	Elf64_Shdr		*sh_strtable;
	char			*strtable;

	hdr = ptr;
	shdr = (void *)hdr + hdr->e_shoff;
	sh_strtable = get_section_byindex_64(hdr, hdr->e_shstrndx);
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

void				print_phdr(void *ptr)
{
	Elf64_Phdr			*phdr;
	Elf64_Ehdr			*hdr;

	hdr = ptr;
	phdr = (void *)hdr + hdr->e_phoff;
	for (int i = 0; i < hdr->e_phnum; i++)
	{
		printf("\n[PROGRAM_HEADER %d]\n", i);
		printf("\tp_type: %u\n", phdr->p_type);
		printf("\tp_flags: %u\n", phdr->p_flags);
		printf("\tp_offset: %lu\n", phdr->p_offset);
		printf("\tp_vaddr: %lu\n", phdr->p_vaddr);
		printf("\tp_paddr: %lu\n", phdr->p_paddr);
		printf("\tp_filesz: %lu\n", phdr->p_filesz);
		printf("\tp_memsz: %lu\n", phdr->p_memsz);
		printf("\tp_align: %lu\n\n", phdr->p_align);
		phdr = (void *)phdr + sizeof(Elf64_Phdr);
	}
}

void				hexdump(void *ptr, size_t len)
{
	size_t	k;
	unsigned char	*c;

	for (size_t i = 0; i < len; i++)
	{
		printf("%p: ", ptr);
		c = ptr;
		k = i;
		for (size_t j = 0; j < 16 && i < len; j++)
		{
			if (j % 2 == 0)
				printf(" ");
			printf("%02x", c[j]);
			i++;
		}
		printf(" ");
		if ((i - k) < 16)
		{
			printf("  ");
			for (size_t l = 0; l < (16 - (i - k)); l++)
				printf("  ");
		}
		for (size_t j = 0; j < 16 && k < len; j++)
		{
			if (ft_isprint(c[j]))
				printf("%c", c[j]);
			else
				printf(".");
			k++;
		}
		printf("\n");
		ptr += 16;
	}
}
