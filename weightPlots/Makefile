

#
# Makefile for Root-based Applications
# Environment variable ROOTSYS must be set
#

# location of the Python header files
 
PYTHON_INCLUDE = $(PYTHON_ROOT)/include/python2.6
PYTHON_LIB = $(PYTHON_ROOT)/lib
 
# location of the Boost Python include files and library
 
BOOST_INC = $(BOOST_ROOT)/include
BOOST_LIB = $(BOOST_ROOT)/lib
  

# Compiler/Linker and Flags
CC = g++
CFLAGS = -std=c++0x -D_REENTRANT -g -fPIC -w -I$(shell root-config --incdir) -I../common
LD = g++
LDFLAGS = -g -w

NAME = multijet_weight_common
ROOTLIBS = $(shell root-config --libs) -lboost_regex

all: $(NAME) HLTPtBinning

%.o: %.cpp
	@echo "compiling the classes..."
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME):$(NAME).o ../common/ptBinning.o ../common/HLTPtBinning.o ../common/logPtPrimeBinning.o ../common/npvBinning.o ../common/etaBinning.o PUReweighting/PUReweighter.o
	@echo "linking..."
	$(LD) $(LDFLAGS) $(NAME).o -o $(NAME) ../common/ptBinning.o ../common/HLTPtBinning.o ../common/logPtPrimeBinning.o ../common/npvBinning.o ../common/etaBinning.o PUReweighting/PUReweighter.o $(ROOTLIBS) 

listTriggers: listTriggers.o
	@echo "linking..."
	$(LD) $(LDFLAGS) listTriggers.o -o listTriggers $(ROOTLIBS) 

HLTPtBinning: ../common/HLTPtBinning_python.o ../common/HLTPtBinning.o ../common/ptBinning.o
	g++ -shared -Wl,--export-dynamic ../common/HLTPtBinning_python.o ../common/HLTPtBinning.o ../common/ptBinning.o -L$(BOOST_LIB) -lboost_python -L$(PYTHON_LIB) -lpython2.6 -o HLTPtBinning.so
 
../common/HLTPtBinning_python.o: ../common/HLTPtBinning_python.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -o $@ -c $<

clean:
	@echo "cleaning up..."
	rm $(NAME).o ../common/ptBinning.o ../common/HLTPtBinning.o ../common/logPtPrimeBinning.o ../common/npvBinning.o ../common/etaBinning.o PUReweighting/PUReweighter.o ../common/HLTPtBinning_python.o HLTPtBinning.so
