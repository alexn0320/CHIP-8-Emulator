CC = gcc
SDL2_CONFIG = `sdl2-config --cflags --libs`
FLAGS = -Wall -g
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

all: app

app: $(OBJS)
	$(CC) $(OBJS) -o $@ $(SDL2_CONFIG)

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf ./*.o
	rm -rf ./app
