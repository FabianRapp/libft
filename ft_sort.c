#include "libft.h"



bool	swap_float(void *a, void *b)
{
	return (*(float *)a > *(float *)b);
}

//for now simple bouble sort
void	ft_sort(void *arr, size_t data_size, size_t arr_len,
		bool (*swap)(void *, void *))
{
	size_t	i;
	bool	sorted;
	uint8_t	tmp;
	size_t	j;
	size_t	end;

	if (arr_len <= 1)
		return ;
	end = arr_len - 1;
	sorted = false;
	while (!sorted)
	{
		i = 0;
		sorted = true;
		while (i < end)
		{
			if (swap(((uint8_t *)arr) + i * data_size,
				((uint8_t *)arr) + i * data_size + data_size))
			{
				sorted = false;
				j = 0;
				while (j < data_size)
				{
					tmp = ((uint8_t *)arr)[ i * data_size + j];
					((uint8_t *)arr)[ i * data_size + j] = ((uint8_t *)arr)[ (i + 1) * data_size + j];
					((uint8_t *)arr)[ (i + 1) * data_size + j] = tmp;
					j++;
				}
			}
			i++;
		}
		end--;
	}
}
