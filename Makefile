#CXXFLAGS = -std=c++1z -march=native  -g -DDEBUG_ENABLED -DPMEM -fpic
CXXFLAGS = -march=native -g -DDEBUG_ENABLED -fpic 
WARNINGS = -Wall -Wpointer-arith -fpermissive #-Werror
SRC = src
BUILD = build
BIN = $(BUILD)/bin
ISAL = isa-l
DASH = dash
INC = -Iinclude -I$(ISAL)/include/isa-l
LIB = $(BUILD)/lib
CONFIG_A = -DPAVISE_VERIFY_STORE #-DPAVISE_VERIFY_LOAD -DPAVISE_COLLECT_LOAD
CONFIG_B = -DPAVISE_VERIFY_LOAD -DPAVISE_COLLECT_LOAD
CONFIG_C = -DPAVISE_VERIFY_LOAD -DPAVISE_COLLECT_LOAD
CONFIG_D = -DPAVISE_VERIFY_ISTORE
CONFIG_PIN = -DPAVISE_VERIFY_STORE -DPAVISE_PINTOOL

# evaluation knobs
ERR_INJ_EVAL = -DPAVISE_ERR_INJ_EVAL
RUNTIME_EVAL = -DPAVISE_RUNTIME_EVAL

a_runtime_eval:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_A) $(RUNTIME_EVAL) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

#all:
#	g++ $(INC) $(CXXFLAGS) $(WARNINGS) -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
#	g++ $(INC) $(CXXFLAGS) $(WARNINGS) $(CONFIG_C) -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
#	g++ $(INC) $(CXXFLAGS) $(WARNINGS) -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
#	rm -f $(LIB)/*
#	g++ $(INC) $(CXXFLAGS) $(WARNINGS) -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

#no verification
nv:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) -DPAVISE_NV -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

b:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_B) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

a:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_A) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

c:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_C) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

d:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_D) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

pin:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_PIN) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal


a_err_inj_eval:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_A) $(ERR_INJ_EVAL) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal


b_runtime_eval:
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -o $(BIN)/pavise_memops.o $(SRC)/pavise_memops.c
	g++ $(INC) $(WARNINGS) $(CONFIG_B) $(RUNTIME_EVAL) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib -o $(BIN)/pavise.o $(SRC)/pavise.cpp -lisal
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -c -L$(LIB) -L$(ISAL)/lib  -o $(BIN)/pavise_interface.o $(SRC)/pavise_interface.cpp -lisal
	rm -f $(LIB)/*
	g++ $(INC) $(WARNINGS) -O2 -g -fpic -march=native -shared -L$(LIB) -L$(ISAL)/lib -o $(LIB)/libpavise_interface.so $(BIN)/pavise.o  $(BIN)/pavise_memops.o $(BIN)/pavise_interface.o -lisal

