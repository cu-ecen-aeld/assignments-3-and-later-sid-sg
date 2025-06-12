CC = gcc
CROSS_CC = aarch64-none-linux-gnu-gcc
TARGET = writer
SRC = writer.c 

ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	CC := $(CROSS_CC)
endif


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET)