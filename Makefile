libpath := $(shell bin/winsdk.exe --type:lib --arch:x64 --kit:um)

cflags += -Oz
cflags += -std=c11
cflags += -Wall
cflags += -Wextra
cflags += -Wno-gnu-empty-initializer
cflags += -pedantic
cflags += -nostdlib
cflags += -ffreestanding
cflags += -fno-stack-check
cflags += -fno-stack-protector
cflags += -mno-stack-arg-probe
cflags += -lkernel32
cflags += -Xlinker /align:16
cflags += -Xlinker /entry:start
cflags += -Xlinker /nodefaultlib
cflags += -Xlinker /subsystem:console
cflags += -Xlinker /libpath:"$(libpath)"

build: bin/smallist.exe bin/smallist32.exe sizes
	-

bench: bin/smallist.exe bin/smallist32.exe
	@$@

output: bin/smallist.exe
	bin\smallist > $@.txt

bin/smallist.exe: smallist.c Makefile
	clang $< $(cflags) -o $@
	llvm-strip -s $@

bin/smallist32.exe: smallist.c Makefile
	clang $< -m32 $(cflags) -o $@
	llvm-strip -s $@

sizes:
	dir /N bin\*list*.exe
