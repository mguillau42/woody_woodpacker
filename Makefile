# ============================================================================= #
#                                                                               #
#               ╦ ╦╔═╗╔═╗╔╦╗╦ ╦  ╦ ╦╔═╗╔═╗╔╦╗╔═╗╔═╗╔═╗╦╔═╔═╗╦═╗                 #
#               ║║║║ ║║ ║ ║║╚╦╝  ║║║║ ║║ ║ ║║╠═╝╠═╣║  ╠╩╗║╣ ╠╦╝                 #
#               ╚╩╝╚═╝╚═╝═╩╝ ╩   ╚╩╝╚═╝╚═╝═╩╝╩  ╩ ╩╚═╝╩ ╩╚═╝╩╚═                 #
#                                                                               #
#                                                                               #
#                          mguillau42 and FlorianVenturini                      #
#             <mguillau@student.42.fr> | <fventuri@student.42.fr>               #
#                                                                               #
# ============================================================================= #

#### COMPILATION VARS ####
NAME = woody_woodpacker
CC = /usr/bin/clang
CFLAGS = -Wall -Wextra -Werror
NASM = /usr/bin/nasm
NASM_FLAGS = -f elf64

C_FILES =	get.c \
			handle_elf64.c \
			inject.c \
			libft.c \
			main.c \
			str_to_hex.c

S_FILES =	decrypt.s \
			encrypt.s

SRCS = $(addprefix srcs/,$(C_FILES)) $(addprefix srcs/,$(S_FILES))
O_FILES = $(C_FILES:.c=.o) $(S_FILES:.s=.o)
OBJ = $(addprefix obj/,$(O_FILES))
H_FILES = woody.h
INCLUDES = $(addprefix includes/,$(H_FILES))
COMPILE_FLAGS = -I includes

#### COLORS ####
NC		= \033[0m
BLACK	= \033[0;30m
RED		= \033[0;31m
GREEN	= \033[0;32m
ORANGE	= \033[0;33m
BLUE	= \033[0;34m
PURPLE	= \033[0;35m
CYAN	= \033[0;36m
LGRAY	= \033[0;37m
DGRAY	= \033[1;30m
LRED	= \033[1;31m
LGREEN	= \033[1;32m
YELLOW	= \033[1;33m
LBLUE	= \033[1;34m
LPURPLE	= \033[1;35m
LCYAN	= \033[1;36m
WHITE	= \033[1;37m

.PHONY: clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	@/usr/bin/printf "[WOODY_WOODPACKER]: Compiling $(GREEN)$(NAME)$(NC)... "
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@/bin/cp obj/decrypt.o inject_me
	@/usr/bin/printf "$(LGREEN)OK$(NC)\n"
	@/usr/bin/printf "[WOODY_WOODPACKER]: $(RED)ALL DONE$(NC)\n"

obj/%.o: srcs/%.c $(INCLUDES)
	@/bin/mkdir -p obj
	@/usr/bin/printf "[WOODY_WOODPACKER]: Compiling $(BLUE)$<$(NC) --> $(BLUE)$@$(NC)... "
	@$(CC) $(CFLAGS) -o $@ -c $< $(COMPILE_FLAGS)
	@/usr/bin/printf "$(LGREEN)OK$(NC)\n"

obj/%.o: srcs/%.s
	@mkdir -p obj
	@/usr/bin/printf "[WOODY_WOODPACKER]: Compiling $(BLUE)$<$(NC) --> $(BLUE)$@$(NC)... "
	@$(NASM) $(NASM_FLAGS) -o $@ $<
	@/usr/bin/printf "$(LGREEN)OK$(NC)\n"

clean:
	@if [ -d obj ]; then \
		/usr/bin/printf "[WOODY_WOODPACKER]: Removing $(PURPLE).o files$(NC)... "; \
		/bin/rm -rf obj; \
		/usr/bin/printf "$(LGREEN)OK$(NC)\n"; \
	fi;

fclean: clean
	@/usr/bin/printf "[WOODY_WOODPACKER]: Removing $(PURPLE)$(NAME)$(NC)... "
	@/bin/rm -f $(NAME) woody inject_me
	@/usr/bin/printf "$(LGREEN)OK$(NC)\n"

re: fclean all
