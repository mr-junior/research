#!/bin/bash

CC=g++
CFLAGS= -ansi -O5 -Wall
LDFLAGS= -ansi -lm -Wall
OBJS_DIR=objs
BIN_DIR=bin
EXEC=$(BIN_DIR)/community $(BIN_DIR)/convert $(BIN_DIR)/hierarchy
OBJ1= $(OBJS_DIR)/graph_binary.o $(OBJS_DIR)/community.o
OBJ2= $(OBJS_DIR)/graph.o

all: $(EXEC)

$(BIN_DIR)/community : $(OBJ1) $(OBJS_DIR)/main_community.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/convert : $(OBJ2) $(OBJS_DIR)/main_convert.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/hierarchy : $(OBJS_DIR)/main_hierarchy.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

##########################################
# Generic rules
##########################################

$(OBJS_DIR)/%.o: %.cpp %.h
	@mkdir -p $(OBJS_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(OBJS_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJS_DIR)/*.o *~ $(EXEC)
