/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 17:56:16 by mguillau          #+#    #+#             */
/*   Updated: 2015/05/15 10:16:42 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*ft_addword(size_t i, char const *s, char c)
{
	size_t		size;
	size_t		j;
	char		*word;

	size = i;
	j = 0;
	while (s[size] && s[size] != c)
		size++;
	if (!(word = ft_strnew(size - i)))
		return (NULL);
	while (i < size)
		word[j++] = s[i++];
	return (word);
}

static size_t	ft_countwords(char const *s, char c)
{
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i])
			j++;
		while (s[i] != c && s[i])
			i++;
	}
	return (j);
}

char			**ft_strsplit(char const *s, char c)
{
	char		**tab;
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	tab = NULL;
	if (!s)
		return (NULL);
	if (!(tab = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1))))
		return (NULL);
	while (j < ft_countwords(s, c))
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			tab[j++] = ft_addword(i, s, c);
		while (s[i] != c && s[i])
			i++;
	}
	tab[j] = 0;
	return (tab);
}
