CC = gcc

PROG = capitalize
HDRS = processes.h
SRCS = main.c processes.c

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG) -lpthread

clean :
	rm -f $(OBJS) $(PROG)
