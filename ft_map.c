#include "libft.h"

#include <math.h>

#ifndef FT_MAP_INIT_LEN
# define FT_MAP_INIT_LEN 127
#endif // FT_MAP_INIT_SIZE

// only for the this file, small numbers are wrong
bool	_is_prime(size_t num) {
	if ((num % 2) == 0 || (num % 3) == 0) {
		return (false);
	}
	size_t limit = (size_t) sqrt(num);
	for (size_t i = 5; i <= limit; i += 6) {
		if ((num % i) == 0 || (num % (i + 2)) == 0) {
			return (false);
		}
	}
	return (true);
}

size_t _next_prime(size_t n) {
	n = (n % 2 == 0) ? (n + 1) : (n + 2);

	while (!_is_prime(n)) {
		n += 2;
	}
	return (n);
}

size_t _next_pseudo_prime(size_t n) {
	return ((n % 2 == 0) ? (n + 1) : (n + 2));
}

/* assumes a 64 bit PML4 system */
typedef struct s_map_node {
	void			*key;
	union {
		void			*value;
	struct {
		uintptr_t	ptr_mask : 48;
		uint16_t	next_empty : 16;
		};
	};
}	__attribute__((aligned(16))) t_map_node;

#define MAP_NODE_GET_VAL_PTR(node) \
	((void *)(((((uintptr_t)1) << 47) - 1) & node.ptr_mask))

struct map_args {
	uint32_t	key_size;
	int			(*cmp_keys)(void *key1, void *key2);
	uint32_t	(*hash)(void *key);
	void		(*free_value)(void *value);
	void		(*free_key)(void *key);
};

typedef struct {
	uint32_t	key_size;
	uint32_t	buf_len;
	int			(*cmp_keys_fn)(void *, void *);
	t_map_node	*buf;
	/* Total size = buf_len * (sizeof(t_map_node))*/
	uint32_t	(*hash_fn)(void *val);
	void		(*free_value_fn)(void *);
	void		(*free_key_fn)(void *);
	size_t		element_count;
	size_t		collision_count;
} __attribute__((aligned(CACHE_LINE_SIZE)))	t_map;

t_map					map_new(struct map_args);
void					*map_get(t_map *map, void *key);
int						map_add(t_map *map, void *key, void *value);
void					map_destruct(t_map *map);
int						default_cmp_str_keys(char *key1, char *key2);
uint32_t				default_hash_str_fn(char *key);

static int				_try_map_add(t_map *map, void *key, void *value);
static bool				_too_many_collisions(t_map *map);
static void				_map_resize(t_map *map);
static inline uint32_t	_get_hash(t_map *map, void *key);
int						_default_cmp_keys(void *key1, void *key2,
							size_t key_size);
static inline int		_cmp_node_keys(t_map *map, t_map_node *node, void *key);

void	print_map(const t_map *map) {
	if (!map) {
		printf("Map is NULL.\n");
		return;
	}

	printf("Map:\n");
	printf("  key_size: %u\n", map->key_size);
	printf("  buf_len: %u\n", map->buf_len);
	printf("  element_count: %zu\n", map->element_count);
	printf("  collision_count: %zu\n", map->collision_count);
	printf("  cmp_keys_fn: %p\n", (void*)map->cmp_keys_fn);
	printf("  hash_fn: %p\n", (void*)map->hash_fn);
	printf("  free_value_fn: %p\n", (void*)map->free_value_fn);
	printf("  free_key_fn: %p\n", (void*)map->free_key_fn);
	printf("  buf: %p\n", (void*)map->buf);
}

/*
static inline uint32_t	fnv1a_hash(const uint8_t *key, size_t length, t_map *map) {
	uint32_t hash = 2166136261U;
	for (size_t i = 0; i < length; i++) {
		hash ^= key[i];
		hash *= 16777619U;
	}
	return (hash);
	(void)map;
}
*/

