/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_del_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:19:09 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:19:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static bool	if_first(t_list **list, t_list *node, void (*del)(void *))
{
	if (*list == node)
	{
		*list = (*list)->next;
		del(node->content);
		free(node);
		return (true);
	}
	return (false);
}

void	ft_lst_del_node(t_list **list, t_list *node, void (*del)(void *))
{
	t_list	*cur;
	t_list	*last;

	if (!*list)
		return ;
	if (if_first(list, node, del))
		return ;
	last = *list;
	cur = (*list)->next;
	while (cur)
	{
		if (cur == node)
		{
			last->next = cur->next;
			del(cur->content);
			free(cur);
			return ;
		}
		last = cur;
		cur = cur->next;
	}
}
