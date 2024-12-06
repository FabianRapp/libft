#include "libft.h"

#include <math.h>

//#ifndef FT_HASH_PRIME
//# define FT_HASH_PRIME 2166136261
//#endif //FT_HASH_PRIME

#ifndef FT_MAP_INIT_LEN
# define FT_MAP_INIT_LEN 2
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


typedef struct s_map_node	t_map_node;
/* uses a linked list for collisions */
typedef struct s_map_node {
	void			*key;
	t_map_node		*next;
	void			*value;
}	t_map_node;

typedef struct {
	t_map_node	*buf;
	size_t		key_size;
	size_t		buf_len;
	size_t		prime;
	/* Total size = buf_len * (sizeof(t_map_node))*/
	int			(*cmp_keys_fn)(void *, void *);
	size_t		(*hash_fn)(void *val, size_t hash);
	void		(*free_value_fn)(void *);
	void		(*free_key_fn)(void *);
	size_t		element_count;
	size_t		collision_count;
}	t_map;

t_map					map_new(int argc, size_t key_size, ...);
void					*map_get(t_map *map, void *key);
int						map_add(t_map *map, void *key, void *value);
void					map_destruct(t_map *map);
int						default_cmp_str_keys(char *key1, char *key2);
size_t					default_hash_str_fn(char *key, size_t prime);

static int				_try_map_add(t_map *map, void *key, void *value);
static bool				_too_many_collisions(t_map *map);
static void				_map_resize(t_map *map);
static inline size_t	_get_hash(t_map *map, void *key);
int						_default_cmp_keys(void *key1, void *key2,
							size_t key_size);
int						_cmp_node_keys(t_map *map, t_map_node *node, void *key);

void	print_map(const t_map *map) {
	if (!map) {
		printf("Map is NULL.\n");
		return;
	}

	printf("Map:\n");
	printf("  key_size: %zu\n", map->key_size);
	printf("  buf_len: %zu\n", map->buf_len);
	printf("  element_count: %zu\n", map->element_count);
	printf("  collision_count: %zu\n", map->collision_count);
	printf("  cmp_keys_fn: %p\n", (void*)map->cmp_keys_fn);
	printf("  hash_fn: %p\n", (void*)map->hash_fn);
	printf("  free_value_fn: %p\n", (void*)map->free_value_fn);
	printf("  free_key_fn: %p\n", (void*)map->free_key_fn);
	printf("  buf: %p\n", (void*)map->buf);
}

static inline size_t fnv1a_hash(const uint8_t *key, size_t length) {
	size_t hash = 1469598103934665603ULL;
	const size_t fnv_prime = 1099511628211ULL;

	for (size_t i = 0; i < length; i++) {
		hash ^= (size_t)key[i];
		hash *= fnv_prime;
	}
	return (hash);
}

size_t	default_hash_str_fn(char *key, size_t prime) {
	size_t	hash = prime;

#ifndef NDEBUG
	assert(key);
#endif // NDEBUG
	for (char c = *key; (c = *key); key++) {
		hash = (hash << 5) + hash + c;
	}
	return (hash);
}

static inline size_t	_get_hash(t_map *map, void *key) {
	size_t	hash;
	if (map->hash_fn) {
		hash = map->hash_fn(key, map->prime);
	} else {
#ifndef NDEBUG
		assert(map->key_size);
#endif // NDEBUG
		hash = fnv1a_hash(key, map->key_size);
		//hash = _default_hash_fn(key, map->key_size, map->prime);
	}
	return (hash);
}

//returns 0 if they keys match, anything else if they differ
int	_default_cmp_keys(void *key1, void *key2, size_t key_size) {
	return (ft_memcmp(key1, key2, key_size));
}

