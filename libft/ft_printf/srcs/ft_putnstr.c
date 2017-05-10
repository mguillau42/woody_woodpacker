/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/12 00:36:05 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:37:48 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t		ft_putnstr(char const *s, size_t size)
{
	size_t	out;
	size_t	i;

	i = 0;
	out = 0;
	while (s[i] && i < size)
	{
		out += ft_putchar_safe(s[i]);
		i++;
	}
	return (out);
}
