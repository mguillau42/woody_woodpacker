/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 16:38:33 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 00:49:57 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	j = ft_strlen(dest);
	while (src[i])
		dest[j++] = src[i++];
	dest[j] = '\0';
	return (dest);
}
