CC        = gcc
NAME      = crOCRodile
CPPFLAGS  = `pkg-config --cflags gtk+-3.0`
CFLAGS    = -Wall -Wextra -std=c99 -g
CFLAGS_R  = -Wall -Wextra -std=c99 -O2
LDLIBS    = `pkg-config --libs gtk+-3.0` -lm
RM        = rm -fv
MKDIR     = mkdir -p

MAIN      = src/interface/interface01.c

SRC       = src/io/image.c               \
            src/processing/canny.c       \
            src/processing/binarizator.c \
            src/util/maths.c             \
            src/util/vector.c            \
            src/util/string.c            \
            src/util/image.c             \
            src/ann/network.c            \
            src/interface/interface.c    \

BUILD_DIR = build

all: $(NAME)

release: $(NAME)_R

$(NAME): $(SRC)
	$(MKDIR) $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(NAME).o $(MAIN) $(SRC) $(CFLAGS) $(CPPFLAGS) \
	$(LDLIBS)

$(NAME)_R: $(SRC)
	$(MKDIR) $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(NAME).o $(MAIN) $(SRC) $(CFLAGS_R) \
	$(CPPFLAGS) $(LDLIBS)

clean:
	$(RM) $(BUILD_DIR)/*.o
	$(RM) $(BUILD_DIR)/*.xml
	rmdir $(BUILD_DIR)
