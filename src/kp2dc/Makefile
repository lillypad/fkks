.PHONY: extract-all
.PHONY: all

all: build

demo-all: build extract-sample strings-all clean

demo-domain: build extract-sample strings-domain clean

demo-key: build extract-sample strings-key clean

build:
	nasm -f elf32 -o bin/emu.o src/emu.asm
	gcc bin/emu.o src/main.c -g -no-pie -masm=intel -m32 -Wno-char-subscripts -o bin/kp2dc

strings-all:
	bin/kp2dc -h
	bin/kp2dc -a bin/sample.bin

strings-domain:
	bin/kp2dc -h
	bin/kp2dc -d bin/sample.bin | sed 's/http/hxxp/;s/\./\[\.\]/g'

strings-key:
	bin/kp2dc -h
	bin/kp2dc -k bin/sample.bin

clean:
	rm -f bin/emu.o bin/kp2dc bin/sample.bin

extract-sample:
	cd bin/ && \
		unzip -P infected sample.zip
