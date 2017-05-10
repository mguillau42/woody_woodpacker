/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 17:52:46 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 00:50:53 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_striter(char *s, void (*f)(char *))
{
	size_t	l;
	size_t	i;

	if (s != NULL && f != NULL)
	{
		l = ft_strlen(s);
		i = 0;
		while (i < l)
		{
			(*f)(s);
			s++;
			i++;
		}
	}
}
