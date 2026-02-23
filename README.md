# UUNIX
tiny riscv64 machine mode kernel

![riscvdemo](var/riscvdemo.png)

## Build
```bash
make clean # needed for creating some dirs the first time
make
```

## Run
```bash
make qemu
```

## Build-Run
```bash
make qc
```

## Debug builds
Just add the needed debug flags to `FCR` for C compiler and `FLDR` for linker.

For example,
```bash
make qc FCR="-gdwarf-5"
```
