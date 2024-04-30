CC = g++
FLAGS = -Wall -g
LD = -lsfml-graphics -lsfml-window -lsfml-system
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

all: app

app: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LD)

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf ./*.o
	rm -rf ./app