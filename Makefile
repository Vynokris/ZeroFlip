PROGRAM   = zero_flip
OBJS      = src/main.o src/Math.o src/card.o src/gameBoard.o src/indicator.o src/shop.o src/ui.o src/billy.o

CXX       = g++
CXX_FLAGS = -O0 -g -m32 -std=c++17 -lm -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable
# CXX_FLAGS = -O0 -g -lm -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable
CPP_FLAGS = -I./Resources/ThirdParty/include -MMD

LIB_DIRS  = -L./Resources/ThirdParty/lib
LIBS      = -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32
# LIBS      = -lsfml-graphics -lsfml-window -lsfml-system

ifeq ($(OS), Windows_NT)
CXX_FLAGS += -m32 -std=c++17
LIB_DIRS   = -L./Resources/ThirdParty/lib
LIBS      += -lopengl32 -lwinmm -lgdi32
LNK_FLAGS  = $(LIB_DIRS) $(LIBS)

else
LNK_FLAGS  = $(LIBS)
endif

DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(CPP_FLAGS)

$(PROGRAM): $(OBJS)
	$(CXX) $^ -o $@ $(CXX_FLAGS) $(CPP_FLAGS) $(LNK_FLAGS)

clean:
	rm -f $(OBJS) $(DEPS) $(PROGRAM) && clear