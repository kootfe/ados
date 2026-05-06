AS = as
CC = gcc
LD = ld
CFLAGS = -ffreestanding -nostdlib -fno-pie -fno-stack-protector -mcmodel=kernel -m64 -c
ASFLAGS = --64
LDFLAGS = -n -T linker.ld
ISO_DIR = iso
BOOT_DIR = $(ISO_DIR)/boot
GRUB_DIR = $(BOOT_DIR)/grub
BUILD_DIR = build

C_SRC := $(shell find . -name "*.c")
S_SRC := $(shell find . -name "*.s")
C_OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SRC))
S_OBJ := $(patsubst %.s, $(BUILD_DIR)/%.o, $(S_SRC))
OBJS := $(C_OBJ) $(S_OBJ)

all: ado.iso

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

ado.iso: kernel.elf $(GRUB_DIR)/grub.cfg
	mkdir -p $(BOOT_DIR)
	cp kernel.elf $(BOOT_DIR)/
	grub-mkrescue -o $@ $(ISO_DIR)

run: ado.iso
	qemu-system-x86_64 -cdrom ado.iso

clean:
	rm -rf $(BUILD_DIR) *.elf *.iso
