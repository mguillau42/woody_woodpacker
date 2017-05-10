/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 16:35:11 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 00:50:31 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strdup(const char *s1)
{
	size_t	j;
	size_t	i;
	char	*s2;

	if (!s1)
		return (NULL);
	i = ft_strlen(s1);
	if (!(s2 = (char *)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	j = 0;
	while (j < i)
	{
		s2[j] = s1[j];
		j++;
	}
	s2[j] = '\0';
	return (s2);
}
