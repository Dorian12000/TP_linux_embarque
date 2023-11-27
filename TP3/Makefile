# List of source files
SOURCES = gpio-leds.c \

# List of object files to be generated
OBJECTS = $(SOURCES:.c=.o)

CFLAGS_MODULE=-fno-pic \
				-w

obj-m := $(OBJECTS)
FPGA_KERNEL=~/linux-socfpga/
KERNEL_SOURCE=/lib/modules/$(shell uname -r)/build

EXECUTABLE = gpio-leds.ko
REMOTE_TARGET = root@192.168.88.18:~/TP_linux

all :
	make -C $(KERNEL_SOURCE)  M=$(PWD) modules 2>&1 | tee build_output.txt
fpga : 
	make -C $(FPGA_KERNEL)  M=$(PWD) modules EXTRA_CFLAGS=$(CFLAGS_MODULE)
clean:
	make -C $(KERNEL_SOURCE) M=$(PWD) clean
	make -C $(FPGA_KERNEL) M=$(PWD) clean
	rm -f build_output.txt
install :
	make −C $(KERNEL_SOURCE) M=$(PWD) modules install
	
# Rule to copy the executable to a remote location using scp
copy_remote: $(EXECUTABLE)
	scp $(EXECUTABLE) $(REMOTE_TARGET)
