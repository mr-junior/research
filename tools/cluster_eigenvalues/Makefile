GCC=gcc
CXXFLAGS=-O3 -std=c++11 -I/home/minas/third_party/boost/install/include -I. 
TAG=opt
LFLAGS=-lstdc++ -lm -L/home/minas/third_party/boost/install/lib -Wl,-Bstatic -lboost_serialization -lboost_graph -lboost_program_options -Wl,-Bdynamic -larmadillo
DIR=objs
BIN=bin
SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, $(DIR)/%.o, $(SOURCES))
TARGET_NAME=cluster_eigens.exe
TARGET=$(BIN)/$(TARGET_NAME)

all: $(TARGET)

.PHONY: clean cleandep all

clean:
	rm -rf $(TARGET) $(DIR)/*.o

cleandep:
	rm -rf $(DIR)/*.d $(DIR)/*.P

$(DIR)/%.o : %.cpp
	@mkdir -p $(DIR)
	$(GCC) $(CXXFLAGS) -MD -c -o $@ $<
	@cp $(DIR)/$*.d $(DIR)/$*.P; \
    sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
        -e '/^$$/ d' -e 's/$$/ :/' < $(DIR)/$*.d >> $(DIR)/$*.P; \
   rm -f $(DIR)/$*.d

$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(GCC) $(OBJS) $(CXXFLAGS) $(LFLAGS) -o $@

-include $(DIR)/*.P
