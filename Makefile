SRCDIR=./src/
INCLUDEDIR=./include/
OBJDIR=./obj/
BINDIR=./bin/

CXX=gcc

CFLAGS=-pedantic -Wall -std=c11 -O3 -I$(INCLUDEDIR) 
LIBS= -lm

DEPS = symulacja.h
SRCS = main.c symulacja.c

OBJS := $(addprefix $(OBJDIR),$(SRCS:.c=.o))
SRCS := $(addprefix $(SRCDIR),$(SRCS))
DEPS := $(addprefix $(INCLUDEDIR),$(DEP))
	
symulacja: $(OBJS) 
	$(CXX) $(CFLAGS) -o $(BINDIR)$@ $^  $(LIBS)

$(OBJDIR)%.o: $(SRCDIR)%.c $(DEPS) dirs
	$(CXX) $(CFLAGS) -c -o $@ $< 

dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

.PHONY: clean all ver


all: symulacja
clean:
	rm -f $(OBJDIR)*
ver:
	$(CXX) --version