int	_cmp_node_keys(t_map *map, t_map_node *node, void *key) {
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
 *argc has to be >= 2 and the value_size has to be given
 *Argument ordering:
	mandetory:
	0.: argc
	potentialy optional (either this or a combination of other optinal args):
	1.: key_size 
	// key_size:
	// Irrelevant if a key compare-/hash-function (arg 3/4) were given. Has
	// to be always given anyway to not mess with va_arg
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
	4.: free_value
	// free_value:
	// Can be left out/NULL, in which case 'free' is used.
	// Since the map takes owner ship and assumes values to be allocated of
	//	all keys and values given to it this function is called by map_distruct
	//	for each key/value in the map.
	5.: free_key:
	// free_key:
	// See free_value above.

 *If no hash_fn was given the default hash function will be used (be carefull
	with this when for example using a string as key: the default will hash the
	pointer to the string, not the content. For this case use the
	'default_str_hash_fn' function which is also in this file)
 *If a destructor is given it is called on a value before freeing it

Error handeling (across all map functions):
 * argc does not count for the arg count
 * On malloc fail:
	1. sets errno on malloc error (leaves the errno set by malloc and returns after
 	freeing potential internal data)
	2. map becomes totaly invalid on internal malloc fail
	
*/
t_map	map_new(int argc, size_t key_size, ...) {
	va_list	argv;
	t_map	map = {
		.buf = 0,
		.key_size = key_size,
		.prime = 2166136261,
		.buf_len = FT_MAP_INIT_LEN,
		.element_count = 0,
		.collision_count = 0,
		//.hash_fn = NULL,
	};

#ifndef NDEBUG
	assert(argc >= 1 && "map_new: need argc >=2");
#endif
	va_start(argv, key_size);
	va_end(argv);
	/* if the destructor is NULL default_hash_fn will be used */
	map.cmp_keys_fn = argc >= 2 ? va_arg(argv, int (*)(void *, void *)) : NULL;
	map.hash_fn = argc >= 3 ? va_arg(argv, size_t (*)(void *, size_t)) : NULL;
	map.free_value_fn = argc >= 4 ? va_arg(argv, void (*)(void *)) : free;
	map.free_key_fn = argc >= 5 ? va_arg(argv, void (*)(void *)) : free;
	//map.buf = calloc(map.buf_len, map.value_size + sizeof(t_map_node));
	map.buf = calloc(map.buf_len, sizeof(t_map_node));

#ifndef NDEBUG
	assert(map.key_size || map.hash_fn);
	assert(map.buf && "Malloc failed");
#endif
	return (map);
}

/*
 * Returns a pointer to the element location in the map
 * NULL if the key has no value in the map
*/
void	*map_get(t_map *map, void *key) {
	t_map_node	*node;

	size_t		hash = _get_hash(map, key);
	node = map->buf + hash % map->buf_len;
	if (!node->key) {
		return (NULL);
	}
	while (node && _cmp_node_keys(map, node, key)) {
		assert(node->key);
		node = node->next;
#ifndef NDEBUG
#endif // NDEBUG
	}
	if (node) {
		return (node->value);
	}
	return (NULL);
}

void	map_destruct(t_map *map) {
#ifndef NDEBUG
	size_t	count = 0;
#endif
	for (size_t idx = 0; idx < map->buf_len; idx++) {
		t_map_node	*cur = map->buf + idx;
		t_map_node	*last;
	
		if (cur->key) {
			map->free_key_fn(cur->key);
			map->free_value_fn(cur->value);
#ifndef NDEBUG
			count++;
#endif
			cur = cur->next;
			while (cur) {
				last = cur;
				cur = cur->next;
				free(last->key);
				free(last->value);
				free(last);
#ifndef NDEBUG
				count++;
#endif
			}
		}
	}
	free(map->buf);
#ifndef NDEBUG
	if (count != map->element_count) {
		printf("map should have %lu elements, but only %lu were found!\n",
			map->element_count, count);
		assert(0 && "wrong element count in freed map");
	}
#endif
}

void	free_buf(t_map_node *buf, size_t len) {
	for (size_t idx = 0; idx < len; idx++) {
		t_map_node	*cur = buf + idx;
		if (!cur->key) {
			continue ;
		}
		cur = cur->next;
		while (cur) {
			t_map_node	*last = cur;
			cur = cur->next;
			free(last);
		}
	}
	free(buf);
}

//return 0 on success
int	_try_move_buf(t_map *map, t_map_node *old_buf, size_t old_len) {
	assert(!_too_many_collisions(map));
	for (size_t idx = 0; idx < old_len; idx++) {
		t_map_node	*cur_src = old_buf + idx;
		if (!cur_src->key) {
			continue ;
		}
		while (cur_src) {
			int val = _try_map_add(map, cur_src->key, cur_src->value);
#ifndef NDEBUG
			assert(val >= 0);
#endif //NDEBUG
			if (val > 0) {
				free_buf(map->buf, map->buf_len);
				return (1);
			}
			cur_src = cur_src->next;
		}
	}
	if (_too_many_collisions(map)) {
		assert(0);
	}
	free_buf(old_buf, old_len);
	return (0);
}

// todo: refactor
// malloc fail: sets errno and cleansup the maps data
static void	_map_resize(t_map *map) {
	//printf("*********************************************************\n");
	//printf("resizing map:\nElement count: %lu\nCollision count: %lu\nRate: %lf\n",
	//	map->element_count, map->collision_count,
	//	((double)map->element_count) / map->collision_count);

	size_t	old_len = map->buf_len;
	t_map_node	*old_buf = map->buf;
	
	size_t resizes = 0;
	while (1) {
		resizes++;
		map->buf_len = _next_prime(map->buf_len * 4);
		//printf("new len: %lu\n", map->buf_len);
		//printf("Old len: %lu\nNew len: %lu\n", old_len, map->buf_len);
		map->buf = calloc(map->buf_len, sizeof(t_map_node));
		if (!map->buf) {
			map->buf = old_buf;
			map->buf_len = old_len;
			map_destruct(map);
#ifndef NDEBUG
			assert(0 && "malloc fail");
#endif // NDEBUG
			return ;
		}

		map->collision_count = 0;
		map->element_count = 0;
		if (!_try_move_buf(map, old_buf, old_len)) {
			break ;
		}
	}

	//printf("After resize map:\nElement count: %lu\nCollision count: %lu\nRate: %lf\n",
	//	map->element_count, map->collision_count,
	//	((double)map->element_count) / map->collision_count);
#ifndef NDEBUG
	if (_too_many_collisions(map)) {
		assert(0);
	}
#endif
	printf("resize attempts %lu\n", resizes);
}

/* hard to find bug fro before(caused logic chage from min elent count to
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
	double	min_rate = 4.0;
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
	map->element_count++;
	size_t	hash = _get_hash(map, key);
	size_t	offset = hash % map->buf_len;

	t_map_node	*head = map->buf + offset;
#ifndef NDEBUG
	assert(!_too_many_collisions(map)
		&& "check condition in _too_many_collisions!");
#endif //NDEBUG

	if (!head->key) {
		head->key = key;
		head->value = value;
		return (0);
	}

	t_map_node	*node = head;

	map->collision_count++;
	while (node->next && _cmp_node_keys(map, node->next, key)) {
		map->collision_count++;
		node = node->next;
	}
	if (node->next) {
		/* exact key was allready in map (not only same hash)
		todo: I dont like this but idk what else to do that is not super
		annoying to use */
		map->collision_count--;
		map->element_count--;
		map->free_value_fn(key);
		map->free_value_fn(value);
		return (-1);
	}

	node->next = calloc(1, sizeof(t_map_node));
	node = node->next;
	node->key = key;
	node->value = value;
	node->next = NULL;

	if (_too_many_collisions(map)) {
		return (1);
	}
	return (0);
}

/*
 * Takes ownership of the given pointers of the key AND data!
 * on success returns 0
 * if the key was allready in the map:
 * 1.: frees the given key and value in case the key was allready in the map
 * 2.: returns (-1)
*/
int	map_add(t_map *map, void *key, void *value) {
	int	ret = _try_map_add(map, key, value);
	if (ret <= 0) {
		return (ret);
	}
	_map_resize(map);
	return (0);
}

// tests
#include <time.h>

bool	test_basic(long long int elements) {
	t_map	map = map_new(1, sizeof(long long));

	for (long long int i = 0; i < elements; i++) {
		long long int	*key = malloc(sizeof(long long int));
		long long int	*val = malloc(sizeof(long long int));
		*key = i - elements / 2;
		*val = i - elements / 2;
		if (map_add(&map, key, val)) {
			printf("key should not bee in map!\n");
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

bool	lld_rdm_keys(long long int elements) {
	t_map	map = map_new(1, sizeof(long long));
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
		assert(0);
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
	t_map	map = map_new(3, 0, default_cmp_str_keys, default_hash_str_fn);
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
		assert(0);
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
	if (!lld_rdm_keys(n)) {
		pass = false;
	}

	//if (!test_basic(n)) {
	//	pass = false;
	//}
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
