/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnwstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 16:38:03 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/25 14:29:09 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	nb_byte(wchar_t n)
{
	if (n < 0 || n > 1114111)
		return (0);
	if (n >= 55296 && n <= 57343)
		return (0);
	if (n <= 127)
		return (1);
	if (n <= 2047)
		return (2);
	else if (n <= 65535)
		return (3);
	else if (n <= 1114111)
		return (4);
	return (0);
}

size_t			ft_putnwstr(wchar_t const *wstr, size_t size)
{
	size_t		out;
	size_t		i;

	i = 0;
	out = 0;
	while (size && wstr[i])
	{
		out += ft_putwchar(wstr[i]);
		size -= nb_byte(wstr[i]);
		i++;
	}
	return (out);
}
