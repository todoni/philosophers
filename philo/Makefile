# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/11 15:38:21 by sohan             #+#    #+#              #
#    Updated: 2022/05/13 18:18:55 by sohan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJECT_DIR = objects
FILES = \
		main\
		atoi_safe\
		life_of_philosopher\
		utils\
		monitor

vpath %.c ./

SRC = $(addsuffix .c, $(FILES))
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(addsuffix .o, $(FILES)))

CC = cc -g $(if $(SANITIZER),-fsanitize=$(SANITIZER))
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

fclean:
	rm -rf $(OBJECT_DIR)
	rm -f $(NAME)

re :
	rm -rf $(OBJECT_DIR)
	rm -f $(NAME)
	make $(NAME)

.PHONY: all clean fclean re
