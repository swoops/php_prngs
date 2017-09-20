LIB_DIR   =  ../libs/
TEST_DIR  =  ../tests/

BASE_NAME =  uniqid
TARGET    =  $(LIB_DIR)/$(BASE_NAME).o
TEST_OUT  =  $(TEST_DIR)/$(BASE_NAME).test
TESTC     =  test.c
CFILE     =  $(BASE_NAME).c

TEST_OBJS = ../libs/mt_rand.o ../libs/php_combined_lcg.o $(TARGET)

CC = gcc
CFLAGS = -Wall -g 

all: $(TARGET) test
default: $(TARGET)  


$(TARGET): $(CFILE) 
	$(CC) $(CFLAGS) -c $(CFILE) -o $(TARGET)

# test stuff is a bit different then the others
test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -DPHP_FILE_MACRO=\"php\ $(shell pwd)/test.php\" -o $(TEST_OUT) $(TEST_OBJS) $(TESTC)
	
clean:
	rm -v $(TARGET) $(TEST_OUT)
