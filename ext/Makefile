TARGET = --target=riscv64-freestanding-none -march=rv64imc_zaamo -mabi=lp64 -mcmodel=medany -mstrict-align -ffreestanding -fno-pic -fno-pie

all: out out/hello.hex

out: clean
	mkdir -p out

out/hello.elf: hello.S
	clang --target=riscv64-freestanding-none -march=rv64imc_zaamo -mabi=lp64 -mcmodel=medany -mstrict-align -ffreestanding -fno-pic -fno-pie -nostdlib -fuse-ld=lld -Wl,-e,_start,-Ttext=0,--image-base=0,--no-relax -o $@ $<

out/hello.bin: out/hello.elf
	llvm-objcopy -O binary -j .text $< $@

out/hello.hex: out/hello.bin
	od -An -tx1 -v $< | tr -d ' \n' > $@
	cat $@

clean:
	rm -rf out
