LIB_DIR   =  ../libs/

TARGET =  ultimate_test
CFILE     =  $(TARGET).c

OBJS = $(LIB_DIR)/mt_rand.o $(LIB_DIR)/php_combined_lcg.o $(LIB_DIR)/uniqid.o

CC = gcc
CFLAGS = -Wall -g 

all: $(TARGET)
default: $(TARGET)  


$(TARGET): $(CFILE) 
	$(CC) $(CFLAGS) $(OBJS) $(CFILE) -o $(TARGET)

clean:
	rm -v $(TARGET) 
