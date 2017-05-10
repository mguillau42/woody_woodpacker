/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrlen_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/14 11:21:36 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:33:07 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t			ft_nbrlen_b(uintmax_t n, char *b)
{
	size_t		len;
	int			b_len;

	len = 1;
	b_len = ft_strlen(b);
	if (!n)
		return (1);
	while (n /= b_len)
		len++;
	return (len);
}
