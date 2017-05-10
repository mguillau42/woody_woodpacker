/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 16:18:45 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:58:05 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	void	*str;

	str = (char *)ft_memalloc(size + 1);
	if (str == NULL)
		return (NULL);
	else
		return (str);
}
