NAME		= c_plane

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -I include

SRC_DIR		= src
LIB_DIR		= lib
OBJ_DIR		= obj

SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/input_parser.c \
			  $(SRC_DIR)/validation.c \
			  $(SRC_DIR)/algorithm.c \
			  $(SRC_DIR)/utils.c

LIB_SRCS	= $(wildcard $(LIB_DIR)/*.c)

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIB_OBJS	= $(LIB_SRCS:$(LIB_DIR)/%.c=$(OBJ_DIR)/lib_%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/lib_%.o: $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

TEST_NAME	= test_runner
TEST_SRCS	= tests/test_algorithm.c \
			  $(SRC_DIR)/algorithm.c

test: $(TEST_NAME)
	@./$(TEST_NAME)

$(TEST_NAME): $(TEST_SRCS)
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_NAME)

test_clean:
	rm -f $(TEST_NAME)

.PHONY: all clean fclean re test test_clean