static inline uint32_t fnv1a_hash_prefetch(const uint8_t *key, size_t length,
	t_map *map) {
	t_map_node *buf = map->buf;
	size_t buf_len	= map->buf_len;
	uint32_t hash = 2166136261U;

	size_t partial_len = length / 2;
	for (size_t i = 0; i < partial_len; i++) {
		hash ^= key[i];
		hash *= 16777619U;
	}
	size_t base_offset = hash % buf_len;

	// each cache line can hold 4 nodes
	__builtin_prefetch(&buf[(base_offset + 0) % buf_len], 0, 2);
	__builtin_prefetch(&buf[(base_offset + 4) % buf_len], 0, 2);
	//__builtin_prefetch(&buf[(base_offset + 6) % buf_len], 0, 1);

	uint32_t	end = 0xff & base_offset;
	for (size_t i = partial_len; i < length; i++) {
		end ^= key[i];
		hash *= 16777619U;
	}
	return ((hash & ~(uint32_t)0x3) | end);
}

uint32_t	default_hash_str_fn(char *key) {
	size_t	hash = 1469598103934665603ULL;

	FT_ASSERT(key);
	for (char c = *key; (c = *key); key++) {
		hash = (hash << 5) + hash + c;
	}
	return (hash);
}

static inline uint32_t	_get_hash(t_map *map, void *key) {
	uint32_t	hash;
	if (map->hash_fn) {
		hash = map->hash_fn(key);
	} else {
		FT_ASSERT(map->key_size);
		//hash = fnv1a_hash(key, map->key_size, map);//9 sec runtime
		hash = fnv1a_hash_prefetch(key, map->key_size, map); //7 sec runtime
	}
	return (hash);
}

//returns 0 if they keys match, anything else if they differ
int	_default_cmp_keys(void *key1, void *key2, size_t key_size) {
	return (ft_memcmp(key1, key2, key_size));
}

static inline int	_cmp_node_keys(t_map *map, t_map_node *node, void *key) {
	if (map->cmp_keys_fn) {
		return (map->cmp_keys_fn(key, node->key));
	} else {
		return (_default_cmp_keys(key, node->key, map->key_size));
	}
}

//returns 0 if they keys match, anything else if they differ
int	default_cmp_str_keys(char *key1, char *key2) {
	return (strcmp(key1, key2));
}



/*
Arguments:
 * struct map_args:
	potentialy optional (either this or a combination of other optinal args):
	1.: key_size 
	// key_size:
	// Irrelevant if a key compare- and hash- function were given.
	optional:
	2.: cmp_keys_fn
	// cmp_keys_fn:
	// Expected to return any number if missmatch, 0 on a match
	// (like strcmp).
	// If not provided the default_cmp_keys_fn is used, which
	// 	derefereces the pointer to the keys and compares each byte. Needs
	// 	the fild 'key_size' to be != 0.
	// There is a default_cmp_str_keys which is a simple strcmp which can
	//	be given for this argument.
	3.: hash_fn
	// hash_fn:
	// If not given dereferences the key and hashes the bytes which needs the
	// field 'key_size' to be != 0.
	// There is 'default_hash_str_fn' which can be given as th args here in case
	//	strings are used as keys.
	4.: free_value_fn
	// free_value_fn:
	// Can be left out/NULL, in which case 'free' is used.
	// Since the map takes owner ship and assumes values to be allocated of
	//	all keys and values given to it this function is called by map_distruct
	//	for each key/value in the map.
	5.: free_key_fn:
	// free_key_fn:
	// See free_value above.

Error handeling (across all map functions):
 * argc does not count for the arg count
 * On malloc fail:
	1. sets errno on malloc error (leaves the errno set by malloc and returns
		after freeing the entire map)
	2. map becomes totaly invalid on internal malloc fail
	
*/
t_map	map_new(struct map_args args) {
	t_map	map = {
		.buf = 0,
		.key_size = args.key_size,
		.buf_len = FT_MAP_INIT_LEN,
		.element_count = 0,
		.collision_count = 0,
		.cmp_keys_fn = args.cmp_keys,
		.hash_fn = args.hash,
		.free_value_fn = args.free_value ? args.free_value : free,
		.free_key_fn = args.free_key ? args.free_key : free,
	};

	FT_ASSERT(sizeof(t_map) <= CACHE_LINE_SIZE);
	FT_ASSERT(sizeof(t_map_node) == 16 && CACHE_LINE_SIZE % 16 == 0);

	/* might very raly over allocate which is fine */
	size_t	alloc_size = map.buf_len * sizeof(t_map_node);
	alloc_size += CACHE_LINE_SIZE - alloc_size % CACHE_LINE_SIZE;
	map.buf = aligned_alloc((size_t)CACHE_LINE_SIZE, alloc_size);
	bzero(map.buf, alloc_size);

	FT_ASSERT(map.key_size || map.hash_fn);
	FT_ASSERT(map.buf && "Malloc failed");
	return (map);
}

