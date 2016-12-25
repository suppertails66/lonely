AR=ar
CXX=g++
# Compile only
DEFINES = 
CXXFLAGS=-std=gnu++11 -O2 -Wall $(DEFINES) -I$(IDIR)

IDIR := include
SRCDIR := src
ODIR := obj
LDIR :=

SRC := $(wildcard $(SRCDIR)/*/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(patsubst %.cpp,%.o,$(SRC)))
DEP := $(patsubst %.o,%.d,$(OBJ))
LIB := libclapton.a
LIBNAME := clapton
BININ := $(SRCDIR)/main.cpp
BINOUT := clapton
#LIB := clapton

all: $(LIB)

-include $(DEP)

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
#	$(CXX) -MMD -MP -MF $(@:.o=.d) -o $@ $< $(CXXFLAGS)
	$(CXX) -I../liblonely/include -L../liblonely -llonely -lpng -c -MMD -MP -MF $(@:.o=.d) -o $@ $< $(CXXFLAGS)
	

$(LIB): $(OBJ)
	$(CXX) $(BININ) -o$(BINOUT) $(OBJ) -I../liblonely/include -L../liblonely -llonely -lpng $(CXXFLAGS)
#	$(AR) rcs $@ $^
	#doxygen Doxyfile

.PHONY: clean

clean:
	rm -f $(LIB)
	rm -rf $(ODIR)
	rm -f $(BINOUT)
