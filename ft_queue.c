

#include "libft.h"

// returns NULL on malloc error
t_queue	*init_que(size_t capacity)
{
	t_queue	*queue;

	queue = malloc(sizeof(t_queue));
	if (!queue)
		return (NULL);
	queue->buffer = malloc(sizeof(void *) * capacity);
	if (!queue->buffer)
		return (free(queue), NULL);
	queue->capacity = capacity;
	queue->len = 0;
	queue->head_index = 0;
	queue->tail_index = capacity - 1;
	return (queue);
}

// returns 0 on sucess
// returns -1 if the data could not be added duo to a full queue
int8_t	enque(t_queue *queue, void *data)
{
	if (queue->len == queue->capacity)
		return (-1);
	queue->len++;
	queue->tail_index++;
	queue->tail_index %= queue->capacity;
	queue->buffer[queue->tail_index] = data;
	return (0);
}

void	*deque(t_queue *queue)
{
	void	*data;

	if (!queue->len)
		return (NULL);
	printf("head index: %lu\n", queue->head_index);
	data = queue->buffer[queue->head_index++];
	queue->head_index %= queue->capacity;
	queue->len--;
	return (data);
}

void	free_que(t_queue **queue)
{
	free((*queue)->buffer);
	free(*queue);
	*queue = NULL;
}

/*
//tests
int	main(void)
{
	int	que_size = 1000;
	int	arr[que_size];
	t_queue		*que = init_que(que_size);
	for (int i = 0; i < que_size; i ++)
	{
		arr[i] = i;
		enque(que, arr + i);
	}
	for (int i = 0; i < que_size; i ++)
	{
		arr[i] = i;
		int *val = deque(que);
		if (val)
			printf("%d\n", *val);
		else
			printf("null\n");
		val = deque(que);
		if (val)
			printf("%d\n", *val);
		else
			printf("null\n");
		enque(que, arr + i);
	}
	int *val = deque(que);
	if (val)
		printf("%d\n", *val);
	else
		printf("null\n");
	free_que(&que);
	return (0);
}
*/