/*
 * Returns a pointer to the element location in the map
 * NULL if the key has no value in the map
*/
void	*map_get(t_map *map, void *key) {
	t_map_node	*node;

	__builtin_prefetch(key, 0, 3);
	size_t		hash = _get_hash(map, key);
	node = map->buf + hash % map->buf_len;
	if (!node->key) {
		return (NULL);
	}
	while (node->key && _cmp_node_keys(map, node, key)) {
		FT_ASSERT(node->key);
		node++;
	}
	if (node->key) {
		return (MAP_NODE_GET_VAL_PTR((*node)));
	}
	return (NULL);
}

void	map_destruct(t_map *map) {
#ifndef NDEBUG
	size_t	count = 0;
#endif
	for (size_t idx = 0; idx < map->buf_len; idx++) {
		if (!map->buf[idx].key) {
			continue ;
		}
		map->free_key_fn(map->buf[idx].key);
		map->free_value_fn(MAP_NODE_GET_VAL_PTR((map->buf[idx])));
#ifndef NDEBUG
		count++;
#endif
	}
	free(map->buf);
#ifndef NDEBUG
	if (count != map->element_count) {
		printf("map should have %lu elements, but only %lu were found!\n",
			map->element_count, count);
		FT_ASSERT(0 && "wrong element count in freed map");
	}
#endif
}

//return 0 on success
int	_try_move_buf(t_map *map, t_map_node *old_buf, size_t old_len) {
	FT_ASSERT(!_too_many_collisions(map));
	for (size_t idx = 0; idx < old_len; idx++) {
		if (!old_buf[idx].key) {
			continue ;
		}
		int val = _try_map_add(map, old_buf[idx].key,old_buf[idx].value);
		FT_ASSERT(val >= 0);
		if (val > 0) {
			free(map->buf);
			return (1);
		}
	}
	FT_ASSERT(!_too_many_collisions(map));
	free(old_buf);
	return (0);
}

// malloc fail: sets errno and cleansup the maps data
static void	_map_resize(t_map *map) {
	size_t	old_len = map->buf_len;
	t_map_node	*old_buf = map->buf;
	
	while (1) {
		map->buf_len = _next_pseudo_prime(map->buf_len * 4);
		/* might very raly over allocate which is fine */
		size_t	alloc_size = map->buf_len * sizeof(t_map_node);
		alloc_size += CACHE_LINE_SIZE - alloc_size % CACHE_LINE_SIZE;
		map->buf = aligned_alloc((size_t)CACHE_LINE_SIZE, alloc_size);
		bzero(map->buf, alloc_size);
		if (!map->buf) {
			map->buf = old_buf;
			map->buf_len = old_len;
			map_destruct(map);
			FT_ASSERT(0 && "malloc fail");
			return ;
		}

		map->collision_count = 0;
		map->element_count = 0;
		if (!_try_move_buf(map, old_buf, old_len)) {
			break ;
		}
	}
	FT_ASSERT(!_too_many_collisions(map));
}

