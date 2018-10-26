/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/25 17:44:46 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/26 20:32:31 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "get_next_line.h"

static char		*ft_first_return(const char *str)
{
	char	*next_nl;
	char	*new_str;

	next_nl = ft_strchr(str, '\n');
	new_str = ft_strnew(next_nl - str + 1);
	ft_strncpy(new_str, str, next_nl - str);
	new_str[next_nl - str] = '\0';
	return (new_str);
}

static t_list	*ft_listlink(t_list *list, const int fd)
{
	if (list)
	{
		while (list)
		{
			if (list->content_size == (size_t)fd)
				return (list);
			list = list->next;
		}
	}
	return (NULL);
}

static t_list	*ft_set_tmp_value(t_list *list, int fd)
{
	t_list *tmp;

	if (ft_listlink(list, fd))
	{
		tmp = ft_listlink(list, fd);
		if (!tmp->content)
			tmp->content = ft_strnew(0);
	}
	else
	{
		ft_lstaddend(&list, ft_lstnew("\0", fd));
		tmp = ft_listlink(list, fd);
	}
	return (tmp);
}

static void		ft_set_line_tmp_value(char **line, t_list **tmp)
{
	if (ft_strchr((*tmp)->content, '\n'))
	{
		*line = ft_first_return((*tmp)->content);
		(*tmp)->content = ft_strchr((*tmp)->content, '\n') + 1;
	}
	else
	{
		*line = ft_strdup((*tmp)->content);
		(*tmp)->content = NULL;
	}
}

int				get_next_line(const int fd, char **line)
{
	int				nbchar;
	char			buf[BUFF_SIZE + 1];
	static t_list	*list = NULL;
	t_list			*tmp;
	char			*tmptofree;

	if (read(fd, buf, 0) == -1)
		return (-1);
	if (!list)
		list = (t_list*)ft_memalloc(sizeof(t_list));
	tmp = ft_set_tmp_value(list, fd);
	while (!ft_strchr(tmp->content, '\n') && (nbchar = read(tmp->content_size, buf, BUFF_SIZE)) > 0)
	{
		buf[nbchar] = '\0';
		//tmptofree = tmp->content;
		//tmp->content = ft_strjoin(tmptofree, buf);
		//free(tmptofree);
		tmptofree = ft_strjoin(tmp->content, buf);
		tmp->content = tmptofree;
		free(tmptofree);
	}
	if (nbchar < 0)
		return (-1);
	else if (((char*)tmp->content)[0] != '\0')
	{
		ft_set_line_tmp_value(line, &tmp);
		return (1);
	}
	else
		return (0);
}