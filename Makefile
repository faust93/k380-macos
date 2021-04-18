all: k380

CC=gcc
COBJS=k380.o
OBJS=$(COBJS)
CFLAGS+=-I. -Wall -c
LIBS=libhidapi.a -framework IOKit -framework CoreFoundation -framework AppKit


k380: $(OBJS)
	$(CC) -Wall $(OBJS) $(LIBS) -o k380

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o k380

.PHONY: clean