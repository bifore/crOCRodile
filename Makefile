CC        = gcc
NAME      = OCRodile
CFLAGS    = -Wall -Wextra -std=c99 -g
RM        = rm -fv
MKDIR     = mkdir -p

MAIN      = src/main.c

SRC       = src/io/bmpReader.c \
            src/utils/error.c  \
            src/utils/lkList.c \
            src/utils/vector.c \
            src/utils/string.c \
            src/utils/image.c \

BUILD_DIR = build/

SRC_TEST  = test/test.c        \
            src/utils/error.c  \
            src/utils/string.c \
            src/utils/vector.c \

TEST      = test/io/imageLoading.c \

all: $(NAME)

$(NAME): $(SRC)
	$(MKDIR) $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)$(NAME).o $(MAIN) $(SRC) $(CFLAGS)

clean:
	$(RM) $(BUILD_DIR)*.o
	$(RM) $(BUILD_DIR)*.xml

test:
	$(MKDIR) $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)$(NAME)_test.o $(SRC_TEST) $(CFLAGS)
	$(BUILD_DIR)$(NAME)_test.o $(BUILD_DIR) $(CC) '$(CFLAGS)' '$(SRC)' $(TEST)

.PHONY: test
