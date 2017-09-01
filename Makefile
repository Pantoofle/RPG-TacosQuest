SRC = run.c tacos.c saver.c
OBJ  := $(SRC:%.c=%.o)
TARGET = tacos

LDFLAGS=-lSDL -lSDL_mixer
CFLAGS=-O3 -Wall -I ./

all: $(TARGET)

tacos: saver.o run.o tacos.o 
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $<  $(CFLAGS) -o $@ 

mrproper: clean
	$(RM) -fr $(OBJ)
clean:
	$(RM) -fr $(TARGET)
