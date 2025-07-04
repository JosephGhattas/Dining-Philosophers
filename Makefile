NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c philo.c init.c utils.c action.c forks.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -lpthread -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all