/* hard to find bug from before(caused logic chage from min element count to
	min collision count):
 * this situation:
	- 1.: this returns false duo to count < min count, even though there
		are many collisons
	- 2.: elemnets are added without collison and the element count
		threshhold is reach while not reaching the min rate
*/
static bool	_too_many_collisions(t_map *map) {
	size_t	min_collisions_to_matter = 8;
	bool	has_min_collisons = min_collisions_to_matter <= map->collision_count;
	double	min_rate = 4;
	if (has_min_collisons) {
		double	rate = ((double)map->element_count) / map->collision_count;
		if (rate < min_rate) {
			return (true);
		}
	}
	return (false);
}

// returns 0 on success, 1 if the map should be made larger,
// -1 for cases like map_add
static int	_try_map_add(t_map *map, void *key, void *value) {
	__builtin_prefetch(key, 0, 3);
	map->element_count++;
	size_t	hash = _get_hash(map, key);
	size_t	offset = hash % map->buf_len;

	FT_ASSERT(!_too_many_collisions(map)
		&& "check condition in _too_many_collisions!");

	if (!map->buf[offset].key || !_cmp_node_keys(map, map->buf + offset, key)) {
		if (map->buf[offset].key) {
			map->free_key_fn(key);
			map->free_value_fn(MAP_NODE_GET_VAL_PTR(map->buf[offset]));
		}
		map->buf[offset].key = key;
		map->buf[offset].value = value;
		return (0);
	}
	map->collision_count++;
	offset = (offset + 1) % map->buf_len;
	while (map->buf[offset].key && _cmp_node_keys(map, map->buf + offset, key)) {
		map->collision_count++;
		offset = (offset + 1) % map->buf_len;
	}
	if (map->buf[offset].key) {
		map->free_key_fn(key);
		map->free_value_fn(MAP_NODE_GET_VAL_PTR(map->buf[offset]));
	}
	map->buf[offset].key = key;
	map->buf[offset].value = value;
	if (_too_many_collisions(map)) {
		return (1);
	}
	return (0);
}

/*
 * Takes ownership of the given pointers of the key AND data!
 * on success returns 0
 * on malloc fail like map_new()
 * if the key was allready in the map:
 * 1.: frees the given key/the old value and inserts the new value
 * 2.: returns (-1)
*/
int	map_add(t_map *map, void *key, void *value) {
	__builtin_prefetch(key, 0, 3);
	int	ret = _try_map_add(map, key, value);
	if (ret <= 0) {
		return (ret);
	}
	_map_resize(map);
	return (0);
}

// tests
// todo:
//	- test for replacing values in map
//	- test for custom passed destructors
//	- test for custom passed hash fns
//	- test for custom cmp function
#include <time.h>

bool	test_basic(long long int elements) {
	struct map_args args = {0};
	args.key_size = sizeof(long long);
	t_map	map = map_new(args);

	for (long long int i = 0; i < elements; i++) {
		long long int	*key = malloc(sizeof(long long int));
		long long int	*val = malloc(sizeof(long long int));
		*key = i - elements / 2;
		*val = i - elements / 2;
		if (map_add(&map, key, val)) {
			printf("key should not be in map!\n");
			map_destruct(&map);
			return (false);
		}
	}
	for (long long int i = 0; i < elements; i++) {
		long long int key = i - elements / 2;
		long long int *val = map_get(&map, &key);
		if (!val || key != *val) {
			printf("failed test_basic\n");
			map_destruct(&map);
			return (false);
		}
	}
	map_destruct(&map);
	return (true);
}

bool	only_add(long long int elements) {
	struct map_args args = {0};
	args.key_size = sizeof(long long);
	t_map	map = map_new(args);

	for (long long int i = 0; i < elements; i++) {
		long long int	*key = malloc(sizeof(long long int));
		long long int	*val = malloc(sizeof(long long int));
		*key = i - elements / 2;
		*val = i - elements / 2;
		map_add(&map, key, val);
	}
	map_destruct(&map);
	return (true);
}

