
# target files
TARGET = $(subst .cpp,,$(subst src/,,$(wildcard src/*.cpp))) $(subst .cpp,,$(subst src/,,$(wildcard src/*/*.cpp)))
TARGET_OBJ = $(addprefix obj/,$(addsuffix .o,$(TARGET)))

#flags
SDLLIB=C:\SDL\i686-w64-mingw32\lib
SDLINC=C:\SDL\i686-w64-mingw32\include
GLEWLIB=C:\glew\lib\Release\Win32
GLEWINC=C:\glew\include
CC=i686-w64-mingw32-g++
CCFLAGS= -Wall -fPIC -std=c++11 -Werror -Dcimg_display=0 -isystemC:/boost_1_65_1 -Iinc -isystem$(SDLINC) -isystem$(GLEWINC)
LDFLAGS= -L$(SDLLIB) -L$(GLEWLIB) -lglew32 -lmingw32 -lSDL2main -lSDL2 C:\Windows\System32\opengl32.dll -pthread

all: graphics.exe

graphics.exe: $(TARGET_OBJ)
#here the order matters keep the flags last....
	$(CC) $(TARGET_OBJ) -o $@ $(LDFLAGS) 
 
obj/%.o: src/%.cpp inc/%.h
	$(CC) $(CCFLAGS) -c $< -o $@
 
obj/%/%.o: src/%/%.cpp inc/%/%.h
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	del /s obj\*.o *.exe

.Phony: all clean