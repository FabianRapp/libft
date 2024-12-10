#include "libft.h"

struct timeval	add_timeval(struct timeval a, struct timeval b) {
	a.tv_sec += b.tv_sec;
	a.tv_usec += b.tv_usec;
	while (a.tv_usec >= 1000000) {
		a.tv_sec++;
		a.tv_usec -= 1000000;
	}
	return (a);
}

struct timeval	sub_timeval(struct timeval a, struct timeval b) {
	a.tv_sec -= b.tv_sec;
	a.tv_usec -= b.tv_usec;
	while (a.tv_usec < 0) {
		a.tv_sec--;
		a.tv_usec += 1000000;
	}
	return (a);
}
