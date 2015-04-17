CC=g++
CFLAGS=-Wall
OBJS=board.o characters.o main.o game.o

all: demotywator

$(OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

demotywator: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o demotywator

