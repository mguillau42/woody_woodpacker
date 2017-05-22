/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 19:34:11 by mguillau          #+#    #+#             */
/*   Updated: 2017/05/11 13:26:34 by fventuri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
#include <sys/syscall.h>

# include <libft.h>


void				print_hdr(void *ptr);
void				print_shdr(void *ptr);
void				print_phdr(void *ptr);
void				print_all(void *ptr);
void				hexdump(void *ptr, size_t len);

Elf64_Shdr			*get_section_byindex_64(Elf64_Ehdr *hdr, Elf64_Half index);
Elf64_Shdr			*get_section_bytype_64(Elf64_Ehdr *hdr, Elf64_Word type);
Elf64_Shdr			*get_section_entry_64(Elf64_Ehdr *hdr, Elf64_Addr entry);
Elf64_Phdr			*get_last_segment_64(Elf64_Ehdr *hdr);
Elf64_Shdr			*get_shdr_table_64(Elf64_Ehdr *hdr, size_t original_size);

void				handle_elf64(void *m, size_t original_size);

void				*encrypt(void *data, size_t len, void *key);
void				inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point);

#endif
