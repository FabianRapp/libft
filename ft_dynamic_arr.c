#include "libft.h"
#include "ft_dynamic_arr.h"

// dynamic array withou macro functions to pass code guidelines
/*
	len : element count
	size : sizeof element
	data : flexible array member will be size 0 but still allows 
	acess to the data in the array
*/
// return NULL on malloc error
void	*dyn_arr_init(uint8_t data_size, size_t init_len)
{
	t_dyn_header	*arr;

	arr = (t_dyn_header *)malloc(data_size * init_len + sizeof(t_dyn_header));
	if (!arr)
		return (NULL);
	arr->len = init_len;
	arr->size = data_size;
	ft_bzero(arr + 1, data_size * init_len);
	return (arr + 1);
}

size_t	dyn_arr_get_len(void *arr)
{
	return (((t_dyn_header *)arr)[-1].len);
}

size_t	dyn_arr_get_size(void *arr)
{
	return (((t_dyn_header *)arr)[-1].size);
}

// double the array size
// basically realloc with calloc
// return 0 on sucess, errno for malloc error
int	dyn_arr_resize(void **arr)
{
	size_t			len;
	size_t			new_len;
	size_t			data_size;
	t_dyn_header	*new;

	len = dyn_arr_get_len(*arr);
	data_size = dyn_arr_get_size(*arr);
	if (len)
		new_len = 2 * len;
	else
		new_len = 1;
	new = (t_dyn_header *)malloc(data_size * new_len + sizeof(t_dyn_header));
	if (!new)
		return (ft_free(arr), errno);
	new->size = ((t_dyn_header *)*arr)[-1].size;
	new->len = new_len;
	new++;
	ft_memcpy(new, *arr, len * data_size);
	ft_bzero((uint8_t *)new + len * data_size, (new_len - len) * data_size);
	free(((t_dyn_header *)(*arr)) - 1);
	*arr = new;
	return (0);
}

// alternative to arr[index] = ... : if the arr is too small its automatically
// resized
// returns non 0 on malloc error
int	dyn_arr_add_save(void **arr, void *data, size_t index)
{
	uint8_t	data_size;

	data_size = dyn_arr_get_size(*arr);
	if (dyn_arr_get_len(*arr) <= index)
	{
		if (dyn_arr_resize(arr))
			return (errno);
	}
	ft_memmove(((uint8_t *)*arr) + data_size * index, data, data_size);
	return (0);
}

//-----------------------------TESTS--------------------------------
/*
#include <stdio.h>

int	main(void)
{
	int16_t		cmp[123];
	uint16_t	*arr;

	ft_bzero(cmp, sizeof cmp);
	arr = dyn_arr_init(2, 123);
	if (!arr)
	{
		printf("error arr == NULL\n");
		return (1);
	}
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		printf("comparing index: %u: ", i);
		if (arr[i] != cmp[i])
		{
			printf("error in test 0: %d != %d\n", arr[i], cmp[i]);
			return (1);
		}
		printf("passed\n");
	}
	dyn_arr_free((void **)&arr);

	arr = dyn_arr_init(2, 0);
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		cmp[i] = i;
		dyn_arr_add_save((void **)&arr, &i, i);
		printf("%u added\n", i);
	}
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		printf("comparing index: %u: ", i);
		if (arr[i] != cmp[i])
		{
			printf("error in test 1: %d != %d\n", arr[i], cmp[i]);
			return (1);
		}
		printf("passed\n");
	}
	dyn_arr_free((void **)&arr);

	arr = dyn_arr_init(2, 10);
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		cmp[i] = i;
		dyn_arr_add_save((void **)&arr, &i, i);
	}
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		printf("comparing index: %u: ", i);
		if (arr[i] != cmp[i])
		{
			printf("error in test 2: %d != %d\n", arr[i], cmp[i]);
			return (1);
		}
		printf("passed\n");
	}
	dyn_arr_free((void **)&arr);

	arr = dyn_arr_init(2, 3);
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		cmp[i] = i;
		dyn_arr_add_save((void **)&arr, &i, i);
	}
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		printf("comparing index: %u: ", i);
		if (arr[i] != cmp[i])
		{
			printf("error in test 3: %d != %d\n", arr[i], cmp[i]);
			return (1);
		}
		printf("passed\n");
	}
	dyn_arr_free((void **)&arr);

	arr = dyn_arr_init(2, 123);
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		cmp[i] = i;
		arr[i] = i;
	}
	for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++)
	{
		printf("comparing index: %u: ", i);
		if (arr[i] != cmp[i])
		{
			printf("error in test 3: %d != %d\n", arr[i], cmp[i]);
			return (1);
		}
		printf("passed\n");
	}
	dyn_arr_free((void **)&arr);
	return (0);
}
*/
