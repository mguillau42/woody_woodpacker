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

#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/syscall.h>
# include <sys/types.h>
# include <unistd.h>

/*
** GET.C
*/
Elf64_Shdr			*get_section_bytype_64(Elf64_Ehdr *hdr, Elf64_Word type);
Elf64_Shdr			*get_section_entry_64(Elf64_Ehdr *hdr, Elf64_Addr entry);
Elf64_Phdr			*get_last_segment_64(Elf64_Ehdr *hdr);
Elf64_Shdr			*get_shdr_table_64(Elf64_Ehdr *hdr, size_t original_size);

/*
** HANDLE_ELF64.C
*/
int					handle_elf64(void *m, size_t original_size, void *key_param);

/*
** INJECT.C
*/
void				*inject_code(void *injected_section, Elf64_Shdr *entry_shdr, Elf64_Ehdr *original_hdr, Elf64_Addr new_entry_point, void *key, void *shellcode);

/*
** LIBFT.C
*/
void				ft_bzero(void *s, size_t n);
int					ft_isprint(int c);
void				*ft_memalloc(size_t size);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
int					ft_strequ(char const *s1, char const *s2);

/*
** ENCRYPT.S
*/
void				*encrypt(void *data, size_t len, void *key);

#endif
