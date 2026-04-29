all: c bin

.PHONY: qemu q clean c cq

CC = clang
OBJCPY = llvm-objcopy

F_CCONFORM = -std=c17 -pedantic -fno-gnu89-inline
F_WARNINGS = -Wall -Wextra -Weverything -Wno-unsafe-buffer-usage -Wno-pre-c11-compat -Wno-declaration-after-statement -Wc11-extensions
F_TARGET = --target=riscv64-freestanding-none -mcmodel=medany -mabi=lp64 -march=rv64imc_zaamo -mstrict-align -mtune=rocket
F_CODEGEN = -fno-stack-protector -ffreestanding -fno-pic -fno-pie
F_OPTIMIZE = -O3
F_EXTRA = # -gdwarf-5

LDFLAGS = --Map=out/kern.map
F_LINKER = -nostdlib -fuse-ld=lld -Wl,$(LDFLAGS)

########################## codegen
out/%.o: src/%.c
	$(CC) $(F_CCONFORM) $(F_WARNINGS) $(F_TARGET) $(F_CODEGEN) $(F_OPTIMIZE) $(F_EXTRA) -c -o $@ $<

out/base.o: src/base.S
	$(CC) $(F_TARGET) $(F_CODEGEN) $(F_OPTIMIZE) $(F_EXTRA) -c -o $@ $^

OBJS := $(patsubst src/%.c,out/%.o,$(wildcard src/*.c)) out/base.o

########################## linking
out/kern.elf: src/linker.ld $(OBJS)
	$(CC) $(F_TARGET) $(F_CODEGEN) $(F_OPTIMIZE) $(F_LINKER) $(F_EXTRA) -T $^ -o $@

out/kern.bin: out/kern.elf
	$(OBJCPY) -O binary $< $@

########################## qemu
QMU = qemu-system-riscv64

F_QMU = -nographic -serial mon:stdio -no-reboot
F_SIFIVE = -machine sifive_u -smp 5 -m 8G -bios none

qemu: out/kern.bin
	$(QMU) $(F_SIFIVE) $(F_QMU) -kernel $^

########################## alias
clean:
	rm -r out || true
	mkdir -p out

elf: out/kern.elf
bin: out/kern.bin

c: clean
q: qemu
cq: c q

cdebug: F_EXTRA += -gdwarf-5 -O0 -ggdb
cdebug: F_OPTIMIZE =
cdebug: c q
qdebug: F_QMU += -s -S
qdebug: cdebug qemu
qgdb: gdbinit.qemu
	gdb -x gdbinit.qemu
