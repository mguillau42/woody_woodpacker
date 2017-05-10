/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putwchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 12:17:02 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:41:07 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t				ft_putwchar_range_four(wchar_t n)
{
	int						out;
	unsigned char			x;
	unsigned char			y;
	unsigned char			z;
	unsigned char			w;

	out = 0;
	w = (((n & 0x1C0000) >> 18) + 0xF0);
	z = (((n & 0x3F000) >> 12) + 0x80);
	y = (((n & 0x0FC0) >> 6) + 0x80);
	x = ((n & 0x003F) + 0x80);
	out += write(1, &w, 1);
	out += write(1, &z, 1);
	out += write(1, &y, 1);
	out += write(1, &x, 1);
	return (out);
}

static size_t				ft_putwchar_range_three(wchar_t n)
{
	int						out;
	unsigned char			x;
	unsigned char			y;
	unsigned char			z;

	out = 0;
	z = (((n & 0xF000) >> 12) + 0xE0);
	y = (((n & 0x0FC0) >> 6) + 0x80);
	x = ((n & 0x003F) + 0x80);
	out += write(1, &z, 1);
	out += write(1, &y, 1);
	out += write(1, &x, 1);
	return (out);
}

static size_t				ft_putwchar_range_two(wchar_t n)
{
	int						out;
	unsigned char			x;
	unsigned char			y;

	out = 0;
	y = (((n & 0x07C0) >> 6) + 0xC0);
	x = ((n & 0x003F) + 0x80);
	out += write(1, &y, 1);
	out += write(1, &x, 1);
	return (out);
}

size_t						ft_putwchar(wchar_t n)
{
	int						out;

	out = 0;
	if (n < 0 || n > 1114111)
		return (-1);
	if (n >= 55296 && n <= 57343)
		return (-1);
	if (n <= 127)
		return (write(1, &n, 1));
	if (n <= 2047)
		out += ft_putwchar_range_two(n);
	else if (n <= 65535)
		out += ft_putwchar_range_three(n);
	else if (n <= 1114111)
		out += ft_putwchar_range_four(n);
	return (out);
}
