/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iziane <iziane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 03:05:49 by iziane            #+#    #+#             */
/*   Updated: 2024/09/05 16:31:17 by iziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_2d_index(char **str, int index)
{
	int	i;

	i = 0;
	while (str && str[i] && i < index)
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
}

// void	ft_free_2d(char **str)
// {
// 	int	i;

// 	i = 0;
// 	if (str == NULL)
// 		return ;
// 	while (str[i] != NULL)
// 	{
// 		free(str[i]);
// 		str[i] = NULL;
// 		i++;
// 	}
// 	free(str);
// }
