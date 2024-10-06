#include "libft.h"

bool	swap_float(void *a, void *b)
{
	return (*(float *)a > *(float *)b);
}

static void	swap(size_t data_size, void *arr, size_t i)
{
	size_t	j;
	uint8_t	tmp;

	j = 0;
	while (j < data_size)
	{
		tmp = ((uint8_t *)arr)[i * data_size + j];
		((uint8_t *)arr)[i * data_size + j] = ((uint8_t *)arr)
		[(i + 1) * data_size + j];
		((uint8_t *)arr)[(i + 1) * data_size + j] = tmp;
		j++;
	}
}

//for now simple bouble sort
void	ft_sort(void *arr, size_t data_size, size_t arr_len,
		bool (*do_swap)(void *, void *))
{
	int		i;
	bool	sorted;
	int		end;

	if (arr_len <= 1)
		return ;
	end = ((int) arr_len) - 1;
	sorted = false;
	while (!sorted)
	{
		i = -1;
		sorted = true;
		while (++i < end)
		{
			if (do_swap(((uint8_t *)arr) + i * data_size,
					((uint8_t *)arr) + i * data_size + data_size))
			{
				sorted = false;
				swap(data_size, arr, i);
			}
		}
		end--;
	}
}
