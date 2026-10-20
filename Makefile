CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lcurl

TARGET = llm_bridge
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)