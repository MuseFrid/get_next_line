/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduchesn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 15:02:48 by gduchesn          #+#    #+#             */
/*   Updated: 2023/03/06 17:56:54 by gduchesn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*set_stash(char *buf, char (*stash)[BUFFER_SIZE + 1], int *ret)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == '\n' && ++i)
	{
		*ret = (*ret) - 1;
		new = ft_substr(buf, 0, i);
		while (buf[i])
			(*stash)[j++] = buf[i++];
		(*stash)[j] = 0;
	}
	else
		new = ft_substr(buf, 0, i);
	free(buf);
	return (new);
}

char	*read_failed_ended(int ret, char *buf, char *str)
{
	free(buf);
	if (ret == -1)
	{
		if (str)
			free(str);
		return (NULL);
	}
	return (str);
}

char	*get_next(int fd, char (*stash)[BUFFER_SIZE + 1], char *str, int *ret)
{
	char		*buf;
	char		*new;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
	{
		if (str)
			free(str);
		return (NULL);
	}
	(*ret) = read(fd, buf, BUFFER_SIZE);
	if (*ret <= 0)
		read_failed_ended(*ret, buf, str);
	buf[(*ret)] = 0;
	new = ft_strjoin(str, buf);
	free(buf);
	if (str)
		free(str);
	if (!new)
		return (NULL);
	buf = set_stash(new, stash, ret);
	return (buf);
}

char	*is_empty(char (*stash)[BUFFER_SIZE + 1])
{
	char	*str;
	int		len;
	int		i;

	i = -1;
	len = ft_strlen(*stash);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (++i < len)
		str[i] = (*stash)[i];
	str[i] = 0;
	i = 0;
	while ((*stash)[i])
		(*stash)[i++] = 0;
	str = set_stash(str, stash, &i);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	stash[OPEN_MAX][BUFFER_SIZE + 1];
	char		*str;
	int			ret;

	str = NULL;
	ret = BUFFER_SIZE;
	if (BUFFER_SIZE <= 0 || fd < 0 || fd > OPEN_MAX)
		return (NULL);
	if (stash[fd][0] != 0)
	{
		str = is_empty(&(stash[fd]));
		if (!str)
			return (NULL);
	}
	if (stash[fd][0] == 0)
	{
		while (ret == BUFFER_SIZE)
		{
			str = get_next(fd, &(stash[fd]), str, &ret);
			if (!str)
				return (NULL);
		}
	}
	return (str);
}
