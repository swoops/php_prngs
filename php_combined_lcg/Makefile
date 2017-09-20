LIB_DIR   =  ../libs/
TEST_DIR  =  ../tests/

BASE_NAME =  php_combined_lcg
TARGET    =  $(LIB_DIR)/$(BASE_NAME).o
TEST_OUT  =  $(TEST_DIR)/$(BASE_NAME).test
TESTC     =  test.c
CFILE     =  $(BASE_NAME).c

CC = gcc
CFLAGS = -Wall -g 

all: $(TARGET) test
default: $(TARGET)  


$(TARGET): $(CFILE) 
	$(CC) $(CFLAGS) -c $(CFILE) -o $(TARGET)

test: $(TARGET)
	$(CC) $(CFLAGS) -DPHP_FILE_MACRO=\"php\ $(shell pwd)/test.php\" -o $(TEST_OUT) $(TARGET) $(TESTC)
	
	
clean:
	rm -v $(TARGET) $(TEST_OUT)
