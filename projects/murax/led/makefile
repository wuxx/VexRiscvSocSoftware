PROJ_NAME=demo
DEBUG=yes
BENCH=no
MULDIV=no


SRCS = 	$(wildcard src/*.c) \
		$(wildcard src/*.cpp)  \
		$(wildcard src/*.S)


INC += -I../../../libs/
INC += -I../libs/
LDSCRIPT = ../libs/linker.ld

include ../../../resources/gcc.mk
include ../../../resources/subproject.mk


	
deploy:
	cp build/demo.elf ~/oss/icesugar/src/advanced/VexRiscv/src/main/ressource/hex/muraxDemo.elf
	cp build/demo.hex ~/oss/icesugar/src/advanced/VexRiscv/src/main/ressource/hex/muraxDemo.hex
