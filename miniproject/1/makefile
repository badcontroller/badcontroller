# program executable name
TARGET = e
# compiler flags
CFLAGS = -g -Wall -D_GNU_SOURCE
# linker flags
# LDFLAGS = -g -std=c99 -lrt -pthread -lm
LDFLAGS = -g -lrt -pthread -lm 
# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)
# define object files
OBJECTS = $(SOURCES:.c=.o)
# compiler
GCC = "/home/student/Desktop/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc"
# link programs
$(TARGET): $(OBJECTS)
	$(GCC) -o $@ $^ $(LDFLAGS) && sudo mv e /export/nfs/root/
# compile
%.o : %.c
	$(GCC) $(CFLAGS) -c -o $@ $<
# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS) 
