/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:22:26 by frapp             #+#    #+#             */
/*   Updated: 2024/05/21 19:05:57 by frapp            ###   ########.fr       */
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

/**************************   BASIC C FUNCTIONS   ***************************/
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
void	ft_free_2darr(char **array);
size_t	ft_strarr_size(char **ptr);

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
void	ft_putnbr_fd(int n, int fd);
int		ft_powint(int base, int expo);
void	*ft_memjoin(void *src1, void *src2, int size1, int size2);

/**********************   LINKED LISTS' FUNCTIONS   *************************/
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

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


char	*get_next_line(int fd, bool do_cleanup);

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

#endif //LIBFT_H
