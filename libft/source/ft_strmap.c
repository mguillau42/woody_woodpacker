/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/12 13:10:59 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:57:53 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmap(char const *s, char (*f)(char))
{
	size_t	i;
	char	*ptr;
	char	*str;

	if (!f || !s)
		return (NULL);
	ptr = (char *)s;
	i = 0;
	if ((str = (char *)malloc(sizeof(char) * ft_strlen(s) + 1)) == NULL)
		return (NULL);
	while (ptr[i] != '\0')
	{
		str[i] = f(ptr[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
