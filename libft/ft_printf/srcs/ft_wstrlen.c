/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wstrlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 13:04:40 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/25 14:52:11 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t		nb_byte(wchar_t n)
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

size_t				ft_wstrlen(wchar_t *wstr, ssize_t precision)
{
	size_t			i;
	size_t			out;

	i = 0;
	out = 0;
	if (!wstr)
		return (out);
	if (precision == -1)
	{
		while (wstr[i])
			out += nb_byte(wstr[i++]);
	}
	else
	{
		while (wstr[i] && (size_t)precision >= (nb_byte(wstr[i]) + out))
		{
			out += nb_byte(wstr[i]);
			i++;
		}
	}
	return (out);
}
