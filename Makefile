CC     = gcc
NAME   = OCRodile
CFLAGS = -Wall -Wextra -std=c99 -g
RM     = rm -fv

SRC    = src/main.c         \
         src/io/bmpReader.c \
         src/utils/error.c  \
         src/utils/lkList.c \
         src/utils/vector.c \

all: $(NAME)

$(NAME): $(SRC)
	$(CC) -o $(NAME).o $(SRC) $(CFLAGS)

clean:
	$(RM) $(NAME).o
