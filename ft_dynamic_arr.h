/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynamic_arr.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:18:49 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:18:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DYNAMIC_ARR_H
# define FT_DYNAMIC_ARR_H

# include <stdint.h>
# include <stdlib.h>

typedef struct s_dyn_header
{
	size_t		len;
	size_t		size;
	char		data[];
}	t_dyn_header;

#endif
