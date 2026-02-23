all: out/kern.elf

.PHONY: qemu q clean c

# --- compiler/assembler
CC = clang
FC = -std=c17 -Wall -Wextra -pedantic
FCR = -O3 -g0
# we shall try to be independent of the "niceties" that the compiler provides to be as compiler agnostic as possible
# we shall tolerate unused args to support more compilers
FCX = -fno-stack-protector -ffreestanding -nostdlib -nostdinc -fno-builtin -mcmodel=medany -fno-pie -fno-pic
FCM = -target riscv64-freestanding-none

out/%.o: src/%.c
	${CC} ${FC} ${FCR} ${FCX} ${FCM} -c -o $@ $<

out/%.o: src/%.S
	${CC} ${FCX} ${FCM} -c -o $@ $<

OBJS := $(patsubst src/%.c,out/%.o,$(wildcard src/*.c)) \
        $(patsubst src/%.S,out/%.o,$(wildcard src/*.S))

# --- linker
LD = ld.lld
# we shall tolerate unused args to support more linkers
FLD = --no-pie
FLDR = --lto=full --strip-all
FLDM = -m elf64lriscv

out/kern.elf: ${OBJS} src/linker.ld
	${LD} ${FLD} ${FLDR} ${FLDM} -T src/linker.ld --Map=out/kern.map -o $@ ${OBJS}

# --- qemu
QMU = qemu-system-riscv64
FQMU = -nographic -serial mon:stdio --no-reboot

qemu: out/kern.elf
	${QMU} -machine sifive_u -smp 5 -m 8G -bios none ${FQMU} -kernel $^

# --- alias
clean:
	rm -r out || true
	mkdir -p out

c: clean
q: qemu
cq: c q
