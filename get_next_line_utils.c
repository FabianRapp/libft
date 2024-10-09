/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:20:02 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:20:03 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"

t_get_next_line	*get_current(t_get_next_line **list, int fd)
{
	t_get_next_line	*cur;
	t_get_next_line	*new;

	cur = *list;
	while (cur && cur->next)
	{
		if (cur->fd == fd)
			return (cur);
		cur = cur->next;
	}
	if (cur && cur->fd == fd)
		return (cur);
	new = ft_calloc(1, sizeof(t_get_next_line));
	if (!new)
		return (NULL);
	new->fd = fd;
	if (cur)
		cur->next = new;
	else
		*list = new;
	return (new);
}

void	cleanup_all_get_next_line(t_get_next_line **list)
{
	t_get_next_line	*cur;
	t_get_next_line	*next;

	if (!*list)
		return ;
	cur = *list;
	*list = NULL;
	while (cur)
	{
		next = cur->next;
		free(cur->tmp_line);
		free(cur);
		cur = next;
	}
}

// always returns NULL
void	*cleanup_get_next_line(t_get_next_line **list, int fd)
{
	t_get_next_line	*cur;

	if (fd == -1 || !*list)
		return (cleanup_all_get_next_line(list), NULL);
	cur = *list;
	if (cur->fd == fd)
	{
		*list = cur->next;
		return (free(cur->tmp_line), free(cur), NULL);
	}
	list = &cur;
	cur = cur->next;
	while (cur)
	{
		if (cur->fd == fd)
		{
			(*list)->next = cur->next;
			return (free(cur->tmp_line), free(cur), NULL);
		}
		list = &cur;
		cur = cur->next;
	}
	return (NULL);
}
