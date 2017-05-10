/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 11:28:01 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 12:10:44 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_atoi(const char *str)
{
	int	sign;
	int	val;
	int	i;

	i = 0;
	sign = 0;
	val = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		val = val * 10 + (str[i] - '0');
		i++;
	}
	if (sign)
		val = -val;
	return (val);
}
