CXX=g++
CXXFLAGS=-O3 -std=c++11 -I.
TAG=opt
LFLAGS=-lstdc++ -L/home/minas/tasks/graph_new/boost/install/lib -Wl,-Bstatic -lboost_serialization -lboost_graph -lboost_regex -lboost_system -lboost_filesystem -Wl,-Bdynamic
DIR=objs
BIN=bin
SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, $(DIR)/%.o, $(SOURCES))
TARGET_NAME=cluster_external_links.exe
TARGET=$(BIN)/$(TARGET_NAME)

all: $(TARGET)

.PHONY: clean cleandep all 

clean:
	rm -rf $(TARGET) $(DIR)/*.o

cleandep:
	rm -rf $(DIR)/*.d $(DIR)/*.P

$(DIR)/%.o : %.cpp
	@mkdir -p $(DIR)
	$(CXX) $(CXXFLAGS) -MD -c -o $@ $<
	@cp $(DIR)/$*.d $(DIR)/$*.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(DIR)/$*.d >> $(DIR)/$*.P; \
	 rm -f $(DIR)/$*.d

$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LFLAGS) -o $@

-include $(DIR)/*.P
