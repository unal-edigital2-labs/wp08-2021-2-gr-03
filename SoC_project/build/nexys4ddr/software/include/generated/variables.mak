PACKAGES=libc libcompiler_rt libbase libfatfs liblitespi liblitedram libliteeth liblitesdcard liblitesata bios
PACKAGE_DIRS=/opt/litex/litex/litex/soc/software/libc /opt/litex/litex/litex/soc/software/libcompiler_rt /opt/litex/litex/litex/soc/software/libbase /opt/litex/litex/litex/soc/software/libfatfs /opt/litex/litex/litex/soc/software/liblitespi /opt/litex/litex/litex/soc/software/liblitedram /opt/litex/litex/litex/soc/software/libliteeth /opt/litex/litex/litex/soc/software/liblitesdcard /opt/litex/litex/litex/soc/software/liblitesata /opt/litex/litex/litex/soc/software/bios
LIBS=libc libcompiler_rt libbase libfatfs liblitespi liblitedram libliteeth liblitesdcard liblitesata
TRIPLE=riscv64-unknown-elf
CPU=vexriscv
CPUFAMILY=riscv
CPUFLAGS=-march=rv32im     -mabi=ilp32 -D__vexriscv__
CPUENDIANNESS=little
CLANG=0
CPU_DIRECTORY=/opt/litex/litex/litex/soc/cores/cpu/vexriscv
SOC_DIRECTORY=/opt/litex/litex/litex/soc
PICOLIBC_DIRECTORY=/opt/litex/pythondata-software-picolibc/pythondata_software_picolibc/data
COMPILER_RT_DIRECTORY=/opt/litex/pythondata-software-compiler_rt/pythondata_software_compiler_rt/data
export BUILDINC_DIRECTORY
BUILDINC_DIRECTORY=/home/daniela/Documentos/UNAL/2021_2/Digitales_2/Proyecto/wp08-2021-2-gr-03/SoC_project/build/nexys4ddr/software/include
LIBC_DIRECTORY=/opt/litex/litex/litex/soc/software/libc
LIBCOMPILER_RT_DIRECTORY=/opt/litex/litex/litex/soc/software/libcompiler_rt
LIBBASE_DIRECTORY=/opt/litex/litex/litex/soc/software/libbase
LIBFATFS_DIRECTORY=/opt/litex/litex/litex/soc/software/libfatfs
LIBLITESPI_DIRECTORY=/opt/litex/litex/litex/soc/software/liblitespi
LIBLITEDRAM_DIRECTORY=/opt/litex/litex/litex/soc/software/liblitedram
LIBLITEETH_DIRECTORY=/opt/litex/litex/litex/soc/software/libliteeth
LIBLITESDCARD_DIRECTORY=/opt/litex/litex/litex/soc/software/liblitesdcard
LIBLITESATA_DIRECTORY=/opt/litex/litex/litex/soc/software/liblitesata
BIOS_DIRECTORY=/opt/litex/litex/litex/soc/software/bios