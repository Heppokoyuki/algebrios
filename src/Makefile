TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -I../include
LDFLAGS = -Map kernel.map -s -x -T kernel.ld

$(TARGET): entry.o main.o console.o fb.o font.o asmfunc.o serial.o
	ld $(LDFLAGS) -o $@ $+
	cp $(TARGET) ../fs/

run:
	qemu-system-x86_64 -bios ../OVMF.fd -drive format=raw,file=fat:rw:../fs

clean:
	rm -f *~ ../include/*~ *.o *.map $(TARGET)

.PHONY: clean run

