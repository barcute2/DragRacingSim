CXX = g++
LD = g++
CXXFLAGS = -Wall  -g -ggdb
EXEC = game
LDFLAGS = -o $(EXEC) -L/usr/lib -lGL -lGLU -lglut -lGLEW -lSDLmain -lSDL -lSDL_image
SOURCES = gg.cpp
OBJS = $(SOURCES:.cpp=.o)


$(EXEC) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS)  

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm *.o

