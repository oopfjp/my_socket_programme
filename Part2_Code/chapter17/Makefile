SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
all:$(EXES)
%:%.c
	gcc $^ -o $@ -g -Wall
clean:
	$(RM) $(EXES)