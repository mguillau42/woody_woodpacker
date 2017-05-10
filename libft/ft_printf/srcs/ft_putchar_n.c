/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_n.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/13 14:13:23 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 18:21:54 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t		ft_putchar_n(char c, size_t n)
{
	size_t	i;
	size_t	out;

	i = 0;
	out = 0;
	while (i < n)
	{
		out += ft_putchar_safe(c);
		i++;
	}
	return (out);
}
