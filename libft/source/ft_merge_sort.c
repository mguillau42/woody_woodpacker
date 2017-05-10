/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_merge_sort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/08 14:19:33 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/08 14:35:55 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Merge sort function for t_list *
**
** Passs a function pointer int (*f)(t_list *a, t_list *b) which compares the
** the content of the two nodes. if f returns 0 pick a else pick b
*/

t_list			*merge(t_list *a, t_list *b, int (*f)(t_list *a, t_list *b))
{
	t_list		*dummy;
	t_list		*cur;
	t_list		*ret;

	dummy = ft_lstnew(NULL, 0);
	cur = dummy;
	while (a != NULL && b != NULL)
	{
		if (!f(a, b))
		{
			cur->next = a;
			a = a->next;
		}
		else
		{
			cur->next = b;
			b = b->next;
		}
		cur = cur->next;
	}
	cur->next = (a == NULL) ? b : a;
	ret = dummy->next;
	ft_lstdelone(&dummy, NULL);
	return (ret);
}

t_list			*get_middle(t_list *head)
{
	t_list		*slow;
	t_list		*fast;

	slow = head;
	fast = head;
	if (head == NULL)
		return (head);
	while (fast->next != NULL && fast->next->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	return (slow);
}

t_list			*ft_merge_sort(t_list **head, int (*f)(t_list *a, t_list *b))
{
	t_list		*middle;
	t_list		*half;
	t_list		*merged;

	if ((*head) == NULL || (*head)->next == NULL || !f)
		return (*head);
	middle = get_middle(*head);
	half = middle->next;
	middle->next = NULL;
	merged = merge(ft_merge_sort(head, f), ft_merge_sort(&half, f), f);
	return (merged);
}
