/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynamic_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */ /*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:18:53 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:18:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_dynamic_arr.h"

// slow but fine for now
// takes a pointer to a dyn arr
void	write_dyn_arr(void **dest, void *src, size_t offset, size_t len) {
	while (len--) {
		dyn_arr_add_save(dest, src++, offset++);
	}
}

// dynamic array
/*
 * header:
	len : element count
	size : sizeof element
	destructor : optional paramenter to pass through dyn_arr_init2,
		it will be called on each element of the array on a dyn_arr_free call
	data : flexible array member will be size 0 but still allows 
	acess to the data in the array
*/
// return NULL on malloc error
void	*dyn_arr_init(uint64_t data_size, size_t init_len)
{
	t_dyn_header	*arr;

	arr = (t_dyn_header *)malloc(data_size * init_len + sizeof(t_dyn_header));
	if (!arr)
		return (NULL);
	arr->len = init_len;
	arr->size = data_size;
	arr->destructor = NULL;
	ft_bzero(arr + 1, data_size * init_len);
	return (arr + 1);
}

/*
 * header:
	len : element count
	size : sizeof element
	destructor : optional paramenter to pass through dyn_arr_init2,
		it will be called on each element of the array on a dyn_arr_free call
	data : flexible array member will be size 0 but still allows 
	acess to the data in the array
*/
// return NULL on error
/*
 * argc: argument count: has to be >= 1
 * max arg coun: 3
 * args after argc:
 * 1.: element byte size
 * 2.: init element count
 * 3.: destructor to call on dyn_arr_free for each element
 * DESTRUCOR HAS TO TAKE A POINTER TO THE OBJECT AND IS NOT ALLOWED TO FREE
 * THE OBJCT IT SELF
example of destructor usage:
void	wrap_free(void **p) {
	free(*p);
}
int main() {
	int **arr = dyn_arr_init2(3, sizeof(int*), 0, wrap_free);
	int	*tmp = malloc(sizeof(int));
	*tmp = 5;
	dyn_arr_add_save((void **)(&arr), &tmp, 10);
	dyn_arr_free((void **)&arr);
}
*/
void	*dyn_arr_init2(int argc, ...) {
	t_dyn_header	*arr;
	size_t			data_size;
	size_t			init_len;
	void			(*destructor)(void *);
	va_list			argv;
	size_t			arr_size;

	if (argc < 1 || argc > 3) {
		return (NULL);
	}
	va_start(argv, argc);
	data_size = va_arg(argv, size_t);
	init_len =  argc >= 2 ? va_arg(argv, size_t) : 0;
	destructor = argc >= 3 ? va_arg(argv, void (*)(void *)) : NULL;
	va_end(argv);
	arr_size = data_size * init_len;

	arr = (t_dyn_header *)malloc(arr_size + sizeof(t_dyn_header));
	if (!arr)
		return (NULL);
	bzero(arr, arr_size + sizeof(t_dyn_header));
	arr->len = init_len;
	arr->size = data_size;
	arr->destructor = destructor;
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
	void			*old_arr = *arr;
	t_dyn_header	*old_header= ((t_dyn_header*)(*arr)) -1;
	size_t			old_len = dyn_arr_get_len(old_arr);
	size_t			data_size = dyn_arr_get_size(old_arr);
	size_t			old_arr_capacity = old_len * data_size;
	t_dyn_header	*new_header;
	void			*new_arr;
	size_t			new_arr_capacity;
	size_t			new_len;

	if (old_len)
		new_len = 2 * old_len;
	else
		new_len = 1;
	new_arr_capacity = data_size * new_len;
	new_header = (t_dyn_header *)malloc(new_arr_capacity + sizeof(t_dyn_header));
	if (!new_header)
		return (ft_free(arr), errno);
	memcpy(new_header, old_header, old_arr_capacity + sizeof(t_dyn_header));
	new_header->len = new_len;
	new_arr = new_header + 1;
	bzero((uint8_t *)new_arr + old_arr_capacity, new_arr_capacity - old_arr_capacity);
	free(old_header);
	*arr = new_arr;
	return (0);
}

