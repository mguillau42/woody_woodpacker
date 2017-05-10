/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_msg_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 21:10:24 by mguillau          #+#    #+#             */
/*   Updated: 2015/05/26 04:58:40 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_msg_display(char *str, char *line, int n)
{
	if (n)
		ft_put_color("[SUCCESS]", TXTGRN);
	else
		ft_put_color("[ERROR]", TXTRED);
	ft_putstr(": ");
	ft_putstr(str);
	if (line)
	{
		ft_putstr(": ");
		ft_putendl(line);
	}
	else
		ft_putchar('\n');
}
