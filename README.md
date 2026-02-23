# UUNIX
tiny self-contained riscv64 machine mode kernel

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
# Priorities
This project is mainly supposed to serve as educational material for OS development.

## Discoverability
The reader should be grep through the source and read the cource of all identifiers.

Note, this also means losing out on compiler optimized implementation and compiler definitions for known-bit-width types.

## TBD

# Notes
This section is to serve as record keeping for architectural decisions. (not necessarily in order)

## C17 (ISO/IEC 9899:2018)
We choose C17 for this project because it is decently old (as of date), stable, and tested with no new language features over C11 and numerous defect fixes over C11.
Note, as of date, C23 is the latest std.

Check for C17 is hardcoded in `src/pre.h` to mitigate a decent amount of behavioural ambiguity.

## `src/pre.h`
This file serves as the contract between the source code and teh compiler. Though, including and using `<stdint.h>`, `<stddef.h>`, etc is technically the right decision, instead we do this for discoverability (see the Priorities section).
