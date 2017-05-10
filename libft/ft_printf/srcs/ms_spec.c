/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_spec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 13:04:40 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 01:21:29 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		ms_calcul_display(t_flag *f, t_display *d)
{
	if (f->minus)
		d->padding = LEFT;
	if (f->zero)
		d->padding_char = '0';
	if (f->precision == -1)
		d->type = STRING;
	d->len = ft_wstrlen(d->wstr, f->precision);
	if (f->fwidth > d->len)
		d->padding_size = f->fwidth - d->len;
}

static size_t	ms_display(t_display display)
{
	size_t		out;

	out = 0;
	if (display.padding == RIGHT)
		out += print_padding(display);
	if (!display.wstr && display.type == STRING)
		out += ft_putstr_ret("(null)");
	else
		out += ft_putnwstr(display.wstr, display.len);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t			ms_spec(t_flag *flag, va_list ap)
{
	t_display	display;

	init_display(&display);
	display.wstr = va_arg(ap, wchar_t *);
	display.type = WSTRING;
	ms_calcul_display(flag, &display);
	return (ms_display(display));
}
