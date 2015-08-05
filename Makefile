NAME	= ctimer.so

SRCS	= list.c \
	  timer.c

OBJS	= $(SRCS:.c=.o)

CFLAGS	+= -W -Wall -Wextra

LDFLAGS	+= -fPIC -shared -lrt

RM	= rm -f

CC	= gcc

%.o:	%.c
	@printf "[\033[32mcompiled\033[0m] %42s\n" $< | tr ' ' '.'
	@$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

all:	$(NAME)

$(NAME):$(OBJS)
	@$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@printf "[\033[36mBuilt\033[0m] %45s\n" $< | tr ' ' '.'

clean:
	@$(RM) $(OBJS)
	@printf "[\033[31mRemoved\033[0m] %43s\n" $(OBJS) | tr ' ' '.'

fclean:	clean
	@$(RM) $(NAME)
	@printf "[\033[31mRemoved\033[0m] %43s\n" $(NAME) | tr ' ' '.'

re:	fclean all

.PHONY:	all clean fclean re
