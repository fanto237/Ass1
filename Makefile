C++ = g++
CFLAGS = -Wall -Wextra -Weffc++ -pedantic -fPIE
C_DEBUG_FLAGS = -g
RM = /bin/rm -f
EXEC = ausgabe
SRC = main.cpp Func.cpp ROBDD.cpp
OBJ = $(SRC:.cpp=.o)

default: $(EXEC)

$(EXEC): $(OBJ)
	$(C++) -o $(EXEC) $(OBJ)

%.o: %.cpp
	$(C++) $(C_DEBUG_FLAGS) $(CFLAGS) -MMD -c $<


%.cpp:
-include $(SRC:.cpp=.d)

clean:
	$(RM) $(EXEC) $(OBJ) *.d
