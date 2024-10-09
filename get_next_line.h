/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:20:07 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:20:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

typedef struct s_get_next_line	t_get_next_line;
typedef struct s_get_next_line
{
	char			buffer[BUFFER_SIZE];
	size_t			first_char_position;
	int				read_position;
	char			*tmp_line;
	size_t			tmp_line_size;
	size_t			cur_allocation_size;
	size_t			next_allocation_size;
	int				read_return;
	int				fd;
	t_get_next_line	*next;
}	t_get_next_line;

char			*get_next_line(int fd, bool do_cleanup);
void			*cleanup_get_next_line(t_get_next_line **list, int fd);
void			cleanup_all_get_next_line(t_get_next_line **list);
t_get_next_line	*get_current(t_get_next_line **list, int fd);

#endif
