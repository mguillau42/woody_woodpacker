NAME =libftprintf.a
CC = clang
FLAGS = -Wall -Werror -Wextra
INCLUDES = -I ./includes/
SRC_D = srcs
OBJS_D = objs
SRCS =	ft_printf.c \
		format_parse.c \
		format_parse_all.c \
		ft_putstr_ret.c \
		ft_putnstr.c \
		ft_nbrlen.c \
		ft_putchar_n.c \
		ft_nbrlen_b.c \
		ft_putnbr_b.c \
		display.c \
		get_spec.c \
		ft_putchar_safe.c \
		c_spec.c \
		d_spec.c \
		u_spec.c \
		o_spec.c \
		mo_spec.c \
		mu_spec.c \
		md_spec.c \
		error.c \
		display_functions.c \
		x_spec.c \
		mx_spec.c \
		mc_spec.c \
		ms_spec.c \
		p_spec.c \
		s_spec.c \
		percent_spec.c \
		ft_putwchar.c \
		ft_putwstr.c\
		ft_putnwstr.c\
		ft_wstrlen.c

OBJS = $(SRCS:.c=.o)

SRC		:=	$(addprefix $(SRC_D)/, $(SRCS))
OBJ		:=	$(addprefix $(OBJS_D)/, $(OBJS))

all: create_objd $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

create_objd: $(OBJS_D)

$(OBJS_D):
	mkdir -p $(OBJS_D)

$(OBJ): $(OBJS_D)/%.o : $(SRC_D)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_D)

fclean: clean
	rm -f $(NAME)
	rm -f debug
re: fclean all

.PHONY: clean fclean all re create_objd
