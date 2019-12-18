CFLAGS=-std=c++11 -I /usr/local -g -Wno-pointer-arith
CC=g++

TARGETS=aslr-attacker aslr-victim single-process cross-process-attacker cross-process-victim
UTILS=util.o 

all: $(TARGETS)

$(UTILS): %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp util.hpp
	$(CC) $(CFLAGS)  -c $< 


$(TARGETS): %:%.o util.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY:	clean

clean:
	rm -f *.o $(HELPERS) $(TARGETS) 