// alternative to arr[index] = ... : if the arr is too small its automatically
// resized
// returns non 0 on malloc error
int	dyn_arr_add_save(void **arr, void *data, size_t index)
{
	size_t	data_size;

	data_size = dyn_arr_get_size(*arr);
	//todo: make this more efficient
	while (dyn_arr_get_len(*arr) <= index)
	{
		if (dyn_arr_resize(arr))
			return (errno);
	}
	ft_memmove(((uint8_t *)*arr) + data_size * index, data, data_size);
	return (0);
}

void	dyn_arr_free(void **arr)
{
	void			*buf = *arr;

	if (!buf) {
		return ;
	}
	t_dyn_header	*header = ((t_dyn_header *)buf) - 1;
	void			(*destructor)(void *) = header->destructor;

	if (destructor) {
		size_t	len = header->len;
		size_t	size = header->size;
		size_t	capacity = len * size;
		for (size_t i = 0; i < capacity; i += size) {
			destructor((void *)(((uint8_t *)buf) + i));
		}
	}
	free(header);
	*arr = NULL;
}

//-----------------------------TESTS--------------------------------
/*
#include <stdio.h>

bool	_check(uint16_t *arr, uint16_t *cmp, unsigned i, int test_idx) {
	if (arr[i] != cmp[i]) {
		printf("error in test %d(index %u): %d != %d\n", test_idx, i, arr[i], cmp[i]);
		return (true);
	}
	return (false);
}

void	wrap_free(void **p) {
	free(*p);
}

int	main(void)
{
	{
		uint16_t	cmp[123];
		uint16_t	*arr;


		ft_bzero(cmp, sizeof cmp);
		arr = dyn_arr_init2(2, 2, 123);
		if (!arr) {
			printf("error arr == NULL\n");
			return (1);
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 0)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);

		arr = dyn_arr_init(2, 0);
		bzero(cmp, sizeof cmp);
		for (unsigned i = sizeof cmp / sizeof cmp[0] / 2; i < sizeof cmp / sizeof cmp[0]; i++){
			cmp[i] = i;
			dyn_arr_add_save((void **)(&arr), &i, i);
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 1)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);

		arr = dyn_arr_init(2, 0);
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			cmp[i] = i;
			dyn_arr_add_save((void **)&arr, &i, i);
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 2)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);

		arr = dyn_arr_init2(2, 2, 10);
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			cmp[i] = i;
			dyn_arr_add_save((void **)&arr, &i, i);
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 3)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);

		arr = dyn_arr_init2(2, 2, 3);
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			cmp[i] = i;
			dyn_arr_add_save((void **)&arr, &i, i);
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 4)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);

		arr = dyn_arr_init2(2, 2, 123);
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			cmp[i] = i;
			arr[i] = i;
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 5)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);

		arr = dyn_arr_init2(2, 2, 0);
		bzero(cmp, sizeof cmp);
		for (unsigned i = sizeof cmp / sizeof cmp[0] / 2; i < sizeof cmp / sizeof cmp[0]; i++){
			cmp[i] = i;
			dyn_arr_add_save((void **)(&arr), &i, i);
		}
		for (unsigned i = 0; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (_check(arr, cmp, i, 6)) {
				return (1);
			}
		}
		dyn_arr_free((void **)&arr);
	}

	{
		// check destructor functionality
		int		cmp[200];
	
		int **arr = dyn_arr_init2(3, sizeof(int*), 0, wrap_free);
		bzero(cmp, sizeof cmp);
		unsigned start_i =  sizeof cmp / sizeof cmp[0] / 2;
		for (unsigned i = start_i; i < sizeof cmp / sizeof cmp[0]; i++){
			cmp[i] = i;
			int	*tmp = malloc(sizeof(int));
			*tmp = i;
			dyn_arr_add_save((void **)(&arr), &tmp, i);
		}
		for (unsigned i = 0; i < start_i; i++) {
			if (arr[i] != NULL) {
				printf("error in test %d(index %u): should be NULL, is %p\n", 7, i, arr[i]);
				return (0);
			}
		}
		for (unsigned i = start_i; i < sizeof cmp / sizeof cmp[0]; i++) {
			if (arr[i][0] != cmp[i]) {
				printf("error in test %d(index %u): %d != %d\n", 7, i, arr[i][0], cmp[i]);
				return (0);
			}
		}
		dyn_arr_free((void **)&arr);
	}

	if (errno) {
		printf("%s\n", strerror(errno));
	} else {
		printf("all dyn arr tests passed\n");
	}
	return (0);
}
*/

