OBJECTS=servo.o descriptors.o error.o linux.o usb.o
CFLAGS=-g -Wall

all: $(OBJECTS)
	$(CC) -o bin/servo $^ $(CFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f bin/servo


