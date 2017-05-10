/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/12 12:26:14 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 00:51:49 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	l;
	size_t	i;

	if (s != NULL && f != NULL)
	{
		l = ft_strlen(s);
		i = 0;
		while (i < l)
		{
			(*f)(i, s);
			s++;
			i++;
		}
	}
}
