/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mu_spec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 14:28:41 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/24 16:57:40 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		mu_calcul_display(t_flag *f, t_display *d)
{
	d->len = ft_nbrlen_b(d->nbr, "0123456789");
	if (f->precision != -1 || f->minus)
		f->zero = 0;
	if (f->minus)
		d->padding = LEFT;
	if (f->precision != -1 && (size_t)f->precision > d->len)
		d->precision = f->precision - d->len;
	if (f->precision == 0 && d->nbr == 0)
		d->len = 0;
	d->padding_char = f->zero ? '0' : ' ';
	if (f->fwidth > (d->len + d->precision))
		d->padding_size = f->fwidth - d->len - d->precision;
}

static size_t	mu_display(t_display display)
{
	size_t		out;

	out = 0;
	out += print_sign(display);
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += print_prefix(display);
	out += print_precision(display);
	if (display.len)
		ft_putnbr_b(display.nbr, BASE_10, BASE_10_SIZE, &out);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t			mu_spec(t_flag *flag, va_list ap)
{
	t_display	display;

	init_display(&display);
	if (flag->minus)
		flag->zero = 0;
	display.nbr = va_arg(ap, unsigned long);
	display.type = UNSIGNED;
	mu_calcul_display(flag, &display);
	return (mu_display(display));
}
