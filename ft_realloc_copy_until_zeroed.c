/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_copy_until_zeroed.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 02:29:02 by frapp             #+#    #+#             */
/*   Updated: 2024/04/05 22:27:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	is_buffer_all_zeros(void *buffer, size_t size)
{
	size_t	i;
	int8_t	*ptr;

	ptr = (int8_t *)buffer;
	i = 0;
	while (i < size)
	{
		if (ptr[i] != 0)
		{
			return (false);
		}
		i++;
	}
	return (true);
}

/**
 * Reallocates a buffer for an array of fixed-size elements and copies elements
 * from the old buffer to the new one until an element consisting entirely of
 * null bytes is encountered. The new buffer is initialized to zero.
 *
 * @param ptr The old buffer to copy data from.
 * @param new_count The number of elements in the new buffer.
 * @param size The size of each element.
 * @return A pointer to the newly allocated buffer, or NULL on failure or
 * 		   if new_count is 0.
 */
void	*ft_realloc_copy_until_zeroed(void *ptr, size_t new_cnt, size_t size)
{
	int8_t			*new;
	size_t			i;
	int8_t			*old;
	size_t			offset;

	if (!new_cnt)
		return (free(ptr), NULL);
	new = ft_calloc(new_cnt, size);
	if (!new)
		return (free(ptr), NULL);
	old = (int8_t *)ptr;
	if (!old)
		return (new);
	i = 0;
	offset = 0;
	while (!is_buffer_all_zeros(ptr + offset, size) && i < new_cnt)
	{
		ft_memcpy(new + offset, ptr + offset, size);
		i++;
		offset = i * size;
	}
	free(ptr);
	return (new);
}
