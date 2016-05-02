AR=ar
CXX=g++
# Compile only
DEFINES = -DTALES_ENABLE_LIBPNG
CXXFLAGS=-std=gnu++11 -O2 -Wall -lpng $(DEFINES) -I$(IDIR)

IDIR := include
SRCDIR := src
ODIR := obj
LDIR :=

SRC := $(wildcard $(SRCDIR)/*/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(patsubst %.cpp,%.o,$(SRC)))
DEP := $(patsubst %.o,%.d,$(OBJ))
LIB := liblonely.a

all: $(LIB)

-include $(DEP)

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c -MMD -MP -MF $(@:.o=.d) -o $@ $< $(CXXFLAGS)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^
	#doxygen Doxyfile

.PHONY: clean

clean:
	rm -f $(LIB)
	rm -rf $(ODIR)
