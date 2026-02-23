all: out/kern.elf

.PHONY: qemu q clean c

########################## compiler/assembler
# this must be a ISO C17 compiler
# this may be replaced with any other ISO C17 compiler
CC = clang -std=c17 -Wall -Wextra -Weverything -Wno-unsafe-buffer-usage -Wno-pre-c11-compat -pedantic -Wc11-extensions -fno-gnu89-inline -Wc11-extensions
FC =
FCR = -O3 -g0
FCX = -fno-stack-protector -ffreestanding -mcmodel=medany -fno-pie -fno-pic
FCM = --target=riscv64-freestanding-none

out/%.o: src/%.c
	${CC} ${FC} ${FCR} ${FCX} ${FCM} -c -o $@ $<

out/%.o: src/%.S
	${CC} ${FCX} ${FCM} -c -o $@ $<

OBJS := $(patsubst src/%.c,out/%.o,$(wildcard src/*.c)) \
        $(patsubst src/%.S,out/%.o,$(wildcard src/*.S))

########################## linker
LD = ld.lld
FLD = --no-pie
FLDR = --lto=full --strip-all
FLDM = -m elf64lriscv

out/kern.elf: ${OBJS} src/linker.ld
	${LD} ${FLD} ${FLDR} ${FLDM} -T src/linker.ld --Map=out/kern.map -o $@ ${OBJS}

########################## qemu
QMU = qemu-system-riscv64
FQMU = -nographic -serial mon:stdio --no-reboot

qemu: out/kern.elf
	${QMU} -machine sifive_u -smp 5 -m 8G -bios none ${FQMU} -kernel $^

########################## alias
clean:
	rm -r out || true
	mkdir -p out

c: clean
q: qemu
cq: c q
