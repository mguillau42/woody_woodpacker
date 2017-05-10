/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 20:24:37 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/14 12:08:45 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ft_printf(const char *format, ...)
{
	int		chars;
	va_list	ap;

	va_start(ap, format);
	chars = format_parse(ap, format);
	va_end(ap);
	return (chars);
}
