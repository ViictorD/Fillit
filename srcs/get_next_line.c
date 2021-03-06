/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/20 00:22:04 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/05 20:04:33 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fillit.h"

int			check_end(char **line, t_nextline *list)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	while (list->buffer[i] && list->buffer[i] != '\n')
		i++;
	if (!(tmp = ft_strsub(list->buffer, 0, i)))
		return (-1);
	tmp2 = *line;
	*line = ft_strjoin(*line, tmp);
	free(tmp);
	free(tmp2);
	return (1);
}

int			lst_remove(t_nextline *elem)
{
	elem->fd = -1;
	return (0);
}

int			treat_extrastr(char **line, t_nextline *list)
{
	char	*str;
	int		i;
	int		bytes;

	i = ft_strlen(list->buffer);
	if (!(str = ft_strchr(list->buffer, '\n')))
		return (0);
	str++;
	if (!(bytes = ft_strlen(str)))
		return (3);
	if (!(str = ft_strsub(str, 0, ft_strlen(str))))
		return (-1);
	ft_bzero(list->buffer, BUFF_SIZE + 1);
	ft_memcpy(list->buffer, str, ft_strlen(str));
	free(str);
	if (ft_strchr(list->buffer, '\n'))
		return (check_end(line, list));
	str = *line;
	*line = ft_strjoin(*line, list->buffer);
	free(str);
	ft_bzero(list->buffer, BUFF_SIZE);
	return (2);
}

t_nextline	*set_list(char **line, t_nextline **list, const int fd)
{
	t_nextline	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (tmp->fd == fd)
		{
			tmp->nbr = treat_extrastr(line, tmp);
			return (tmp);
		}
		tmp = tmp->next;
	}
	tmp = (t_nextline*)malloc(sizeof(t_nextline));
	tmp->fd = fd;
	tmp->nbr = 42;
	tmp->next = *list;
	*list = tmp;
	return (tmp);
}

int			get_next_line(const int fd, char **line)
{
	static t_nextline	*list = NULL;
	t_nextline			*list_tmp;
	char				*tmp;
	int					bytes;

	if (!line || !(*line = (char*)malloc(1)) || fd < 0 || read(fd, NULL, 0) < 0)
		return (-1);
	ft_bzero(*line, ft_strlen(*line));
	if ((list_tmp = set_list(line, &list, fd)) && !list_tmp->nbr)
		return (lst_remove(list_tmp));
	if (list_tmp->nbr >= -1 && list_tmp->nbr <= 1)
		return (list_tmp->nbr);
	while (1)
	{
		bytes = read(fd, list_tmp->buffer, BUFF_SIZE);
		list_tmp->buffer[bytes] = '\0';
		if (!bytes && !ft_strlen(*line))
			return (lst_remove(list_tmp));
		if (bytes < BUFF_SIZE || ft_strchr(list_tmp->buffer, '\n'))
			return (check_end(line, list_tmp));
		tmp = *line;
		*line = ft_strjoin(*line, list_tmp->buffer);
		free(tmp);
	}
	return (-1);
}
