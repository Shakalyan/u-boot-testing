KERNEL_ES = kernel.s kernel.ld

uboot:
	make -C u-boot/ qemu-riscv64_defconfig
	make -C u-boot/ CROSS_COMPILE=riscv64-unknown-linux-gnu- ARCH=riscv

kernel.elf: $(KERNEL_ES)
	riscv64-unknown-elf-as -march=rv64i -mabi=lp64 -o kernel.o kernel.s
	riscv64-unknown-elf-ld -T kernel.ld --no-dynamic-linker -m elf64lriscv -static -nostdlib -s -o kernel.elf kernel.o
	riscv64-unknown-elf-objdump -D kernel.elf > kernel.dis

kernel.bin: kernel.elf
	riscv64-unknown-elf-objcopy -O binary kernel.elf kernel.bin

run: kernel.bin
	cp kernel.bin rootfs/
	qemu-system-riscv64 -machine virt \
						-bios u-boot/u-boot.bin \
						-drive file=fat:rw:./rootfs,format=raw,media=disk,if=virtio \
						-cpu rv64 \
						-nographic \
						-serial /dev/ttyS1 \

						#-chardev serial,path=/dev/ttyS0,id=hostusbserial \
						#-device serial0,chardev=hostusbserial \
						#-serial tcp:3000,server

clean:
	-rm *.bin *.elf *.o *.dis
