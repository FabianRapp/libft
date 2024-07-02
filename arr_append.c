/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_append.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:42:21 by frapp             #+#    #+#             */
/*   Updated: 2024/05/21 18:42:29 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//return false on error
// *arr has to be dynamicly allocated and was freed if this function returns true
// *new_data does not have to be dynamic memory and is not freeed
// in case of a malloc error this fucntion only frees memory it allocated
bool	arr_append(void **arr, void *new_data, int data_size, int old_arr_len)
{
	void	*new;

	if (!data_size)
		return (false);
	if (!new_data)
		return (false);
	if (!arr)
		return (false);
	if (!*arr && old_arr_len)
		return (false);
	new = malloc((old_arr_len + 1) * data_size);
	if (!new)
		return (false);
	ft_memcpy(new, *arr, old_arr_len * data_size);
	ft_memcpy(((uint8_t *)new) + data_size * old_arr_len, new_data, data_size);
	free(*arr);
	*arr = new;
	return (true);
}

