# # COLORS #
# GREEN = @echo "\033[0;32m"
# BLUE = @echo "\033[0;34m" 
# PURPLE = @echo "\033[0;35m"
# CYAN = @echo "\033[0;36m"
# RESET = "\033[1;0m"

# # RESOURCES #
# SRCS =	srcs/ft_nm.c \
# 		srcs/main.c \
# 		srcs/checker.c \
# 		srcs/_64/analisis.c \
# 		srcs/_64/debug.c

# # FLAGS #
# OBJS_DIR = objetos
# OBJS = $(SRCS:.c=.o)


# NAME = prueba
# FILE_TEST = analisis.o##tests/ft_strlen.o#

# CC = gcc

# CFLAGS = #-Wall -Werror -Wextra



# # Instructions #
# all:${NAME}

# ${NAME}: ${OBJS}
# 	$(BLUE) NM set Up $(RESET)
# 	$(CC) $(CFLAGS) -o $(NAME) $(SRCS)
	
# %.o:	%.c 
# 	$(CYAN) Compiling LIBFT Object $< $(RESET)
# 	$(CC) $(CFLAGS) -c $<

# clean:
# 	$(PURPLE) Cleaning LIBFT Objects $(RESET)
# 	@rm -rf $(OBJS_DIR)
# fclean: clean
# 	rm -rf ${NAME}

# re: fclean all

# test: re
# 	./$(NAME) $(FILE_TEST)

# .PHONY: all clean fclean re test
# COLORS #
GREEN = @echo "\033[0;32m"
BLUE = @echo "\033[0;34m" 
PURPLE = @echo "\033[0;35m"
CYAN = @echo "\033[0;36m"
RESET = "\033[1;0m"

# RESOURCES #
SRCS =	srcs/ft_nm.c \
		srcs/main.c \
		srcs/checker.c \
		srcs/_64/analisis.c \
		srcs/_64/debug.c

# FLAGS #
OBJS_DIR = objetos
OBJS = $(SRCS:srcs/%.c=$(OBJS_DIR)/%.o)

NAME = prueba
FILE_TEST = objetos/main.o#tests/ft_strlen.o#

CC = gcc

CFLAGS = #-Wall -Werror -Wextra

# Instructions #
all: $(NAME)

$(NAME): $(OBJS)
	$(BLUE) NM set Up $(RESET)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: srcs/%.c
	$(CYAN) Compiling LIBFT Object $< $(RESET)
#Creamos las carpetas necesarias para compilar
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(PURPLE) Cleaning LIBFT Objects $(RESET)
	@rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

test: re
	./$(NAME) $(FILE_TEST)

.PHONY: all clean fclean re test





