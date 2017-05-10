/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 13:55:35 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 15:03:12 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	char	*dst_b;
	char	*src_b;
	size_t	i;

	dst_b = (char *)dst;
	src_b = (char *)src;
	i = 0;
	while (i < n)
	{
		dst_b[i] = src_b[i];
		if (src_b[i] == c)
			return (&dst_b[i + 1]);
		i++;
	}
	return (NULL);
}
