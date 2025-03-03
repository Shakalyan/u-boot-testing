KERNEL_ES = kernel.s kernel.ld
U_BOOT_REPO = ../u-boot
U_BOOT_BIN = $(U_BOOT_REPO)/u-boot.bin

PTY_REDIRECT ?= yes
QEMU_FLAGS = -machine virt -bios $(U_BOOT_BIN) -drive file=fat:rw:./rootfs,format=raw,media=disk,if=virtio -cpu rv64 -nographic

ifeq ($(PTY_REDIRECT), yes)
QEMU_FLAGS+= -serial pty
endif

CC_pref = riscv64-unknown-linux-gnu

$(U_BOOT_BIN):
	make -C $(U_BOOT_REPO) qemu-riscv64_defconfig
	make -C $(U_BOOT_REPO) CROSS_COMPILE=$(CC_pref)- ARCH=riscv

kernel.elf: $(KERNEL_ES)
	$(CC_pref)-as -march=rv64i -mabi=lp64 -o kernel.o kernel.s
	$(CC_pref)-ld -T kernel.ld --no-dynamic-linker -m elf64lriscv -static -nostdlib -s -o kernel.elf kernel.o
	$(CC_pref)-objdump -D kernel.elf > kernel.dis

kernel.bin: kernel.elf
	$(CC_pref)-objcopy -O binary kernel.elf kernel.bin

run: $(U_BOOT_BIN) kernel.bin
	cp kernel.bin rootfs/
	qemu-system-riscv64 $(QEMU_FLAGS)

clean:
	-rm *.bin *.elf *.o *.dis
