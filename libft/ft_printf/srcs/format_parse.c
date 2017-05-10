/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 22:03:15 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 01:20:43 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		format_init_flag(t_flag *flag)
{
	flag->alt = 0;
	flag->zero = 0;
	flag->minus = 0;
	flag->space = 0;
	flag->plus = 0;
	flag->fwidth = 0;
	flag->precision = -1;
	flag->length_modifier = NONE;
}

int			format_parse_all(t_flag *flag, va_list ap, const char **format)
{
	size_t	ret;
	t_spec	spec_fct;

	ret = 0;
	if (*(++*format) == 0)
		return (0);
	if (format_parse_flags(flag, format) == NULL)
		exit_failure();
	if (format_parse_width(flag, format) == NULL)
		exit_failure();
	if (format_parse_precision(flag, format) == NULL)
		exit_failure();
	if (format_parse_length(flag, format) == NULL)
		exit_failure();
	if ((spec_fct = get_spec_function(**format)) == NULL)
		ret = undefined_display(flag, **format);
	else
		ret = spec_fct(flag, ap);
	(*format)++;
	return (ret);
}

int			format_parse(va_list ap, const char *format)
{
	t_flag	flag;
	char	*next;
	int		out;

	out = 0;
	while (format != NULL)
	{
		format_init_flag(&flag);
		next = ft_strchr(format, '%');
		if (*format == 0)
			return (out);
		else if (next == NULL)
			return (out + ft_putstr_ret(format));
		else if (next > format)
		{
			out += ft_putnstr(format, next - format);
			format += next - format;
		}
		else
			out += format_parse_all(&flag, ap, &format);
	}
	return (out);
}
