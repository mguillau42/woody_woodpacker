/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 20:57:16 by mguillau          #+#    #+#             */
/*   Updated: 2015/05/24 21:15:01 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_put_color(char *str, char *color)
{
	ft_putstr(color);
	ft_putstr(str);
	ft_putstr(TXTOFF);
}
