/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 22:19:44 by frapp             #+#    #+#             */
/*   Updated: 2024/10/09 22:19:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	swap_float(void *a, void *b)
{
	return (*(float *)a > *(float *)b);
}

bool	swap_ints(void *a, void *b) {
	return (*((int*)b) < *((int*)a));
}

static inline void	swap(size_t data_size, void *arr, size_t i, size_t j)
{
	uint8_t	tmp[16];
	uint8_t	*a = ((uint8_t *)arr) + data_size * i;
	uint8_t	*b = ((uint8_t *)arr) + data_size * j;

	while (data_size >= 16) {
		memcpy(tmp, a, 16);
		memcpy(a, b, 16);
		memcpy(b, tmp, 16);
		a += 16;
		b += 16;
		data_size -= 16;
	}
	if (data_size) {
		memcpy(tmp, a, data_size);
		memcpy(a, b, data_size);
		memcpy(b, tmp, data_size);
	}
}

static	void	q_sort(void *arr, size_t data_size, size_t begin, size_t end,
		bool (*do_swap)(void *, void *))
{
	if (begin >= end) {
		return ;
	}
	size_t	pivot = end;
	size_t	j = begin;
	size_t	i = j;

	while (j < pivot) {
		if (do_swap(((uint8_t *)arr) + (pivot * data_size),
			((uint8_t *)arr) + (j * data_size)))
		{
			swap(data_size, arr, i++, j);
		}
		j++;
	}
	swap(data_size, arr, i, pivot);//swap pivot to it's place
	if (i > 1) {
		q_sort(arr, data_size, begin, i - 1, do_swap);
	}
	q_sort(arr, data_size, i + 1, end, do_swap);
}

void	ft_sort(void *arr, size_t data_size, size_t arr_len,
		bool (*do_swap)(void *, void *))
{
	if (arr_len <= 1) {
		return ;
	}
	q_sort(arr, data_size, 0, arr_len - 1, do_swap);
}

/*
#include <time.h>

int	main(void) {
	srand(time(0));
	for (size_t j = 0; j < 100; j++) {
		int	n = rand() % 10000;
		printf("n: %d\n", n);
		int	*a = malloc(sizeof(int) * n);
		int	*b = malloc(sizeof(int) * n);
		for (int i = 0; i < n; i++) {
			a[i] = rand();
		}
		memcpy(b, a, sizeof(int) * n);
		printf("sorting q sort..\n");
		ft_sort(a, sizeof(int), n, swap_ints);
		printf("sorting b sort..\n");
		ft_sort2(b, sizeof(int), n, swap_ints);
		printf("sorting done\n");
		for (int i = 0; i < n; i++) {
			if (a[i] != b[i] || (i < n - 1 && a[i] > a[i + 1])) {
				printf("error\n");
				free(a);
				free(b);
				exit(1);
			}
		}
		free(a);
		free(b);
		printf("pass iter %lu\n", j);
	}
}
*/
