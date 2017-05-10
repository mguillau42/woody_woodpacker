/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mo_spec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 16:48:32 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/25 14:33:15 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		mo_calcul_display(t_flag *f, t_display *d)
{
	d->len = ft_nbrlen_b(d->nbr, "01234567");
	if (f->precision != -1 || f->minus)
		f->zero = 0;
	if (f->minus)
		d->padding = LEFT;
	if (f->space)
		d->prefix = SPACE;
	if (f->alt)
		d->prefix = ALT;
	if (f->precision != -1 && (size_t)f->precision > d->len)
		d->precision = f->precision - d->len;
	if (d->prefix == ALT && d->nbr != 0 && d->precision > 0)
		d->precision -= 1;
	if (f->precision == 0 && d->nbr == 0 && !f->alt)
		d->len = 0;
	d->padding_char = f->zero ? '0' : ' ';
	if (f->fwidth > (d->len + d->precision))
		d->padding_size = f->fwidth - d->len - d->precision;
}

static size_t	mo_display(t_display display)
{
	size_t		out;

	out = 0;
	out += print_sign(display);
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += print_prefix(display);
	out += print_precision(display);
	if (display.len)
		ft_putnbr_b(display.nbr, "01234567", 8, &out);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t			mo_spec(t_flag *flag, va_list ap)
{
	t_display	display;

	init_display(&display);
	display.type = OCTAL;
	display.nbr = va_arg(ap, unsigned long);
	mo_calcul_display(flag, &display);
	return (mo_display(display));
}
