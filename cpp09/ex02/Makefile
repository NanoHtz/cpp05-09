# ========================= VARIABLES GLOBALES =============================== #

NAME         = PmergeMe
CC           = c++
CFLAGS       = -Wall -Wextra -Werror -std=c++98
RM           = rm -f

# ========================= DIRECTORIOS Y ARCHIVOS =========================== #

SOURCES      =  main.cpp \
				PmergeMe.cpp

# ========================= OBJETOS =========================== #
OBJSDIR     = ./obj/

SRCS        = $(sort $(SOURCES))
OBJS        = $(patsubst %.cpp,$(OBJSDIR)%.o,$(SOURCES))

# ========================= COLORES PARA EL OUTPUT =========================== #

GREEN			= \033[0;32m
YELLOW			= \033[0;33m
CYAN			= \033[0;36m
MAGENTA			= \033[0;35m
RESET			= \033[0m
RED             = \033[0;31m

# ========================= REGLAS PRINCIPALES =============================== #
.PHONY: all clean fclean re

all: $(NAME)
	@echo "${GREEN}[OK] Todo listo: $(NAME) compilada.${RESET}"

$(NAME): $(OBJS)
	@echo "\n${MAGENTA}Compilando el ejecutable $(NAME)...${RESET}\n"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# ========================= REGLAS PARA LOS OBJETOS ========================== #

$(OBJSDIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "${CYAN}Compilando: $<${RESET}"
	$(CC) $(CFLAGS) -c $< -o $@

# ========================= LIMPIEZA DE ARCHIVOS ============================= #

clean:
	@echo "${YELLOW}Limpiando archivos objeto de $(NAME)...${RESET}"
	$(RM) -r $(OBJSDIR)

fclean: clean
	@echo "${RED}Eliminando el ejecutable $(NAME)...${RESET}"
	$(RM) $(NAME)

re: fclean all
