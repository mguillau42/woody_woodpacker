/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 13:30:50 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 14:59:25 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*dst_b;
	char	*src_b;

	dst_b = (char *)dst;
	src_b = (char *)src;
	i = 0;
	while (i < n)
	{
		dst_b[i] = src_b[i];
		i++;
	}
	return (dst);
}
