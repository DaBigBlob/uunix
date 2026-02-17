all: out/kern.elf

.PHONY: qemu q clean c

# --- compiler/assembler
CC = clang
FC = -std=c11 -O3 -g0 -Wall -Wextra -pedantic # -O3 -g0
FCX = -fno-stack-protector -ffreestanding -nostdlib -nostdinc -mcmodel=medany -fno-pie -fno-pic
FCM = -target riscv64-freestanding-none

out/%.o: src/%.c
	${CC} ${FC} ${FCX} ${FCM} -c -o $@ $^

out/%.o: src/%.S
	$(CC) $(FCX) $(FCM) -c -o $@ $<

OBJS := $(patsubst src/%.c,out/%.o,$(wildcard src/*.c)) \
        $(patsubst src/%.S,out/%.o,$(wildcard src/*.S))

# --- linker
LD = ld.lld
FLD =
FLDM = -m elf64lriscv

out/kern.elf: ${OBJS} src/linker.ld
	${LD} ${FLD} ${FLDM} -T src/linker.ld --Map=out/kern.map -o $@ ${OBJS}

# --- qemu
QMU = qemu-system-riscv64
FQMU = -nographic -serial mon:stdio --no-reboot

qemu: out/kern.elf
	${QMU} -machine sifive_u -smp 5 -m 8G -bios none ${FQMU} -kernel $^

# --- alias
clean:
	rm -r out || true
	mkdir out

c: clean
q: qemu
qc: c q
