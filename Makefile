CC = g++
MAINSRCS = main.cpp
OTHSRCS = \
	sources/BoardInstance.cpp \
	sources/Commands.cpp \
	sources/Settings.cpp \
	sources/SharedFunctions.cpp \
	sources/Incephalon.cpp

SRCS = $(MAINSRCS) $(OTHSRCS)
OBJS = $(SRCS:.cpp=.o)
TARGETS = pbrain-gomoku-ai

CFLAGS = -O3 -I./includes

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS)
	rm -rf $(OBJS)

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS) $(TARGETS)

re: fclean all clean

.PHONY: all clean re fclean
