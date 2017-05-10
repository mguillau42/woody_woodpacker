/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_spec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 16:44:15 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:45:36 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			set_spec_array(t_spec *p)
{
	p['s'] = &s_spec;
	p['p'] = &p_spec;
	p['x'] = &x_spec;
	p['X'] = &mx_spec;
	p['c'] = &c_spec;
	p['d'] = &d_spec;
	p['o'] = &o_spec;
	p['O'] = &mo_spec;
	p['D'] = &md_spec;
	p['i'] = &d_spec;
	p['u'] = &u_spec;
	p['U'] = &mu_spec;
	p['%'] = &percent_spec;
	p['C'] = &mc_spec;
	p['S'] = &ms_spec;
}

static t_spec		*get_spec_array(void)
{
	static t_spec	*ptr_array = NULL;

	if (ptr_array == NULL)
	{
		ptr_array = ft_memalloc(sizeof(t_spec *) * 256);
		if (ptr_array != NULL)
			set_spec_array(ptr_array);
	}
	return (ptr_array);
}

t_spec				get_spec_function(char c)
{
	t_spec			*ptr_array;

	ptr_array = get_spec_array();
	return (ptr_array[(int)c]);
}

void				free_spec_array(void)
{
	free(get_spec_array());
}
