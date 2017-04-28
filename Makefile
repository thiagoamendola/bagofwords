
BIN=.
CC = g++

#FLAGS= -g -O0 -Wall -Wno-unused-result -std=gnu99 -pedantic
FLAGS= -O3  -Wall -Wno-unused-result  -pedantic -fopenmp -Wno-write-strings -fsanitize=address
#-std=gnu99

FLLIBPNGINCLUDES = -I./libFL/externals/libpng/include
FLLIBJPEGINCLUDES = -I./libFL/externals/libjpeg/include

FL = -L./libFL/lib -lFL
LINKER =  $(FL)

FLFLAGS  = -I./libFL/include
FLAGSIM =  $(FLLIBPNGINCLUDES) $(FLLIBJPEGINCLUDES) -I./libFL/include $(FLAGS)


libFL:
	cd libFL; $(MAKE) ; cd -\

$@.cpp: libFL $@.cpp
	make
	$(CC) $(FLAGSIM) $@.cpp -o $(BIN)/$@  $(LINKER); \
