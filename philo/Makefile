# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/11 15:38:21 by sohan             #+#    #+#              #
#    Updated: 2022/03/11 15:39:01 by sohan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJECT_DIR = objects
FILES = \
		philo\
		atoi_safe\

vpath %.c ./

SRC = $(addsuffix .c, $(FILES))
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(addsuffix .o, $(FILES)))

CC = cc -g
CFLAGS = -Wall -Wextra -Werror

NAME = philo

all: $(NAME)

$(NAME): $(OBJECT_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJECT_DIR):
	@echo make object folder
	@mkdir -p $(OBJECT_DIR)

$(OBJECT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECT_DIR)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re