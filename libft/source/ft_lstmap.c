/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/03 11:52:54 by mguillau          #+#    #+#             */
/*   Updated: 2015/01/03 14:40:54 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, t_list *(*f) (t_list *elem))
{
	t_list	*newlst;
	t_list	*begin;

	if (f == NULL || lst == NULL)
		return (NULL);
	newlst = ft_lstnew(f(lst)->content, f(lst)->content_size);
	if (newlst == NULL)
		return (NULL);
	begin = newlst;
	while (lst->next != NULL)
	{
		newlst->next = ft_lstnew(f(lst->next)->content,
				f(lst->next)->content_size);
		lst = lst->next;
		newlst = newlst->next;
	}
	return (begin);
}
