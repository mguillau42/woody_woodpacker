/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 11:31:33 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:58:09 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	char	to_find;
	int		i;

	to_find = c;
	str = NULL;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == to_find)
			str = (char *)(s + i);
		i++;
	}
	if (s[i] == to_find)
		str = (char *)(s + i);
	return (str);
}
