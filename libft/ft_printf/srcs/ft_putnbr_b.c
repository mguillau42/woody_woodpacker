/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/14 11:20:32 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:37:44 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr_b(uintmax_t n, char *b, size_t b_len, size_t *out)
{
	if (n >= b_len)
	{
		ft_putnbr_b(n / b_len, b, b_len, out);
		ft_putnbr_b(n % b_len, b, b_len, out);
	}
	else
		*out += ft_putchar_safe(b[n]);
}
