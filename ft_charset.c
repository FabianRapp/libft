#include "libft.h"

int	ft_is_charset(int c, char *set) {
	int	i;

	if (!set) {
		return (0);
	}
	i = 0;
	while (set[i]) {
		if (set[i++] == c) {
			return (1);
		}
	}
	return (0);
}

size_t	ft_strlen_charet(char *str, char *set) {
	size_t	len;

	if (!str) {
		return (0);
	}
	len = 0;
	while (str[len]) {
		if (!ft_is_charset(str[len], set)) {
			return (len);
		}
		len++;
	}
	return (len);
}
