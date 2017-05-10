/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_spec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 18:27:42 by mguillau          #+#    #+#             */
/*   Updated: 2017/04/04 01:21:38 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		s_calcul_display(t_flag *f, t_display *d)
{
	if (f->minus)
		d->padding = LEFT;
	if (f->zero)
		d->padding_char = '0';
	if (f->precision != -1 && (size_t)f->precision <= ft_strlen(d->s))
		d->len = f->precision;
	else
		d->len = ft_strlen(d->s);
	if (f->fwidth > d->len)
		d->padding_size = f->fwidth - d->len;
}

static size_t	s_display(t_flag *flag, t_display display)
{
	size_t		out;

	out = 0;
	if (display.padding == RIGHT)
		out += print_padding(display);
	if (display.s == NULL && flag->precision == -1)
		out += ft_putstr_ret("(null)");
	else if (display.s != NULL)
		out += ft_putnstr(display.s, display.len);
	if (display.padding == LEFT)
		out += print_padding(display);
	return (out);
}

size_t			s_spec(t_flag *flag, va_list ap)
{
	t_display	display;

	if (flag->length_modifier == L)
		return (ms_spec(flag, ap));
	init_display(&display);
	display.s = va_arg(ap, char*);
	display.type = STRING;
	s_calcul_display(flag, &display);
	return (s_display(flag, display));
}
