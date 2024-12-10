/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iziane <iziane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:22:26 by frapp             #+#    #+#             */
/*   Updated: 2024/09/05 16:35:40 by iziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdint.h>
# include <assert.h>
# include <string.h>
# include <errno.h>
# include "libft_macros.h"
# include "map.h"

/**************************   BASIC C FUNCTIONS   ***************************/
int		ft_is_charset(int c, char *set);
size_t	ft_strlen_charet(char *str, char *set);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
bool	ft_iswhitespace(char c);
int		ft_toupper(int c);
int		ft_tolower(int c);
void	ft_strtolower(char *str);
size_t	ft_strlen(const char *s);
size_t	ft_strlen_char(const char *s, char additional_terminator);
size_t	ft_strnlen(const char *s, size_t max);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memdup(const void *src, size_t size);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
char	*ft_strchr_fn(const char *s, bool fn(char));
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s1, size_t max_size);
void	free_2d_arr(char **array);
size_t	ft_strarr_size(char **ptr);

/**************************   Ilies (iziane)   ***************************/
double	atod(char *str);
void	ft_free_2d_index(char **str, int index);
int		count_words(const char *str);
/**************************   Ilies (iziane)   ***************************/

char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
bool	ft_strjoin_inplace(char **s1, char const *s2);
bool	ft_strjoin_inplace_char(char **s1, char const s2);
char	*ft_strjoin_free_s1(char *s1, char *s2);
char	*ft_strjoin_free_both(char *s1, char *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	**ft_split_fn(char const *s, bool is_sep(char));
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int		ft_putnbr_fd(int n, int fd);
int		ft_powint(int base, int expo);
void	*ft_memjoin(void *src1, void *src2, int size1, int size2);

/**********************   LINKED LISTS' FUNCTIONS   *************************/
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

void	ft_lst_dummy_del(void *content);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void	ft_lst_del_node(t_list **list, t_list *node, void (*del)(void *));

/**********************   FT_PRINTF AND FT_FPRINTF   ************************/
int		ft_printf(const char *format_str, ...);
int		ft_fprintf(int fd, const char *format_str, ...);
int		logic_part(va_list *arg, const char **format_str,
			int *written_bytes, int fd);
int		ft_printf_handle_c(va_list *arg, int *written_bytes, int fd);
int		ft_printf_handle_di(va_list *arg, int *written_bytes, int fd);
int		ft_printf_handle_p(va_list *arg, int *written_bytes, int fd);
int		ft_printf_handle_percent(int *written_bytes, int fd);
int		ft_printf_handle_s(va_list *arg, int *written_bytes, int fd);
int		ft_printf_handle_u(va_list *arg, int *written_bytes, int fd);
int		ft_printf_handle_x(va_list *arg, int *written_bytes, int fd);
int		ft_printf_handle_xx(va_list *arg, int *written_bytes, int fd);

/***********************   GET NEXT LINE *************************************/

char	*get_next_line(int fd, bool do_cleanup);

/***********************   DYNAMIC AARRY *************************************/
/*
 * private structure:
typedef struct s_dyn_header
{
	size_t		len;
	uint8_t		size;
	char		data[];
}	t_dyn_header;
*/
void	dyn_arr_free(void **arr);
void	*dyn_arr_init(uint64_t data_size, size_t init_len);
void	*dyn_arr_init2(int argc, ...);
int		dyn_arr_add_save(void **arr, void *data, size_t index);
size_t	dyn_arr_get_len(void *arr);
size_t	dyn_arr_get_size(void *arr);
int		dyn_arr_resize(void **arr);

/*****************************   QUEUE   *************************************/

typedef struct s_queue	t_queue;
typedef struct s_queue
{
	void	**buffer;
	size_t	len;
	size_t	capacity;
	size_t	head_index;
	size_t	tail_index;
}	t_queue;

// returns NULL on malloc error
t_queue	*init_que(size_t capacity);
//returns -1 if the queue is full
int8_t	enque(t_queue *queue, void *data);
void	*deque(t_queue *queue);
void	free_que(t_queue **queue);

/*****************************   FT_SORT  ************************************/

bool	cmp_float(const void *a, const void *b);
bool	cmp_ints(const void *a, const void *b);
void	ft_sort(void *arr, size_t data_size, size_t arr_len,
			bool (*cmp)(const void *, const void *));

/*****************************   OTHERS  *************************************/

void	ft_assert(bool cond, char *file, int line, char *msg);
float	str_to_float(char *str);
int		ft_put_float(int fd, float nb, size_t precision);
struct timeval	add_timeval(struct timeval a, struct timeval b);
struct timeval	sub_timeval(struct timeval a, struct timeval b);

/**************************   MINISHELL ADDITIONS   **************************/
bool	contains_non_white_spcace(char *str);
void	*ft_realloc_copy_until_zeroed(void *ptr,
			size_t new_count, size_t size);
bool	is_buffer_all_zeros(void *buffer, size_t size);
char	*ft_strstrtrim(char const *s1, char const *sub);
char	**ft_split_wildcards(char const *s);

/**************************   CUB3D ADDITIONS   ******************************/
bool	arr_append(void **arr, void *new_data, int data_size, int old_arr_len);
void	ft_free(void **ptr);


/*****************************   HASH MAP ************************************/

t_map		map_new(struct map_args);
void		*map_get(t_map *map, const void *key);
int			map_add(t_map *map, void *key, void *value);
void		map_destruct(t_map *map);
int			default_cmp_str_keys(const char *key1, const char *key2);
uint64_t	default_hash_str_fn(const char *key);

/*****************************   ANSI ESPCAPES  ******************************/

# ifndef FT_ANSI_RESET
#  define FT_ANSI_RESET "\033[0m"
# endif

// Red
# ifndef FT_ANSI_RED
#  define FT_ANSI_RED "\033[31m"
# endif

# ifndef FT_ANSI_RED_BOLD
#  define FT_ANSI_RED_BOLD "\033[31;1m"
# endif

# ifndef FT_ANSI_RED_UNDERLINE
#  define FT_ANSI_RED_UNDERLINE "\033[31;4m"
# endif

# ifndef FT_ANSI_RED_BOLD_UNDERLINE
#  define FT_ANSI_RED_BOLD_UNDERLINE "\033[31;1;4m"
# endif

// Green
# ifndef FT_ANSI_GREEN
#  define FT_ANSI_GREEN "\033[32m"
# endif

# ifndef FT_ANSI_GREEN_BOLD
#  define FT_ANSI_GREEN_BOLD "\033[32;1m"
# endif

# ifndef FT_ANSI_GREEN_UNDERLINE
#  define FT_ANSI_GREEN_UNDERLINE "\033[32;4m"
# endif

# ifndef FT_ANSI_GREEN_BOLD_UNDERLINE
#  define FT_ANSI_GREEN_BOLD_UNDERLINE "\033[32;1;4m"
# endif

// Yellow
# ifndef FT_ANSI_YELLOW
#  define FT_ANSI_YELLOW "\033[33m"
# endif

# ifndef FT_ANSI_YELLOW_BOLD
#  define FT_ANSI_YELLOW_BOLD "\033[33;1m"
# endif

# ifndef FT_ANSI_YELLOW_UNDERLINE
#  define FT_ANSI_YELLOW_UNDERLINE "\033[33;4m"
# endif

# ifndef FT_ANSI_YELLOW_BOLD_UNDERLINE
#  define FT_ANSI_YELLOW_BOLD_UNDERLINE "\033[33;1;4m"
# endif

// Blue
# ifndef FT_ANSI_BLUE
#  define FT_ANSI_BLUE "\033[0;34m"
# endif

# ifndef FT_ANSI_BLUE_BOLD
#  define FT_ANSI_BLUE_BOLD "\033[34;1m"
# endif

# ifndef FT_ANSI_BLUE_UNDERLINE
#  define FT_ANSI_BLUE_UNDERLINE "\033[34;4m"
# endif

# ifndef FT_ANSI_BLUE_BOLD_UNDERLINE
#  define FT_ANSI_BLUE_BOLD_UNDERLINE "\033[34;1;4m"
# endif

// Magenta
# ifndef FT_ANSI_MAGENTA
#  define FT_ANSI_MAGENTA "\033[35m"
# endif

# ifndef FT_ANSI_MAGENTA_BOLD
#  define FT_ANSI_MAGENTA_BOLD "\033[35;1m"
# endif

# ifndef FT_ANSI_MAGENTA_UNDERLINE
#  define FT_ANSI_MAGENTA_UNDERLINE "\033[35;4m"
# endif

# ifndef FT_ANSI_MAGENTA_BOLD_UNDERLINE
#  define FT_ANSI_MAGENTA_BOLD_UNDERLINE "\033[35;1;4m"
# endif

// Cyan
# ifndef FT_ANSI_CYAN
#  define FT_ANSI_CYAN "\033[36m"
# endif

# ifndef FT_ANSI_CYAN_BOLD
#  define FT_ANSI_CYAN_BOLD "\033[36;1m"
# endif

# ifndef FT_ANSI_CYAN_UNDERLINE
#  define FT_ANSI_CYAN_UNDERLINE "\033[36;4m"
# endif

# ifndef FT_ANSI_CYAN_BOLD_UNDERLINE
#  define FT_ANSI_CYAN_BOLD_UNDERLINE "\033[36;1;4m"
# endif

// White
# ifndef FT_ANSI_WHITE
#  define FT_ANSI_WHITE "\033[37m"
# endif

# ifndef FT_ANSI_WHITE_BOLD
#  define FT_ANSI_WHITE_BOLD "\033[37;1m"
# endif

# ifndef FT_ANSI_WHITE_UNDERLINE
#  define FT_ANSI_WHITE_UNDERLINE "\033[37;4m"
# endif

# ifndef FT_ANSI_WHITE_BOLD_UNDERLINE
#  define FT_ANSI_WHITE_BOLD_UNDERLINE "\033[37;1;4m"
# endif

// Black
# ifndef FT_ANSI_BLACK
#  define FT_ANSI_BLACK "\033[30m"
# endif

# ifndef FT_ANSI_BLACK_BOLD
#  define FT_ANSI_BLACK_BOLD "\033[30;1m"
# endif

# ifndef FT_ANSI_BLACK_UNDERLINE
#  define FT_ANSI_BLACK_UNDERLINE "\033[30;4m"
# endif

# ifndef FT_ANSI_BLACK_BOLD_UNDERLINE
#  define FT_ANSI_BLACK_BOLD_UNDERLINE "\033[30;1;4m"
# endif

#ifndef FT_ANSI_BOLD
#define FT_ANSI_BOLD "\x1b[1m"
#endif // ANSI_BOLD

#ifndef FT_ANSI_UNDERLINE
#define FT_ANSI_UNDERLINE "\x1b[4m"
#endif // ANSI_UNDERLINE

#ifndef FT_ANSI_ITALIC
#define FT_ANSI_ITALIC "\x1b[3m"
#endif // ANSI_ITALIC

#endif //LIBFT_H
