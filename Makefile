CFLAGS =  -Wall -Wextra -Werror
SRC = my_printf.c
OBJ = $(SRC:.c=.o)
NAME = my_printf

all : $(NAME)

$(NAME) : $(OBJ)
    gcc $(CFLAGS) -o $(NAME) $(OBJ)

clean:
    rm -f *.o

fclean: clean
    rm -f $(NAME)

re: fclean all