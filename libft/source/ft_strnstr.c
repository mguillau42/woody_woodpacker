/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 17:44:06 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:58:07 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t	l;

	if (!*s2)
		return ((char *)s1);
	l = ft_strlen(s2);
	while (*s1 && n >= l)
	{
		if (!ft_memcmp(s1, s2, l))
			return ((char *)s1);
		s1++;
		n--;
	}
	return (NULL);
}
