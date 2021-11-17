CC     := gcc

SRC    := $(wildcard *.c)
TARGET := $(patsubst %.c,%,$(SRC))

all: $(TARGET)

$(TARGET) : 
	$(CC)  $(patsubst %,%.c,$@) -o $@ 

.PHONY : clean
clean :
	rm -f $(TARGET)
