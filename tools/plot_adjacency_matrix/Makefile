GCC=g++
CXXFLAGS=-g -std=c++11 -fPIC -I. -I /usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I /usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I /usr/include/x86_64-linux-gnu/qt5/QtGui
TAG=opt
LFLAGS=-ltulip-core-4.8 -ltulip-ogl-4.8 -ltulip-gui-4.8 -lQt5Core -lQt5Gui -lQt5Widgets
DIR=objs
BIN=bin
SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, $(DIR)/%.o, $(SOURCES))
TARGET_NAME=plot_adjacency_matrix.exe
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
