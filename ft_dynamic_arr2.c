#include "libft.h"
#include "ft_dynamic_arr.h"

void	dyn_arr_free(void **arr)
{
	if (*arr)
		free(((t_dyn_header *)(*arr)) - 1);
	*arr = NULL;
}
