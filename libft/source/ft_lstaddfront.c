/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddfront.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/11 16:32:29 by mguillau          #+#    #+#             */
/*   Updated: 2016/08/11 17:17:33 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstaddfront(t_list **alst, t_list *new)
{
	if (alst && new)
	{
		while (*alst)
			alst = &(*alst)->next;
		ft_lstadd(alst, new);
	}
}
