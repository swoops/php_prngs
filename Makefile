
TEST_DIR = ./tests/
MT_RAND_TEST = $(TEST_DIR)/mt_rand.test
PHP_COMBINED_LCG_TEST = $(TEST_DIR)/php_combined_lcg.test
UNIQID_TEST = $(TEST_DIR)/uniqid.test

TEST_FILES  = $(PHP_COMBINED_LCG_TEST) $(MT_RAND_TEST)  $(UNIQID_TEST)

all: $(TEST_FILES)

$(MT_RAND_TEST): 
	+$(MAKE) -C mt_rand

$(PHP_COMBINED_LCG_TEST):
	+$(MAKE) -C php_combined_lcg

$(UNIQID_TEST):
	+$(MAKE) -C uniqid

test: $(TEST_FILES)
	$(PHP_COMBINED_LCG_TEST)
	$(MT_RAND_TEST)
	$(UNIQID_TEST)

clean:
	+$(MAKE) -C php_combined_lcg clean
	+$(MAKE) -C mt_rand clean
	+$(MAKE) -C uniqid clean
