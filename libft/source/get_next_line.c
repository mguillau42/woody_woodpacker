/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguillau <mguillau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 14:18:44 by mguillau          #+#    #+#             */
/*   Updated: 2015/05/26 05:01:19 by mguillau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*str_next(char *dest, const char *src, int len)
{
	char			*cpy;

	cpy = ft_strnew(ft_strlen(dest) + len);
	cpy = ft_strcpy(cpy, dest);
	cpy = ft_strncat(cpy, src, len);
	free(dest);
	return (cpy);
}

static int		get_line(char **line, char **buff, char *str, int len)
{
	char			*cpy;

	*line = str_next(*line, *buff, len);
	cpy = ft_strnew(BUFF_SIZE);
	cpy = ft_strcpy(cpy, str + 1);
	free(*buff);
	*buff = cpy;
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	int				ret;
	char			*str;
	static char		*buff = NULL;

	if (line == NULL)
		return (-1);
	*line = ft_strnew(0);
	if (buff)
	{
		if ((str = ft_strchr(buff, '\n')))
			return (get_line(line, &buff, str, str - buff));
		*line = str_next(*line, buff, ft_strlen(buff));
	}
	else
		buff = ft_strnew(BUFF_SIZE);
	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if ((str = ft_strchr(buff, '\n')) || ret == 0)
			return (get_line(line, &buff, str, str - buff));
		*line = str_next(*line, buff, ret);
	}
	if (ret == 0)
		ft_strdel(&buff);
	return (ret);
}
