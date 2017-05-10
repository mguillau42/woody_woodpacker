/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 14:38:01 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 00:54:36 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *s)
{
	char	*s1;
	size_t	len;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	while ((*s == ' ' || *s == '\n' || *s == '\t') && *s)
		s++;
	len = ft_strlen(s);
	while (s[i] != '\0')
		i++;
	i--;
	while ((s[i] == ' ' || s[i] == '\n' || s[i] == '\t') && i >= 0 && *s)
	{
		i--;
		len--;
	}
	if (!(s1 = ft_strnew(len + 1)))
		return (NULL);
	s1 = ft_strncpy(s1, s, (len));
	s1[len + 1] = '\0';
	return (s1);
}
