NAME = woody_woodpacker
CC = clang
FLAGS = -Wall -Werror -Wextra
INCLUDES = -I ./include/ -I ./libft/include -I ./libft/ft_printf/includes
LIBS = -L ./libft -lft
SRC_D = source
OBJS_D = obj

SRCS =	main.c \

OBJS = $(SRCS:.c=.o)

SRC		:=	$(addprefix $(SRC_D)/, $(SRCS))
OBJ		:=	$(addprefix $(OBJS_D)/, $(OBJS))

all: create_objd $(NAME)

$(NAME): $(OBJ)
	make -C ./libft/
	$(CC) $(FLAGS) $(INCLUDES) $(OBJ) $(LIBS) -o $(NAME)

create_objd: $(OBJS_D)

$(OBJS_D):
	mkdir -p $(OBJS_D)

$(OBJ): $(OBJS_D)/%.o : $(SRC_D)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_D)
	make clean -C ./libft/

fclean: clean
	make fclean -C ./libft/
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re create_objd