bool	lld_rdm_keys(long long int elements) {
	struct map_args args = {0};
	args.key_size = sizeof(long long);
	t_map	map = map_new(args);
	long long int **keys = malloc(sizeof(long long int *) * elements);

	long long int	key_conflicts = 0;
	for (long long int i = 0; i < elements; i++) {
		long long int	*key = malloc(sizeof(long long int));
		long long int	*val = malloc(sizeof(long long int));

		*key = ((long long)rand() << 32) | (long long)rand();
		*val = i - elements / 2;
		if (map_add(&map, key, val)) {
			key_conflicts++;
		} else {
			keys[i] = key;
		}
	}
	if (key_conflicts > elements / 100) {
		printf("key conflicts(%lld) way too sus, check test logic!\n", key_conflicts);
		FT_ASSERT(0);
	}
	for (long long int i = 0; i < elements; i++) {
		long long int	*key = keys[i];
		if (!key) {
			continue ;
		}
		long long int *val = map_get(&map, key);
		long long int expected = i - elements / 2;
		if (!val || expected != *val) {
			printf("failed test_str_key idx %lld\n", i);
			if (!val) {
				printf("no val for key %lld!\n", *key);
			} else {
				printf("expected: %lld; acutal %lld; for key %lld!\n", expected, *val, *key);
			}
			map_destruct(&map);
			free(keys);
			exit(1);
			return (false);
		}
	}
	free(keys);
	map_destruct(&map);
	return (true);
}

bool	test_str_keys(long long int elements) {
	struct map_args args = {0};
	args.cmp_keys = (int (*)(void *, void *))default_cmp_str_keys;
	args.hash = (uint32_t (*)(void *))default_hash_str_fn;
	t_map	map = map_new(args);
	char	**keys = malloc(sizeof(char *) * elements);

	long long int	key_conflicts = 0;
	for (long long int i = 0; i < elements; i++) {
		size_t	buff_size = 30;
		char			*key = malloc(sizeof(char) * buff_size);
		long long int	*val = malloc(sizeof(long long int));
		long long int	key_int = ((long long)rand() << 32) | (long long)rand();
		snprintf(key, 30, "%lldd", key_int);
		//snprintf(key, 30, "%lld", i - elements / 2);
		keys[i] = key;
		*val = i - elements / 2;
		if (map_add(&map, key, val)) {
			keys[i] = NULL;
			key_conflicts++;
		}
	}
	if (key_conflicts > elements / 100) {
		printf("key conflicts(%lld) way too sus, check test logic!\n", key_conflicts);
		FT_ASSERT(0);
	}

	for (long long int i = 0; i < elements; i++) {
		char	*key = keys[i];
		if (!key) {
			continue ;
		}
		long long int *val = map_get(&map, key);
		long long int expected = i - elements / 2;
		if (!val || expected != *val) {
			printf("failed test_str_key idx %lld\n", i);
			if (!val) {
				printf("no val for key %s!\n", key);
			} else {
				printf("expected: %lld; acutal %lld; for key %s!\n", expected, *val, key);
			}
			map_destruct(&map);
			free(keys);
			exit(1);
			return (false);
		}
	}
	free(keys);
	map_destruct(&map);
	return (true);
}

int	main(void) {
	bool	pass = true;
	srand(time(0));
#ifdef NDEBUG
	printf("NOT in debug mode\n");
#else
	printf("in debug mode\n");
#endif
	long long int n = 10000000;
	time_t	start;
	time_t	end;
	start = time(0);
	
	if (!only_add(n)) {
		pass = false;
	}

	if (!test_basic(n)) {
		pass = false;
	}

	if (!lld_rdm_keys(n)) {
		pass = false;
	}

	//if (!test_str_keys(n)) {
	//	pass = false;
	//}
	end = time(0);
	printf("Time taken: %ld seconds\n", end - start);
	if (pass) {
		printf("passed all tests\n");
	} else {
		printf("failed tests\n");
	}
	return (0);
}

