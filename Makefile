CFLAGS = -std=c++11

PROGRAM = test.out
SOURCE  = main.cpp
     
all: $(PROGRAM)
     
$(PROGRAM): $(SOURCE) ; g++ $(CFLAGS) -g -o $(PROGRAM) $(SOURCE)
     
clean: ; rm -f $(PROGRAM)
