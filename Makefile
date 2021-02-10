CC ?= gcc
CFLAGS += -O3 -UDEBUG -DNO_CGI -DBWA_ENABLE_MATH_FUNC -DBWA_ENABLE_THREADS
LDFLAGS += -lm -ldl -lrt -lpthread

.PHONY: all libbwa

all: cynogale

obj/civetweb.o: civetweb.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/bwa.o: bwa.c
	$(CC) $(CFLAGS) -c -o $@ $<

cynogale: main.c obj/civetweb.o obj/bwa.o
	$(CC) -g -o $@ $^ $(LDFLAGS) -lsqlite3
#	$(CC) $(CFLAGS) -DUSE_SQLITE -DSCRIPT_CACHE -o $@ $^ -lph7 $(LDFLAGS) -lsqlite3
#	$(CC) $(CFLAGS) -o $@ $^ -lph7 $(LDFLAGS)
#	$(CC) -g -DNO_SSL -o $@ $^ -lph7 $(LDFLAGS)

clean:
	-rm -f cyanogale
	-rm -f obj/*.o

$(shell mkdir -p obj)
