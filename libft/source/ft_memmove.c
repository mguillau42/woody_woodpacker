/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 10:53:15 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 14:58:35 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*p_src;
	char	*p_dst;

	p_src = (char *)src;
	p_dst = (char *)dst;
	if (dst <= src)
		return (ft_memcpy(dst, src, len));
	p_dst += len;
	p_src += len;
	while (len)
	{
		*--p_dst = *--p_src;
		len--;
	}
	return (dst);
}
