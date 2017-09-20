LIB_DIR   =  ../../libs/

TARGET		=  test
OBJS      =  $(LIB_DIR)/php_combined_lcg.o
TESTC     =  test.c

CC = gcc
CFLAGS = -Wall -g 

all: $(TARGET)
default: $(TARGET)  

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(TESTC)
	
	
clean:
	rm -v $(TARGET) 
