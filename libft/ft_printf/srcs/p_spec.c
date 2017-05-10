/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_spec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 14:59:55 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/25 14:46:07 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			p_calcul_display(t_flag *f, t_display *d)
{
	if (f->minus)
		d->padding = LEFT;
	d->len = ft_nbrlen_b(d->nbr, MBASE_16);
	d->is_neg = 0;
	d->padding_char = f->zero ? '0' : ' ';
	if (f->precision != -1 && (size_t)f->precision > d->len)
		d->precision = f->precision - d->len;
	if (f->precision == 0 && d->nbr == 0)
		d->len = 0;
	if (f->fwidth > (d->len + 2))
		d->padding_size = f->fwidth - (d->len + 2);
	d->prefix = ALT;
}

static size_t		p_display(t_flag *flag, t_display display)
{
	size_t			out;

	out = 0;
	if (flag->fwidth && flag->zero)
		out += print_prefix(display);
	if (display.padding == RIGHT)
		out += print_padding(display);
	if (!flag->zero)
		out += print_prefix(display);
	out += print_precision(display);
	if (display.len)
		ft_putnbr_b(display.nbr, BASE_16, BASE_16_SIZE, &out);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t				p_spec(t_flag *flag, va_list ap)
{
	t_display		display;

	init_display(&display);
	display.type = PTR;
	display.nbr = (uintmax_t)va_arg(ap, void*);
	p_calcul_display(flag, &display);
	return (p_display(flag, display));
}
