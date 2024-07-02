/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 21:18:07 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:29:34 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*buffer;
	int		i;
	int		bytes;

	bytes = count * size;
	if (!bytes)
		return (NULL);
	buffer = (char *)malloc(bytes);
	if (!buffer)
	{
		return (NULL);
	}
	i = 0;
	while (i < bytes)
		buffer[i++] = 0;
	return ((void *)buffer);
}
