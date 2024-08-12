#include "libft.h"

void	dyn_arr_free(void **arr)
{
	free(((t_dyn_header *)(*arr)) - 1);
	*arr = NULL;
}
