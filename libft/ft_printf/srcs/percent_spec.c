/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent_spec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 14:28:41 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 18:30:50 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			percent_calcul_display(t_flag *f, t_display *d)
{
	d->len = 1;
	if (f->minus)
		d->padding = LEFT;
	d->padding_char = f->zero ? '0' : ' ';
	if (f->fwidth > d->len)
		d->padding_size = f->fwidth - d->len;
}

static size_t		percent_display(t_display display)
{
	size_t			out;

	out = 0;
	out += print_sign(display);
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += ft_putchar_safe('%');
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t				percent_spec(t_flag *flag, va_list ap)
{
	t_display		display;

	(void)ap;
	init_display(&display);
	percent_calcul_display(flag, &display);
	return (percent_display(display));
}
