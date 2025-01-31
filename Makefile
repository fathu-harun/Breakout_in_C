CXX=clang -g
CXXW=x86_64-w64-mingw32-gcc
CFLAGS1=-Wall -g -O0 -pedantic
CFLAGS_PROFILE=-pg -O0 -Wall -Werror -ansi -pedantic 
CFLAGSWIN=-Wall -g -O0
CFLAGS_WIN_PROFILE=-Wall -pg -O0
PKG_CONFIG_PATH=pkgconfig
CFLAGSWINRELEASE=-g -Wall -Werror -ansi -pedantic -O0 -Wl,-subsystem,windows -std=c99
LDFLAGS=-lallegro -lallegro_font -L/usr/local/lib -lm
HFLAGS_RELEASE=-I./linux_release/include
LDFLAGS_RELEASE=-L./linux_release/lib64 -Wl,-Bstatic -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lfreetype -lcrypto -lwavpack -lssl -Wl,-Bdynamic -lX11 -lc -lm -lgcc_s -lX11 -lGL -lGLU -llsan
LDFLAGS_RELEASE_DYNAMIC=-Wl,-Bdynamic -lX11 -lc -lm -lgcc_s -lX11 -lGL -lGLU -llsan -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lfreetype -lcrypto -lwavpack -lssl
FRAMEWORK_RELEASE=-F./mac_release/ -framework SDL2 SDL2_image SDL2_ttf
LDFLAGSWIN=-L./win_release/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcrypto -lssl -lm -lws2_32
INCLUDE1=src/main.h src/header.h src/constants.h src/buttons.h src/text.h src/screens.h
DEBUGMODE=1
OBJS=src/main.c src/buttons.c src/text.c src/scores.c src/screens.c
OUT=./linux_release/wwr
OUTWIN=./win_release/wwr.exe
OUTMAC=./mac_release/wwr
LIBS32=-L./win_release/lib
LIBS=-L./linux_release/lib64
INC32=-I./win_release/include

all: main_rule

clean:
	rm -rf *.o main

release:
	$(CXX) $(OBJS) -o $(OUT) $(CFLAGS_PROFILE) $(HFLAGS_RELEASE) -Wno-unused-variable $(LDFLAGS_RELEASE)

dynamic_release:
	$(CXX) $(OBJS) -o $(OUT) $(CFLAGS_PROFILE_DYNAMIC) $(HFLAGS_RELEASE) -Wno-unused-variable $(LDFLAGS_RELEASE)


windows-release:
	$(CXXW) $(OBJS) $(LIBS32) $(INC32) -o $(OUTWIN) $(CFLAGSWINRELEASE) -Wno-unused-variable $(LDFLAGSWIN)

main_rule: $(OBJS)
	$(CXX) $(OBJS) -o $(OUT) $(INCLUDE1) $(CFLAGS1) $(LDFLAGS)

CC = gcc -g -pg -O0 -Wall  -Werror -ansi -pedantic -std=c99  -ferror-limit=1000
# CC = gcc -g
SDL2_DIR = /opt/homebrew/Cellar/sdl2/2.30.0
SDL2_TTF_DIR = /opt/homebrew/Cellar/sdl2_ttf/2.22.0
SDL2_IMAGE_DIR = /opt/homebrew/Cellar/sdl2_image/2.8.2_1
SDL_MIXER_DIR = /opt/homebrew/Cellar/sdl2_mixer/2.8.0
OPEN_SSL_DIR = /opt/homebrew/Cellar/openssl@3/3.2.1

SDL2_INCLUDE = $(SDL2_DIR)/include
SDL2_TTF_INCLUDE = $(SDL2_TTF_DIR)/include
SDL2_IMAGE_INCLUDE = $(SDL2_IMAGE_DIR)/include
SDL2_MIXER_INCLUDE = $(SDL_MIXER_DIR)/include
OPEN_SSL_INCLUDE= $(OPEN_SSL_DIR)/include

SDL2_LIB = $(SDL2_DIR)/lib
SDL2_TTF_LIB = $(SDL2_TTF_DIR)/lib
SDL2_IMAGE_LIB = $(SDL2_IMAGE_DIR)/lib
SDL2_MIXER_LIB = $(SDL_MIXER_DIR)/lib
OPEN_SSL_LIB = $(OPEN_SSL_DIR)/lib

CFLAGS = -g -I$(SDL2_INCLUDE) -I$(SDL2_TTF_INCLUDE) -I$(SDL2_IMAGE_INCLUDE) -I$(OPEN_SSL_INCLUDE) -I$(SDL2_MIXER_INCLUDE)
LDFLAGS = -L$(SDL2_LIB) -L$(SDL2_TTF_LIB) -L$(SDL2_IMAGE_LIB) -L$(OPEN_SSL_LIB) -L$(SDL2_MIXER_LIB)
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lssl -lcrypto -lSDL2_mixer

TARGET = main
SOURCES =src/scores.c src/main.c src/screens.c src/buttons.c   src/text.c 
OBJECTS = $(SOURCES:%.c=%.o)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

mac-clean:
	rm -rf src/*.o $(TARGET)


run: $(TARGET)
	./$(TARGET)

mac: mac-clean $(TARGET) run