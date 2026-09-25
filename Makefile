CC = gcc

PREFIX = $(D)/usr
BINDIR = $(PREFIX)/bin

PKG_CFG_LDFLAGS = $(shell pkg-config --libs raylib)
# flags comunes
BASE_CFLAGS = -g -Wall -Wextra
BASE_LDFLAGS = $(PKG_CFG_LDFLAGS)
#BASE_LDFLAGS = -lncursesw -ltinfow -lcurl -lcjson -lmpv -lpthread \
#			  -lssl -lcrypto -L./lib -Wl,-Bstatic -ldeezer_crypto -Wl,-Bdynamic

# Flags para release
CFLAGS = $(BASE_CFLAGS)
LDFLAGS = $(BASE_LDFLAGS)
TARGET = titris

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# release (dynamic libs)
all: $(TARGET)

# debug
debug: CFLAGS = $(BASE_CFLAGS) -fsanitize=address
debug: LDFLAGS = $(BASE_LDFLAGS) -static-libasan
debug: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

.PHONY: all debug clean run

