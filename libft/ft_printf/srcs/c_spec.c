/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_spec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 16:20:14 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:28:20 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			c_calcul_display(t_flag *flag, t_display *display)
{
	display->len = 1;
	if (flag->minus)
		display->padding = LEFT;
	display->padding_char = flag->zero ? '0' : ' ';
	if (flag->fwidth > display->len)
		display->padding_size = flag->fwidth -
			display->len - display->precision;
}

static size_t		c_display(t_display display)
{
	size_t			out;

	out = 0;
	out += print_sign(display);
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += print_prefix(display);
	out += print_precision(display);
	out += ft_putchar_safe(display.nbr);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t				c_spec(t_flag *flag, va_list ap)
{
	t_display		display;

	if (flag->length_modifier == L)
		return (mc_spec(flag, ap));
	init_display(&display);
	display.nbr = va_arg(ap, int);
	display.type = CHAR;
	c_calcul_display(flag, &display);
	return (c_display(display));
}
