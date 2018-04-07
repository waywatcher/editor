
# target files
TARGET = $(subst .cpp,,$(subst src/,,$(wildcard src/*.cpp))) $(subst .cpp,,$(subst src/,,$(wildcard src/*/*.cpp)))
TARGET_OBJ = $(addprefix obj/,$(addsuffix .o,$(TARGET)))

#flags
SDL=C:\SDL2-2.0.8\i686-w64-mingw32
GLEW=C:\glew-2.1.0
CC=gcc
BOOST=C:\boost\boost_1_65_1
PYTHON=C:\Python35-32
CCFLAGS= -Wall -fPIC -std=c++11 -Werror -g
INCLUDES= \
 -isystem$(BOOST) \
 -isystem$(PYTHON)\include \
 -Iinc \
 -isystem$(SDL)\include \
 -isystem$(GLEW)\include
LDFLAGS= \
 --enable-stdcall-fixup \
 -L$(SDL)\lib \
 -L$(GLEW)\lib\Release\Win32 \
 -lglew32 \
 -lmingw32 \
 -lSDL2main \
 -lSDL2 \
 C:\Windows\System32\opengl32.dll \
 $(PYTHON)\libs\libpython35.a \
 $(BOOST)\stage\gcc\lib\libboost_python3-mgw63-mt-1_65_1.a \
 $(BOOST)\stage\gcc\lib\libboost_thread-mgw63-mt-1_65_1.a \
 $(BOOST)\stage\gcc\lib\libboost_system-mgw63-mt-1_65_1.a \
 $(BOOST)\stage\gcc\lib\libboost_chrono-mgw63-mt-1_65_1.a \
 C:\MinGW\lib\gcc\mingw32\6.3.0\libstdc++.a

all: graphics.exe

graphics.exe: $(TARGET_OBJ)
#here the order matters keep the flags last....
	$(CC) $(TARGET_OBJ) -o $@ $(LDFLAGS) 
 
obj/%.o: src/%.cpp inc/%.h
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@
 
obj/%/%.o: src/%/%.cpp inc/%/%.h
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm obj/*.o obj/*/*.o *.exe

.Phony: all clean