include sources.mk

ifeq ($(BB),black)
	BB_TYPE:=black
else
	BB_TYPE:=green
endif

CC:=gcc
CC_CROSS:=arm-linux-gnueabihf-gcc
CFLAGS:= -g3 -O0 -DBB_TYPE=$(BB_TYPE) -DSTDOUT_ALIVE -DVALUES #-DLOGVALUES #-DSTDOUT_LOG #-std=gnu11
LDFLAGS:= -lpthread -lrt -lmraa -lm
INCLUDES:= -I ../include/common -I ../include/cmocka -I ../include/BeagleBone

OUTPUT_FILE_EXT:=.out

OUTPUT_FILE:= project1$(OUTPUT_FILE_EXT)


.PHONY:build
build:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUT_FILE) $(SRCS) $(LDFLAGS)

.PHONY:cross
cross:
	$(CC_CROSS) $(CFLAGS) $(INCLUDES) -o BB_$(OUTPUT_FILE) $(SRCS) $(LDFLAGS)
	scp BB_$(OUTPUT_FILE) configuration.dat gunj@192.168.7.2:~/
	#ssh gunj@192.168.7.2 sudo ./BB_$(OUTPUT_FILE)

.PHONY:run
run: build
	sudo ./project1.out

.PHONY:TEST_CONFIG
TEST_CONFIG:
	$(CC) $(CFLAGS) -DSELF_TEST $(INCLUDES) -o test_readconfig readConfiguration.c


.PHONY:clean
clean:
	rm -f *.o *.log $(OUTPUT_FILE) BB_$(OUTPUT_FILE) test_readconfig
