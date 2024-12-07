#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "libft_macros.h"

#ifndef FT_MAP_INIT_LEN
# define FT_MAP_INIT_LEN 127
#endif // FT_MAP_INIT_SIZE


/* assumes a 64 bit PML4 system */
typedef struct s_map_node {
	void			*key;
	union {
		void			*value;
	struct {
		uintptr_t	ptr_mask : 48;
		uint16_t	meta_data : 16;
		};
	};
}	__attribute__((aligned(16))) t_map_node;

#define MAP_NODE_GET_VAL_PTR(node) \
	((void *)(((((uintptr_t)1) << 47) - 1) & node.ptr_mask))

typedef struct {
	uint32_t	key_size;
	uint32_t	buf_len;
	int			(*cmp_keys_fn)(const void *, const void *);
	t_map_node	*buf;
	/* Total size = buf_len * (sizeof(t_map_node))*/
	uint64_t	(*hash_fn)(const void *val);
	void		(*free_value_fn)(void *);
	void		(*free_key_fn)(void *);
	size_t		element_count;
	size_t		collision_count;
} __attribute__((aligned(CACHE_LINE_SIZE)))	t_map;

struct map_args {
	uint32_t	key_size;
	int			(*cmp_keys)(const void *key1, const void *key2);
	uint64_t	(*hash)(const void *key);
	void		(*free_value)(void *value);
	void		(*free_key)(void *key);
};
