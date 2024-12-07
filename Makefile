NAME	:=	libft.a
NDEBUG	:=
-DNDEBUG
CFLAGS	:=	-Wall -Wextra -Werror -Ofast -march=native -g -fsanitize=address $(NDEBUG)
LDFLAGS := -lm $(NDEBUG)
#-fsanitize=address -g
CC		:=	cc

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

OBJ_DIR	:=	./obj/
DIRS 	:= ./obj/  ./obj/ft_printf

SRCS	:=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
			ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
			ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
			ft_striteri.c ft_putnbr_fd.c \
			ft_charset.c \
			ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
			ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
			ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \
			ft_lst_dummy_delete.c ft_list_del_node.c \
			get_next_line_utils.c get_next_line.c \
			ft_printf/ft_printf.c \
			ft_printf/ft_printf_pattern1.c \
			ft_printf/ft_printf_format_nbs.c \
			ft_powint.c ft_realloc_copy_until_zeroed.c \
			ft_strarr_size.c ft_free_2darr.c ft_strjoin_free_s1.c ft_strjoin_free_both.c \
			ft_split_wildcards.c ft_split_fn.c ft_iswhitespace.c \
			arr_append.c ft_free.c \
			ft_dynamic_arr.c ft_queue.c \
			ft_assert.c ft_free_2d.c ft_count_word.c atod.c \
			ft_put_float.c ft_str_to_float.c \
			ft_sort.c

OBJS	:=	$(SRCS:%.c=$(OBJ_DIR)%.o)

#all: $(DIRS) $(SRCS) $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) ft_map.c $(LDFLAGS)

all: $(NAME)

$(NAME): $(DIRS) $(SRCS) $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)libft build$(CLEAR)"

prof: CFLAGSa+= -pg
prof: LDFLAGS += -pg
prof: $(all)
	$(CC) $(CFLAGS) $(OBJS) ft_map.c -lm -o profiled_binary $(LDFLAGS)
	./profiled_binary
	gprof -l profiled_binary gmon.out > prof
	@echo "$(YELLOW)Profile report generated in prof$(CLEAR)"

$(DIRS):
	@mkdir -p $(DIRS)

$(OBJ_DIR)%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)libft objects cleaned$(CLEAR)"

fclean:
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)libft fclean$(CLEAR)"

re: fclean all

flags: CFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: LDFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: $(NAME)

leaks: CFLAGS += -DLEAK_CHECK=1
leaks: $(NAME) $(OBJS)

.PHONY: all clean fclean re prof
