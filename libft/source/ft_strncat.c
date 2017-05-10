/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 18:13:53 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 00:54:09 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strncat(char *s1, const char *s2, int n)
{
	size_t	j;
	int		i;

	i = 0;
	j = ft_strlen(s1);
	while (i < n && s2[i] != '\0')
	{
		s1[i + j] = s2[i];
		i++;
	}
	s1[i + j] = '\0';
	return (s1);
}
