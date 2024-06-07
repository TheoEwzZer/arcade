##
## EPITECH PROJECT, 2024
## Arcade
## File description:
## A Retro Platform
##

NAME	=	arcade

CC		=	g++ -g3
RM		=	rm -rf

CORE_SRC		=	$(shell find src/core -type f -name "*.cpp")
NIBBLER_SRC		=	$(shell find src/games/nibbler -type f -name "*.cpp")
SNAKE_SRC		=	$(shell find src/games/snake -type f -name "*.cpp")
NCURSES_SRC		=	$(shell find src/graphicals/ncurses -type f -name "*.cpp")
SDL2_SRC		=	$(shell find src/graphicals/sdl2 -type f -name "*.cpp")
SFML_SRC		=	$(shell find src/graphicals/sfml -type f -name "*.cpp")
GRAPHICALS_SRC	=	src/graphicals/ADisplayModule.cpp
GAME_SRC		=	src/games/AGameModule.cpp


GRAPHICALS_OBJ	=	$(GRAPHICALS_SRC:.cpp=.o)
GAME_OBJ		=	$(GAME_SRC:.cpp=.o)
CORE_OBJ		=	$(CORE_SRC:.cpp=.o)
NIBBLER_OBJ		=	$(NIBBLER_SRC:.cpp=.o)
SNAKE_OBJ		=	$(SNAKE_SRC:.cpp=.o)
NCURSES_OBJ		=	$(NCURSES_SRC:.cpp=.o)
SDL2_OBJ		=	$(SDL2_SRC:.cpp=.o)
SFML_OBJ		=	$(SFML_SRC:.cpp=.o)

FLAGS			= 	-std=c++20 -Wall -Wextra -Werror -g3

$(NAME):	core games graphicals

core:	$(CORE_OBJ)
	@$(CC) -o $(NAME) $(CORE_OBJ) -fno-gnu-unique

games:	$(NIBBLER_OBJ) $(SNAKE_OBJ) $(GAME_OBJ)
	@$(CC) -shared -fPIC -o lib/arcade_nibbler.so $(NIBBLER_OBJ) $(GAME_OBJ)
	@$(CC) -shared -fPIC -o lib/arcade_snake.so $(SNAKE_OBJ) $(GAME_OBJ)

graphicals:	$(NCURSES_OBJ) $(SDL2_OBJ) $(SFML_OBJ) $(GRAPHICALS_OBJ)
	@$(CC) -shared -fPIC -o lib/arcade_ncurses.so $(NCURSES_OBJ) $(GRAPHICALS_OBJ) -lncurses
	@$(CC) -shared -fPIC -o lib/arcade_sdl2.so $(SDL2_OBJ) $(GRAPHICALS_OBJ) -lSDL2 -lSDL2_ttf -lSDL2_image
	@$(CC) -shared -fPIC -o lib/arcade_sfml.so $(SFML_OBJ) $(GRAPHICALS_OBJ) -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cpp
	@$(CC) -c $< -o $@ -fPIC $(FLAGS)

all:	$(NAME)

clean:
	@$(RM) $(CORE_OBJ) $(NIBBLER_OBJ) $(SNAKE_OBJ) $(NCURSES_OBJ) $(SDL2_OBJ) $(SFML_OBJ)
	@$(RM) vgcore.*

fclean:	clean
	@$(RM) $(NAME) lib/arcade_nibbler.so lib/arcade_snake.so lib/arcade_ncurses.so lib/arcade_sdl2.so lib/arcade_sfml.so

re:		fclean all

.PHONY: all clean fclean re core games graphicals
