/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/03 12:30:41 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:57:16 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	int i;

	i = 0;
	if (dst != NULL && src != NULL)
	{
		while (src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
		return (dst);
	}
	return (NULL);
}
