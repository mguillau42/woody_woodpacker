/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_parse_all.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 16:14:38 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:17:28 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

const char	*format_parse_flags(t_flag *flag, const char **format)
{
	if (**format == ' ' || **format == '+' ||
		**format == '-' || **format == '0' || **format == '#')
	{
		if (!(ft_strncmp(*format, "-", 1)))
			flag->minus = 1;
		else if (!(ft_strncmp(*format, " ", 1)))
			flag->space = 1;
		else if (!(ft_strncmp(*format, "+", 1)))
			flag->plus = 1;
		else if (!(ft_strncmp(*format, "0", 1)))
			flag->zero = 1;
		else if (!(ft_strncmp(*format, "#", 1)))
			flag->alt = 1;
		(*format)++;
		return (format_parse_flags(flag, format));
	}
	else
		return (*format);
}

const char	*format_parse_width(t_flag *flag, const char **format)
{
	if (ft_isdigit(**format))
	{
		flag->fwidth = ft_atoi(*format);
		(*format) += ft_nbrlen(flag->fwidth);
	}
	return (*format);
}

const char	*format_parse_precision(t_flag *flag, const char **format)
{
	if (!(ft_strncmp(*format, ".", 1)))
	{
		if (ft_isdigit(*(*format + 1)))
		{
			flag->precision = ft_atoi(++(*format));
			if (flag->precision >= 0)
				(*format) += ft_nbrlen(flag->precision);
		}
		else
		{
			(*format)++;
			flag->precision = 0;
		}
	}
	return (*format);
}

const char	*format_parse_length(t_flag *flag, const char **format)
{
	if (!(ft_strncmp(*format, "hh", 2)))
	{
		flag->length_modifier = HH;
		return (*format += 2);
	}
	else if (!(ft_strncmp(*format, "ll", 2)))
	{
		flag->length_modifier = LL;
		return (*format += 2);
	}
	else if (**format == 'h' || **format == 'l' ||
		**format == 'j' || **format == 'z')
	{
		if (!(ft_strncmp(*format, "h", 1)))
			flag->length_modifier = H;
		else if (!(ft_strncmp(*format, "l", 1)))
			flag->length_modifier = L;
		else if (!(ft_strncmp(*format, "j", 1)))
			flag->length_modifier = J;
		else if (!(ft_strncmp(*format, "z", 1)))
			flag->length_modifier = Z;
		return (*format += 1);
	}
	return (*format);
}
