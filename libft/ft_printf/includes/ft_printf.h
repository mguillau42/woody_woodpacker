/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 18:45:28 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 01:16:56 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <sys/types.h>
# include <limits.h>
# include <wchar.h>

# define BASE_8 		"01234567"
# define BASE_8_SIZE	8

# define BASE_10 		"0123456789"
# define BASE_10_SIZE	10

# define BASE_16		"0123456789abcdef"
# define MBASE_16		"0123456789ABCDEF"
# define BASE_16_SIZE	16

typedef	struct s_flag		t_flag;
typedef	struct s_display	t_display;
typedef size_t	(*t_spec)(t_flag *, va_list);

typedef enum e_lmod			t_lmod;
typedef enum e_type			t_type;
typedef enum e_side			t_side;
typedef enum e_prefix		t_prefix;

enum			e_lmod
{
	NONE,
	H,
	HH,
	L,
	LL,
	J,
	Z
};

enum			e_type
{
	SIGNED,
	UNSIGNED,
	CHAR,
	STRING,
	OCTAL,
	HEX,
	HEXM,
	PTR,
	WCHAR,
	WSTRING
};

enum	e_prefix
{
	NO_PREFIX,
	SPACE,
	PLUS,
	ALT
};

enum			e_side
{
	LEFT,
	RIGHT
};

struct			s_flag
{
	int		alt;
	int		zero;
	int		minus;
	int		space;
	int		plus;
	ssize_t	precision;
	size_t	fwidth;
	int		length_modifier;
};

struct			s_display
{
	t_type		type;
	uintmax_t	nbr;
	char		*s;
	size_t		len;
	int			is_neg;
	size_t		precision;
	t_side		padding;
	size_t		padding_size;
	char		padding_char;
	t_prefix	prefix;
	wchar_t		wchar;
	wchar_t		*wstr;
};

int				ft_atoi(const char *str);
int				ft_isdigit(int c);
unsigned int	ft_nbrlen(int nbr);
void			ft_bzero(void *s, size_t n);
void			*ft_memset(void *b, int c, size_t len);
void			*ft_memalloc(size_t size);
char			*ft_strchr(const char *s, int c);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_putnstr(char const *s, size_t size);
size_t			ft_putstr_ret(char const *s);
size_t			ft_putchar_safe(char c);
size_t			ft_putchar_n(char c, size_t n);
size_t			ft_strlen(const char *s);
size_t			ft_putnwstr(wchar_t const *wstr, size_t size);
size_t			ft_wstrlen(wchar_t *wstr, ssize_t precision);
int				ft_printf(const char *format, ...);
int				format_parse(va_list ap, const char *format);
const char		*format_parse_flags(t_flag *flag, const char **format);
const char		*format_parse_width(t_flag *flag, const char **format);
const char		*format_parse_precision(t_flag *flag, const char **format);
const char		*format_parse_length(t_flag *flag, const char **format);
int				format_parse_all(t_flag *flag, va_list ap, const char **format);
void			format_init_flag(t_flag *flag);
void			exit_failure(void);
t_spec			get_spec_function(char c);
void			free_spec_array(void);
size_t			c_spec(t_flag *flag, va_list ap);
size_t			d_spec(t_flag *flag, va_list ap);
size_t			md_spec(t_flag *flag, va_list ap);
size_t			o_spec(t_flag *flag, va_list ap);
size_t			mo_spec(t_flag *flag, va_list ap);
size_t			u_spec(t_flag *flag, va_list ap);
size_t			mu_spec(t_flag *flag, va_list ap);
size_t			x_spec(t_flag *flag, va_list ap);
size_t			p_spec(t_flag *flag, va_list ap);
size_t			mx_spec(t_flag *flag, va_list ap);
size_t			s_spec(t_flag *flag, va_list ap);
size_t			mc_spec(t_flag *flag, va_list ap);
size_t			ms_spec(t_flag *flag, va_list ap);
size_t			percent_spec(t_flag *flag, va_list ap);
void			ft_putnbr_b(uintmax_t n, char *b, size_t b_len, size_t *out);
size_t			ft_nbrlen_b(uintmax_t n, char *b);
void			init_display(t_display *display);
size_t			print_prefix(t_display display);
size_t			print_precision(t_display display);
size_t			print_sign(t_display display);
size_t			print_padding(t_display display);
size_t			ft_putwchar(wchar_t n);
size_t			ft_putwstr(wchar_t *str);
size_t			undefined_display(t_flag *flag, char c);

#endif
