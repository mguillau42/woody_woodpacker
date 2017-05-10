/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/17 14:58:32 by mguillau          #+#    #+#             */
/*   Updated: 2015/05/23 18:53:15 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*new_elem;

	if (!(new_elem = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	if (!content || !content_size)
	{
		new_elem->content = NULL;
		new_elem->content_size = 0;
	}
	else
	{
		if (!(new_elem->content = (void *)malloc(content_size + 1)))
			return (NULL);
		if (content_size >= 4294967295)
			return (NULL);
		if (!(ft_memcpy(new_elem->content, content, content_size + 1)))
			return (NULL);
		new_elem->content_size = content_size;
	}
	new_elem->next = NULL;
	return (new_elem);
}
