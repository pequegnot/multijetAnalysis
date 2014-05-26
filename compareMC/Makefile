

#
# Makefile for Root-based Applications
# Environment variable ROOTSYS must be set
#

# Compiler/Linker and Flags
CC = g++
CFLAGS = -std=c++0x -D_REENTRANT -g -fPIC -w -I$(shell root-config --incdir) -I../common
LD = g++
LDFLAGS = -g -w

NAME = compareMC
ROOTLIBS = $(shell root-config --libs) 

all: $(NAME)

%.o: %.cpp
	@echo "compiling the classes..."
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME):$(NAME).o ../common/ptBinning.o ../common/HLTPtBinning.o ../common/npvBinning.o ../common/etaBinning.o
	@echo "linking..."
	$(LD) $(LDFLAGS) $(NAME).o -o $(NAME) ../common/ptBinning.o ../common/HLTPtBinning.o ../common/npvBinning.o ../common/etaBinning.o $(ROOTLIBS) 

clean:
	@echo "cleaning up..."
	rm $(NAME).o ../common/ptBinning.o ../common/HLTPtBinning.o ../common/npvBinning.o ../common/etaBinning.o
