
#ifndef FT_DYNAMIC_ARR_H
# define FT_DYNAMIC_ARR_H

#include <stdint.h>
#include <stdlib.h>

typedef struct s_dyn_header
{
	size_t		len;
	uint8_t		size;
	char		data[];
}	t_dyn_header;

#endif
