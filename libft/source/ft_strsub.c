/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 11:11:03 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:58:28 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	len_str;
	char	*str;

	if (!s)
		return (NULL);
	len_str = ft_strlen(s);
	if (start > len_str || len > len_str)
		return (NULL);
	if (start + len > len_str)
		return (NULL);
	if (!(str = ft_strnew(len)))
		return (NULL);
	str = ft_strncpy(str, s + start, len);
	str[len] = '\0';
	return (str);
}
