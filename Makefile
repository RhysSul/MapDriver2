####
#
# Makefile
#
# Main makefile to build: - ASCIIMap character device driver
#                         - main() to test the driver
#
# Serguei Mokhov
#
##

CC=gcc
DEBUG=-g -D_DEBUG
DEFINE=-DMODULE -D__KERNEL__ -DLINUX
WARNINGS=-Wall -Wmissing-prototypes -Wmissing-declarations
#ISO=-ansi -pedantic
CC_OPTIONS=-O1 $(WARNINGS) $(ISO) $(DEBUG) $(DEFINE)

# Where to look for header files
INC=-I. -I/usr/include -I/usr/src/kernels/`uname -r`/include

DRIVER=asciimap.o
MODULE=asciimap.ko
EXE=asciimap-test
OBJ=main.o $(DRIVER)

obj-m += $(DRIVER)

all: $(EXE)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules	
	@echo "ASCII Character Device Driver and the Test Program have been built."
	@echo "Type 'make register' to register ASCII device module."
	@echo "Then follow module's suggestion to mknod /dev/asciimap with the correct major number!"

clean:
	rm -f $(EXE) $(OBJ)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

compile: $(EXE) $(OBJ)

register: $(DRIVER)
	insmod ./$(MODULE)
	modinfo $(MODULE)
	lsmod | grep ascii
	@echo ""
	@echo "ASCII Character Device Driver has been built and registered."
	@echo ""

$(EXE): main.o
	$(CC) main.o -o $(EXE)

main.o: main.c common.h
	$(CC) $(CC_OPTIONS) $(INC) -c main.c

$(DRIVER): types.h asciimap.h asciimap.c
	$(CC) $(CC_OPTIONS) $(INC) -c asciimap.c

clean-all:
	make clean
	rmmod ascii
	lsmod

# EOF
