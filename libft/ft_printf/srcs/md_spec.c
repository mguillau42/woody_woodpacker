/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md_spec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 14:28:41 by mguillau          #+#    #+#             */
/*   Updated: 2016/05/25 14:46:27 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			md_calcul_display(t_flag *f, t_display *d)
{
	d->len = ft_nbrlen_b(d->nbr, "0123456789");
	if (f->precision != -1 || f->minus)
		f->zero = 0;
	if (f->minus)
		d->padding = LEFT;
	if (f->space)
		d->prefix = SPACE;
	if (f->plus)
		d->prefix = PLUS;
	if (f->precision == 0 && d->nbr == 0)
		d->len = 0;
	if (f->precision != -1 && (size_t)f->precision > d->len)
		d->precision = f->precision - d->len;
	d->padding_char = f->zero ? '0' : ' ';
	if (f->fwidth > (d->len + d->precision))
	{
		if ((f->plus && !d->is_neg) || d->is_neg || f->space)
			f->fwidth -= 1;
		if (f->zero)
			d->precision += f->fwidth - d->len - d->precision;
		else
			d->padding_size = f->fwidth - d->len - d->precision;
	}
}

static size_t		md_display(t_display display)
{
	size_t			out;

	out = 0;
	if (display.padding == RIGHT)
		out += print_padding(display);
	out += print_sign(display);
	out += print_prefix(display);
	out += print_precision(display);
	if (display.len)
		ft_putnbr_b(display.nbr, BASE_10, BASE_10_SIZE, &out);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t				md_spec(t_flag *flag, va_list ap)
{
	intmax_t		n;
	t_display		display;

	init_display(&display);
	if (flag->minus)
		flag->zero = 0;
	n = va_arg(ap, long);
	if (n == LLONG_MIN)
		return (write(1, "-9223372036854775808", 20));
	if (n < 0)
	{
		n = -n;
		display.is_neg = 1;
	}
	display.nbr = n;
	display.type = SIGNED;
	md_calcul_display(flag, &display);
	return (md_display(display));
}
