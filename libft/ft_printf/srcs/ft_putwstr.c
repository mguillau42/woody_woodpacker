/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putwstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/19 17:30:32 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 01:21:18 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t		ft_putwstr(wchar_t *str)
{
	size_t	out;

	out = 0;
	if (str == NULL)
		return (ft_putstr_ret("(null)"));
	while (*str)
	{
		out += ft_putwchar(*str);
		str++;
	}
	return (out);
}
