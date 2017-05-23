/* *************************************************************************** */
/*                                                                             */
/*               ╦ ╦╔═╗╔═╗╔╦╗╦ ╦  ╦ ╦╔═╗╔═╗╔╦╗╔═╗╔═╗╔═╗╦╔═╔═╗╦═╗               */
/*               ║║║║ ║║ ║ ║║╚╦╝  ║║║║ ║║ ║ ║║╠═╝╠═╣║  ╠╩╗║╣ ╠╦╝               */
/*               ╚╩╝╚═╝╚═╝═╩╝ ╩   ╚╩╝╚═╝╚═╝═╩╝╩  ╩ ╩╚═╝╩ ╩╚═╝╩╚═               */
/*                                                                             */
/*                                                                             */
/*                          mguillau42 and FlorianVenturini                    */
/*             <mguillau@student.42.fr> | <fventuri@student.42.fr>             */
/*                                                                             */
/* *************************************************************************** */

#include <woody.h>

static int	getval(char c)
{
	const char	digits[16] = "0123456789abcdef";
	int			i;

	for (i = 0; i < 16; ++i)
		if (c == digits[i])
			return (i);
	return (-1);
}

int			str_to_hex(const char *str)
{
	int		ret = 0;
	int		val;
	int		i;

	if ((val = getval(str[0])) == -1)
		return (-1);
	ret = val << 4;
	if ((val = getval(str[1])) == -1)
		return (-1);
	return (ret + val);
}

char		*str_tolower(char *str)
{
	int		i = -1;

	while (str[++i])
		if (str[i] < 91 && str[i] > 64)
			str[i] += 32;
}

int		ft_strlen(const char *s)
{
	int		i = -1;

	if (!s)
		return (0);
	while (s[++i]);
	return (i);
}
