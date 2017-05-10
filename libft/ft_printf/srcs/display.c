/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/14 11:25:38 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/25 14:46:26 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			init_display(t_display *display)
{
	display->len = 0;
	display->is_neg = 0;
	display->precision = 0;
	display->padding_size = 0;
	display->padding_char = ' ';
	display->prefix = NO_PREFIX;
	display->nbr = 0;
	display->s = NULL;
	display->padding = RIGHT;
}

static void		undefined_calcul_display(t_flag *f, t_display *d)
{
	d->len = 1;
	if (f->minus)
		d->padding = LEFT;
	d->padding_char = f->zero ? '0' : ' ';
	if (f->fwidth > (d->len + d->precision))
		d->padding_size = f->fwidth - d->len - d->precision;
}

size_t			undefined_display(t_flag *flag, char c)
{
	size_t		out;
	t_display	display;

	init_display(&display);
	undefined_calcul_display(flag, &display);
	out = 0;
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += print_prefix(display);
	out += ft_putchar_safe(c);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}
