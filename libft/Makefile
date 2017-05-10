NAME = libft.a
CC = clang
FLAGS = -Wall -Werror -Wextra
INCLUDES = -I include -I ./ft_printf/includes
SRC_D = source
OBJS_D = obj

SRCS =	ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
ft_isdigit.c ft_isprint.c ft_memccpy.c ft_memchr.c ft_memcmp.c \
ft_memcpy.c ft_memset.c ft_memmove.c ft_putchar.c ft_putendl.c \
ft_putnbr.c ft_putstr.c ft_strcat.c ft_strchr.c ft_strcmp.c \
ft_strcpy.c ft_strdup.c ft_strlen.c ft_strncat.c ft_strncmp.c \
ft_strncpy.c ft_strnstr.c ft_strrchr.c ft_strstr.c ft_tolower.c \
ft_toupper.c ft_memalloc.c ft_memdel.c ft_strnew.c ft_strdel.c \
ft_strclr.c ft_striter.c ft_striteri.c ft_strlcat.c ft_strmap.c \
ft_strequ.c ft_strnequ.c ft_strsub.c ft_strjoin.c ft_strmapi.c \
ft_strtrim.c ft_itoa.c ft_strsplit.c ft_putchar_fd.c ft_putendl_fd.c \
ft_putstr_fd.c ft_putnbr_fd.c ft_lstnew.c ft_lstdelone.c ft_lstadd.c \
ft_lstiter.c ft_lstdel.c ft_lstmap.c get_next_line.c ft_put_color.c \
ft_msg_display.c ft_strprev.c ft_freetab.c ft_tablen.c ft_lstaddfront.c ft_merge_sort.c ft_lstcount.c

OBJS = $(SRCS:.c=.o)
OBJ		:=	$(addprefix $(OBJS_D)/, $(OBJS))

FT_PRINTF = ./ft_printf/libftprintf.a

all: create_objd $(NAME)

$(NAME): $(OBJ) $(FT_PRINTF)
	libtool -static -o $@ $^

create_objd: $(OBJS_D)

$(OBJS_D):
	mkdir -p $(OBJS_D)

$(OBJ): $(OBJS_D)/%.o : $(SRC_D)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(FT_PRINTF):
	make -C ./ft_printf

clean:
	make clean -C ./ft_printf
	rm -rf $(OBJS_D)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean all re create_objd
