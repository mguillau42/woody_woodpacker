/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 22:03:15 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 01:20:29 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t		print_padding(t_display display)
{
	size_t	out;

	out = ft_putchar_n(display.padding_char, display.padding_size);
	return (out);
}

size_t		print_prefix(t_display display)
{
	if (display.type == SIGNED && display.prefix == SPACE && !display.is_neg)
		return (ft_putchar_safe(' '));
	else if (display.prefix == ALT)
	{
		if (display.type == OCTAL && display.nbr != 0)
			return (ft_putchar_safe('0'));
		else if (display.type == HEX && display.nbr != 0)
			return (ft_putstr_ret("0x"));
		else if (display.type == HEXM && display.nbr != 0)
			return (ft_putstr_ret("0X"));
		else if (display.type == PTR)
			return (ft_putstr_ret("0x"));
	}
	return (0);
}

size_t		print_precision(t_display display)
{
	size_t out;

	out = ft_putchar_n('0', display.precision);
	return (out);
}

size_t		print_sign(t_display display)
{
	if (display.type == SIGNED)
	{
		if (display.is_neg)
			return (ft_putchar_safe('-'));
		else if (display.prefix == PLUS)
			return (ft_putchar_safe('+'));
	}
	return (0);
}
