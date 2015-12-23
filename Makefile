##
## author: jokhan
## date: 2015.11.25
## 
ROOT = main
OBJ1 = $(ROOT).o
OBJ2 = GA.o
OBJ3 = wolf.o
head = ./include/
src = ./src/
CC = c99 
OPT = ggdb3
CFLAGS = `pkg-config  --cflags  opencv`
LDLIBS = `pkg-config --libs opencv`

$(ROOT):$(OBJ1) $(OBJ2) $(OBJ3)
	$(CC)  $(CFLAGS) $(OBJ1) $(OBJ2) $(OBJ3) -o $@ $(LDLIBS)
$(OBJ1):main.c $(head)GA.h $(head)wolf.h
	$(CC) -$(OPT) -c $< 
$(OBJ2):$(src)GA.c $(head)GA.h $(head)wolf.h
	$(CC) -$(OPT) -c $< 
$(OBJ3):$(src)wolf.c $(head)wolf.h
	$(CC) -$(OPT) -c $< 
clean:
	@echo "cleanning project"
	-rm $(OBJ1) $(OBJ2) $(OBJ3) $(ROOT) 
	@echo "clean completed"
test:
	-valgrind --leak-check=full --log-file=test.log ./$(ROOT) 

.PHONY: clean test
