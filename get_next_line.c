#include "get_next_line.h"
#include "libft.h"

//allocates the minium size and copies the data over to 1 string
char	*get_final_line(t_get_next_line *data)
{
	size_t	size;
	char	*line;

	size = data->read_position - data->first_char_position
		+ data->tmp_line_size;
	line = malloc(sizeof(char) * (size + 1));
	if (line)
	{
		ft_memcpy(line, data->tmp_line, data->tmp_line_size);
		ft_memcpy(line + data->tmp_line_size, data->buffer
			+ data->first_char_position, data->read_position
			- data->first_char_position);
		line[size] = 0;
	}
	data->first_char_position = data->read_position;
	free(data->tmp_line);
	data->tmp_line = NULL;
	data->tmp_line_size = 0;
	return (line);
}

bool	join_buffer_to_tmp_line(t_get_next_line *data)
{
	char	*tmp;

	if (data->tmp_line_size + data->read_return - data->first_char_position
		+ 1 > data->cur_allocation_size)
	{
		tmp = malloc(data->next_allocation_size * sizeof(char));
		if (!tmp)
			return (false);
		ft_memcpy(tmp, data->tmp_line, data->tmp_line_size);
		free(data->tmp_line);
		data->tmp_line = tmp;
		data->cur_allocation_size = data->next_allocation_size;
		data->next_allocation_size *= 2;
	}
	ft_memcpy(data->tmp_line + data->tmp_line_size, data->buffer
		+ data->first_char_position, data->read_return
		- data->first_char_position);
	data->tmp_line_size += BUFFER_SIZE - data->first_char_position;
	data->tmp_line[data->tmp_line_size] = 0;
	return (true);
}

char	*get_line(t_get_next_line *data)
{
	data->cur_allocation_size = 0;
	data->next_allocation_size = BUFFER_SIZE + 1;
	free(data->tmp_line);
	data->tmp_line = NULL;
	while (1)
	{
		while (data->read_position < data->read_return)
		{
			if (data->buffer[data->read_position++] == '\n')
				return (get_final_line(data));
		}
		if (data->read_return && data->read_return < BUFFER_SIZE)
			return (get_final_line(data));
		if (data->read_return)
			join_buffer_to_tmp_line(data);
		data->read_return = read(data->fd, data->buffer, BUFFER_SIZE);
		data->first_char_position = 0;
		data->read_position = 0;
		if (!data->read_return && !data->tmp_line_size)
			return (NULL);
		if (!data->read_return)
			return (get_final_line(data));
		if (data->read_return < 0)
			return (NULL);
	}
}

// made so it segfaults on currupted state
void	remove_node(t_get_next_line **list, t_get_next_line *node)
{
	free(node->tmp_line);
	if (*list == node)
	{
		*list = node->next;
		free(node);
		return ;
	}
	while ((*list)->next != node)
		list = &(*list)->next;
	(*list)->next = node->next;
	free(node);
}

// if cleanup is set the interanl data for the given fd is cleaned (not the fd)
// if cleanup is set and the fd is -1 everything is cleaned
// return NULL if cleanup is set or an error occurs
char	*get_next_line(int fd, bool do_cleanup)
{
	static t_get_next_line	*list = NULL;
	t_get_next_line			*cur;
	char					*line;

	if (do_cleanup)
	{
		cleanup_get_next_line(&list, fd);
		return (NULL);
	}
	if (fd < 0)
		return (NULL);
	cur = get_current(&list, fd);
	if (!cur)
		return (NULL);
	line = get_line(cur);
	if (!line || cur->read_position == cur->read_return)
		remove_node(&list, cur);
	return (line);
}
