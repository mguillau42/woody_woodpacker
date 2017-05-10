/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mc_spec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 22:28:21 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:46:18 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		mc_calcul_display(t_flag *flag, t_display *display)
{
	if (flag->minus)
		display->padding = LEFT;
	if (flag->zero)
		display->padding_char = '0';
}

static size_t	mc_display(t_display display)
{
	size_t out;

	out = 0;
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += ft_putwchar(display.wchar);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t			mc_spec(t_flag *flag, va_list ap)
{
	t_display	display;

	init_display(&display);
	display.wchar = (wchar_t)va_arg(ap, wint_t);
	display.type = WCHAR;
	mc_calcul_display(flag, &display);
	return (mc_display(display));
}
