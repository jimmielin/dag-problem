CC = gcc
CFLAGS = -Wall -std=c89

OBJS = main.o graph_utils.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o is_dag $(OBJS)

clean:
	$(RM) -f is_dag $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<
