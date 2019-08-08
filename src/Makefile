TARGET = kernel.bin
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -fno-stack-protector -I../include
LDFLAGS = -Map kernel.map -s -x -T kernel.ld

$(TARGET): entry.o main.o fb.o fbcon.o font.o asmfunc.o serial.o idt.o handler.o util.o hpet.o acpi.o common.o
	ld $(LDFLAGS) -o $@ $+
	cp $(TARGET) ../fs/

run:
	sudo qemu-system-x86_64 -m 4G -serial stdio \
	-drive if=pflash,format=raw,readonly,unit=0,file=/usr/share/ovmf/x64/OVMF_CODE.fd \
	-drive if=pflash,format=raw,unit=1,file=/usr/share/ovmf/x64/OVMF_VARS.fd \
	-drive format=raw,file=fat:rw:../fs

write: $(TARGET)
	sudo mount /dev/sdb1 /mnt
	sudo rm -rf /mnt/*
	sudo cp -r ../fs/* /mnt
	sudo umount /mnt

clean:
	rm -f *~ ../include/*~ *.o *.map $(TARGET)

.PHONY: clean run

