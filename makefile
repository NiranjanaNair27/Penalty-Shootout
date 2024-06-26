#changing platform dependant stuff, do not change this
# Linux (default)
LDFLAGS = -lGL -lGLU -lglut -lSOIL -ltiff -lpng
CFLAGS=-g -Wall -std=c++11
CC=g++
EXEEXT=
RM=rm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
	# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	endif
endif

PROGRAM_NAME= penaltyshootout

run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEEXT)

#To add a file to compile list use name of file.o to ensure there are no compile errors and or clean errors when using make command
$(PROGRAM_NAME): penaltyshootout.o goalkeeper.o ball.o puck.o post.o hockeystick.o mathLib3D.o imageio.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT)
