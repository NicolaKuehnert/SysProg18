CC = gcc

SRCDIR = src
INCLUDEDIR = include

CFLAGS = -fPIC -Wall -I$(INCLUDEDIR)
LDFLAGS = -shared -ldl
TARGET_LIB = libleakcheck.so

SRCS = $(SRCDIR)/allocation_list.c $(SRCDIR)/library.c $(SRCDIR)/hooks/calloc_hook.c $(SRCDIR)/hooks/free_hook.c $(SRCDIR)/hooks/malloc_hook.c $(SRCDIR)/hooks/realloc_hook.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^ -ldl

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
