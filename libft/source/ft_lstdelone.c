/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/17 10:46:49 by mguillau          #+#    #+#             */
/*   Updated: 2015/05/23 19:17:12 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*tmp;

	if (alst && *alst)
	{
		tmp = (*alst)->next;
		if (del)
			del((*alst)->content, (*alst)->content_size);
		(*alst)->content = NULL;
		free(*alst);
		*alst = tmp;
	}
}
