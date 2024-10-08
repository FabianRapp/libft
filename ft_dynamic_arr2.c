/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynamic_arr2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:18:56 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:18:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_dynamic_arr.h"

void	dyn_arr_free(void **arr)
{
	if (*arr)
		free(((t_dyn_header *)(*arr)) - 1);
	*arr = NULL;
}
