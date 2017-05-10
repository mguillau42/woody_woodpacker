/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 15:42:52 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:55:59 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void *str;

	if ((str = (void *)malloc(sizeof(void *) * size)) == NULL)
		return (NULL);
	ft_bzero(str, size);
	return (str);
}
