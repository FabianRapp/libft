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

bool	cmp_float(const void *a, const void *b)
{
	return (*(const float *)a > *(const float *)b);
}

bool	cmp_ints(const void *a, const void *b) {
	return (*((const int*)b) < *((const int*)a));
}

int	cmp_ints2(const void *a, const void *b) {
	return (*((int*)b) < *((int*)a));
}

static inline void	swap(void *location1, void *location2, size_t data_size)
{
	uint8_t	tmp[16];
	uint8_t	*a = location1;
	uint8_t	*b = location2;

	if (location1 == location2) {
		return ;
	}

	while (data_size >= 16) {
		memmove(tmp, a, 16);
		memmove(a, b, 16);
		memmove(b, tmp, 16);
		a += 16;
		b += 16;
		data_size -= 16;
	}
	if (data_size) {
		memmove(tmp, a, data_size);
		memmove(a, b, data_size);
		memmove(b, tmp, data_size);
	}
}

static	void	q_sort(size_t data_size, uint8_t *begin, uint8_t *end,
		bool (*cmp)(const void *, const void *))
{
	if (begin >= end) {
		return ;
	}
	uint8_t	*pivot = end;
	uint8_t	*j = begin;
	uint8_t	*i = j;

	while (j < pivot) {
		if (cmp(pivot, j))
		{
			swap(i, j, data_size);
			i += data_size;
		}
		j += data_size;
	}
	swap(i, pivot, data_size);//swap pivot to it's place
	pivot = i;
	if (pivot > ((uint8_t *)NULL) + data_size) {//underflow prevention
		q_sort(data_size, begin, pivot - data_size, cmp);
	}
	q_sort(data_size, pivot + data_size, end, cmp);
}

void	ft_sort(void *arr, size_t data_size, size_t arr_len,
		bool (*cmp)(const void *, const void *))
{
	if (arr_len <= 1) {
		return ;
	}
	q_sort(data_size, arr, ((uint8_t *)arr) + (arr_len - 1) * data_size, cmp);
}
/*
#include <time.h>
#include <sys/time.h>



int	main(void) {
	struct timeval	start;
	struct timeval	end;
	struct timeval	my_time = {0};
	struct timeval	std_time = {0};

	const size_t	max_n = 100000;
	const unsigned	sleep_time = 100;
	int	n;
	int	*a = malloc(sizeof(int) * max_n);
	int	*b = malloc(sizeof(int) * max_n);
	srand(time(0));
	for (size_t j = 0; j < 10000; j++) {
		{
			//n = rand() % max_n;
			n = max_n;

			int nb = rand();
			for (int i = 0; i < n; i++) {
				nb *= 123121;
				a[i] = nb;
			}
			memcpy(b, a, sizeof(int) * n);
			usleep(sleep_time);

			{
				gettimeofday(&start, NULL);
				ft_sort(a, sizeof(int), n, cmp_ints);
				gettimeofday(&end, NULL);
				my_time = add_timeval(my_time, sub_timeval(end, start));
			}
			usleep(sleep_time);
			{
				gettimeofday(&start, NULL);
				qsort(b, n, sizeof(int), cmp_ints2);
				gettimeofday(&end, NULL);
				std_time = add_timeval(std_time, sub_timeval(end, start));
			}
			usleep(sleep_time);
			for (int i = 0; i < n; i++) {
				if (a[i] != b[i] || (i < n - 1 && a[i] > a[i + 1])) {
					printf("error\n");
					free(a);
					free(b);
					exit(1);
				}
			}
		}
		{
			//int	n = rand() % max_n;
			n = max_n;
			int nb = rand();
			for (int i = 0; i < n; i++) {
				nb *= 123121;
				a[i] = nb;
			}
			memcpy(b, a, sizeof(int) * n);
			usleep(sleep_time);
			{
				gettimeofday(&start, NULL);
				qsort(b, n, sizeof(int), cmp_ints2);
				gettimeofday(&end, NULL);
				std_time = add_timeval(std_time, sub_timeval(end, start));
			}
			usleep(sleep_time);
			{
				gettimeofday(&start, NULL);
				ft_sort(a, sizeof(int), n, cmp_ints);
				gettimeofday(&end, NULL);
				my_time = add_timeval(my_time, sub_timeval(end, start));
			}
			//for (int i = 0; i < n; i++) {
			//	if (a[i] != b[i] || (i < n - 1 && a[i] > a[i + 1])) {
			//		printf("error\n");
			//		free(a);
			//		free(b);
			//		exit(1);
			//	}
			//}

		}
		printf("pass iter %lu\n", j);
	}
	free(a);
	free(b);
	printf("my time: %ld seconds and %ld micro seconds\n", my_time.tv_sec, my_time.tv_usec);
	printf("std time: %ld seconds and %ld micro seconds\n", std_time.tv_sec, std_time.tv_usec);
}
*/
