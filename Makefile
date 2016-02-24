# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pivanovi <pivanovi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/10/30 15:30:51 by pivanovi          #+#    #+#              #
#    Updated: 2016/01/13 14:47:44 by pivanovi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
PRINTF = printf.a
LIBFT = libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS =	ft_printf.c


OBJS = $(SRCS:.c=.o)

CFLAGS += -I.

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(PRINTF) $(OBJS)
	libtool -static -o $(NAME) $(PRINTF) $(LIBFT)

clean:
	/bin/rm -f $(OBJS) $(PRINTF)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

