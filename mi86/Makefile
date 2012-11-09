all: io.o irq.o mem.o reg.o debug.o syscall.o lib.o
io.o: io.s
		cc -mi86 -Was-ncc -c io.s
irq.o: irq.s
		cc -mi86 -Was-ncc -c irq.s
mem.o: mem.s
		cc -mi86 -Was-ncc -c mem.s
reg.o: reg.s
		cc -mi86 -Was-ncc -c reg.s
debug.o: debug.s
		cc -mi86 -Was-ncc -c debug.s
syscall.o: syscall.s
		cc -mi86 -Was-ncc -c syscall.s
lib.o: lib.s
		cc -mi86 -Was-ncc -c lib.s
