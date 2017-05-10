/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/14 16:49:05 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 13:53:46 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_digits(int n)
{
	size_t	i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char			*ft_itoa(int n)
{
	size_t	ndg;
	size_t	ncp;
	char	*str;

	ncp = n;
	ndg = ft_digits(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		ncp = -n;
		ndg++;
	}
	if (!(str = ft_strnew(ndg)))
		return (NULL);
	str[--ndg] = ncp % 10 + '0';
	while (ncp /= 10)
		str[--ndg] = ncp % 10 + '0';
	if (n < 0)
		str[0] = '-';
	return (str);
}
