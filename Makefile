CC = gcc
LD = ld

CFLAGS = -Wall -Werror -O0 -I./include -g

KCFILES = $(shell find ./ -name "*.c")
KSFILES = $(shell find ./ -name "*.S")
KOBJS = $(KCFILES:.c=.o) $(KSFILES:.S=.o)

main: $(KOBJS)
	$(CC) $(KOBJS) -o main -g -lpthread 
clean:
	rm -rf $(KOBJS)
