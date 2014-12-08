GCC	=	gcc

CFLAGS	=	-std=c99 -Wall -Werror -pedantic -pipe

CLIBS	=	-lssl -lcrypto -lm

SRC	=	LolOTP.c	\
		base32.c	\

OBJ	=	$(SRC:.c=.o)

NAME	=	LolOTP

$(NAME):	$(OBJ)
		$(GCC) $(OBJ) -o $(NAME) $(CFLAGS) $(CLIBS)

all:		$(NAME)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re:		fclean all
